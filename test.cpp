#include <iostream>
#include <fstream>
#include <filesystem>
#include "json.hpp" // Для работы со схемой schema.json
#include "structures.h"
#include "writeNReadCSV.h"
#include "lockUnlock.h"
#include <thread>
#include <mutex>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <atomic>

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

// Функция для создания директорий и файлов по схеме json
void createDirectoryAndFiles(const string& schemaName, const json& structure) {
    // Создаем директорию с названием схемы
    fs::create_directory(schemaName);

    // Проходим по всем таблицам в структуре
    for (const auto& table : structure) {
        string tableName = table["name"];
        SinglyLinkedList<string> columns;

        // Добавляем колонку с первичным ключом
        string pkColumnName = tableName + "_pk";
        columns.addToTail(pkColumnName);

        // Заполняем SinglyLinkedList названиями колонок
        for (const auto& column : table["columns"]) {
            columns.addToTail(column);
        }

        // Создаем поддиректорию для таблицы
        fs::path tablePath = fs::path(schemaName) / tableName;
        fs::create_directory(tablePath);

        // Создаем файл 1.csv в поддиректории таблицы
        fs::path filePath = tablePath / "1.csv";
        ofstream file(filePath);
        if (file.is_open()) {
            // Записываем названия колонок в файл
            SinglyLinkedList<string>::FLNode* columnNode = columns.head;
            while (columnNode) {
                file << columnNode->value;
                if (columnNode->next) {
                    file << ",";
                }
                columnNode = columnNode->next;
            }
            file << endl; // Добавляем перевод строки в конце
            file.close();
            cout << "Created file: " << filePath << endl;
        } else {
            cerr << "Failed to create file: " << filePath << endl;
        }

        // Создаем файл для хранения текущего первичного ключа
        fs::path pkSequencePath = tablePath / (pkColumnName + "_sequence");
        ofstream pkFile(pkSequencePath);
        if (pkFile.is_open()) {
            pkFile << "1" << endl; // Начальное значение первичного ключа
            pkFile.close();
            cout << "Created file: " << pkSequencePath << endl;
        } else {
            cerr << "Failed to create file: " << pkSequencePath << endl;
        }

        // Создаем файл для блокировки таблицы
        fs::path lockPath = tablePath / (tableName + "_lock");
        ofstream lockFile(lockPath);
        if (lockFile.is_open()) {
            lockFile << "unlocked" << endl; // Начальное состояние - разблокировано
            lockFile.close();
            cout << "Created file: " << lockPath << endl;
        } else {
            cerr << "Failed to create file: " << lockPath << endl;
        }
    }
}

Expression* parseWhereClause(const string& command, int& index, const SinglyLinkedList<string>& headers) {
    Expression* root = nullptr;
    Expression* current = nullptr;
    bool inQuotes = false;
    string tableName, columnName, operatorType, value;
    bool readingTableName = true;

    while (command[index] != '\0') {
        if (command[index] == '\'') {
            inQuotes = !inQuotes;
            index++;
            continue;
        }

        if (inQuotes) {
            value += command[index];
            index++;
            continue;
        }

        if (command[index] == ' ') {
            index++;
            continue;
        }

        if (command[index] == '.') {
            readingTableName = false;
            index++;
            continue;
        }

        if (command[index] == '=') {
            operatorType = "=";
            index++;
            continue;
        }

        if (command.substr(index, 3) == "AND") {
            index += 3;
            Expression* left = new Expression(ExpressionType::Comparison, columnName, operatorType, value);
            Expression* right = parseWhereClause(command, index, headers);
            root = new Expression(ExpressionType::Binary, "AND", left, right);
            break;
        }

        if (command.substr(index, 2) == "OR") {
            index += 2;
            Expression* left = new Expression(ExpressionType::Comparison, columnName, operatorType, value);
            Expression* right = parseWhereClause(command, index, headers);
            root = new Expression(ExpressionType::Binary, "OR", left, right);
            break;
        }

        if (readingTableName) {
            tableName += command[index];
        } else {
            columnName += command[index];
        }
        index++;
    }

    if (!root) {
        root = new Expression(ExpressionType::Comparison, columnName, operatorType, value);
    }

    return root;
}

// Функция для парсинга команды INSERT INTO
void parseInsertCommand(const string& command, string& tableName, SinglyLinkedList<string>& values) {
    int i = 0;

    // Пропускаем "INSERT INTO "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Получаем имя таблицы
    int start = i;
    while (command[i] != '\0' && command[i] != ' ') i++;
    tableName = command.substr(start, i - start);

    // Пропускаем " VALUES "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Пропускаем открывающую скобку
    if (command[i] == '(') i++;

    string substr; // Подстрока для сохранения данных
    bool inQuotes = false; // Флаг для отслеживания, находимся ли мы внутри кавычек

    // Парсим значения
    while (command[i] != '\0') {
        // Пропускаем открывающую кавычку
        if (command[i] == '\'') {
            inQuotes = !inQuotes;
            i++;
            continue;
        }

        // Если мы внутри кавычек, добавляем символы в substr
        if (inQuotes) {
            substr += command[i];
        } else {
            // Если мы вне кавычек и встречаем запятую или закрывающую скобку, добавляем значение в values
            if (command[i] == ',' || command[i] == ')') {
                values.addToTail(substr);
                substr.clear(); // Очищаем substr для следующего значения
            }
        }
        i++;
    }

    // Добавляем последнее значение, если оно есть
    if (!substr.empty()) {
        values.addToTail(substr);
    }
}

// Функция для вставки данных в таблицу
void insertIntoTable(const string& schemaName, const string& tableName, const SinglyLinkedList<string>& values, map<string, mutex>& tableMutexes) {
    fs::path tablePath = fs::path(schemaName) / tableName;
    fs::path filePath = tablePath / "1.csv";
    fs::path pkSequencePath = tablePath / (tableName + "_pk_sequence");

    lock_guard<mutex> lock(tableMutexes[tableName]);
    //// Блокируем таблицу
    //if (!lockTable(tablePath, tableName)) {
    //    cerr << "Table is locked. Cannot insert." << endl;
    //    return;
    //}

    // Читаем текущее значение первичного ключа
    ifstream pkFile(pkSequencePath);
    int pkValue = 1;
    if (pkFile.is_open()) {
        pkFile >> pkValue;
        pkFile.close();
    }

    // Увеличиваем значение первичного ключа
    pkValue++;

    // Записываем новое значение первичного ключа
    ofstream pkFileOut(pkSequencePath);
    if (pkFileOut.is_open()) {
        pkFileOut << pkValue << endl;
        pkFileOut.close();
    } else {
        cerr << "Failed to update primary key sequence file." << endl;
        //unlockTable(tablePath, tableName);
        return;
    }

    // Открываем файл таблицы в режиме добавления
    ofstream file(filePath, ios::app);
    if (file.is_open()) {
        // Записываем первичный ключ
        file << pkValue;

        // Записываем остальные значения
        SinglyLinkedList<string>::FLNode* valueNode = values.head;
        while (valueNode) {
            file << "," << valueNode->value;
            valueNode = valueNode->next;
        }
        file << endl; // Добавляем перевод строки в конце
        file.close();
        cout << "Inserted into table: " << tableName << endl;
    } else {
        cerr << "Failed to open file: " << filePath << endl;
    }

    //// Разблокируем таблицу
    //unlockTable(tablePath, tableName);
}

// Функция для парсинга команды DELETE FROM
void parseDeleteCommand(const string& command, string& tableName, Expression*& whereClause, const SinglyLinkedList<string>& headers) {
    int i = 0;

    // Пропускаем "DELETE FROM "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Получаем имя таблицы
    while (command[i] != '\0' && command[i] != ' ') {
        tableName += command[i];
        i++;
    }
    i++;

    // Пропускаем " WHERE ", если есть токен WHERE
    string subStrWhere;
    while (command[i] != '\0' && command[i] != ' ') {
        subStrWhere += command[i];
        i++;
    }

    if (subStrWhere == "WHERE") {
        i++;
        whereClause = parseWhereClause(command, i, headers);
    }
}

// Функция для удаления данных из таблицы с использованием дерева выражений
void deleteFromTable(const string& schemaName, const string& tableName, Expression* whereClause, map<string, mutex>& tableMutexes) {
    fs::path tablePath = fs::path(schemaName) / tableName;
    fs::path filePath = tablePath / "1.csv";

    lock_guard<mutex> lock(tableMutexes[tableName]);
    //// Блокируем таблицу
    //if (!lockTable(tablePath, tableName)) {
    //    cerr << "Table is locked. Cannot delete." << endl;
    //    return;
    //}

    // Читаем таблицу из файла
    SinglyLinkedList<string> headers;
    SinglyLinkedList<SinglyLinkedList<string>> table = readTableFromFile(filePath.string(), headers);

    // Удаляем строки, соответствующие условию
    SinglyLinkedList<SinglyLinkedList<string>>::FLNode* currentRow = table.head;
    SinglyLinkedList<SinglyLinkedList<string>>::FLNode* prevRow = nullptr;

    while (currentRow) {
        if (whereClause->evaluate(currentRow->value, headers)) {
            // Удаляем строку
            if (prevRow) {
                prevRow->next = currentRow->next;
            } else {
                table.head = currentRow->next;
            }
            SinglyLinkedList<SinglyLinkedList<string>>::FLNode* temp = currentRow;
            currentRow = currentRow->next;
            delete temp;
            continue; // Пропускаем обновление prevRow и currentRow
        }

        prevRow = currentRow;
        currentRow = currentRow->next;
    }

    // Записываем обновленную таблицу обратно в файл
    writeTableToFile(filePath.string(), table, headers);

    //// Разблокируем таблицу
    //unlockTable(tablePath, tableName);
}

void parseSelectCommand(const string& command, SinglyLinkedList<string>& columns, SinglyLinkedList<string>& tables,
Expression*& whereClause, const SinglyLinkedList<string>& headers) {
    int i = 0;
    string column;
    string table;

    // Пропускаем "SELECT "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Получаем колонки
    while (command[i] != '\0' && command[i] != ' ') {
        if (command[i] == ',') {
            columns.addToTail(column);
            column.clear();
        } else {
            column += command[i];
        }
        i++;
    }
    columns.addToTail(column);
    i++;

    // Пропускаем "FROM "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Получаем таблицы
    while (command[i] != '\0' && command[i] != ' ') {
        if (command[i] == ',') {
            tables.addToTail(table);
            table.clear();
        } else {
            table += command[i];
        }
        i++;
    }
    tables.addToTail(table);
    i++;
    if (command[i] == '\0'){
        return;
    }
    // Пропускаем "WHERE ", если есть токен WHERE
    string subStrWhere;
    while (command[i] != '\0' && command[i] != ' ') {
        subStrWhere += command[i];
        i++;
    }

    if (subStrWhere == "WHERE") {
        i++;
        whereClause = parseWhereClause(command, i, headers);
    }
}

// Функция для выполнения cross join двух таблиц
SinglyLinkedList<SinglyLinkedList<string>> crossJoin(const SinglyLinkedList<SinglyLinkedList<string>>& table1,
const SinglyLinkedList<SinglyLinkedList<string>>& table2) {
    SinglyLinkedList<SinglyLinkedList<string>> result;

    SinglyLinkedList<SinglyLinkedList<string>>::FLNode* row1Node = table1.head;
    while (row1Node) {
        SinglyLinkedList<SinglyLinkedList<string>>::FLNode* row2Node = table2.head;
        while (row2Node) {
            SinglyLinkedList<string> newRow;
            SinglyLinkedList<string>::FLNode* valueNode1 = row1Node->value.head;
            while (valueNode1) {
                newRow.addToTail(valueNode1->value);
                valueNode1 = valueNode1->next;
            }
            SinglyLinkedList<string>::FLNode* valueNode2 = row2Node->value.head;
            while (valueNode2) {
                newRow.addToTail(valueNode2->value);
                valueNode2 = valueNode2->next;
            }
            result.addToTail(newRow);
            row2Node = row2Node->next;
        }
        row1Node = row1Node->next;
    }

    return result;
}

string selectFromTables(const string& schemaName, const SinglyLinkedList<string>& columns, const SinglyLinkedList<string>& tables,
                        Expression* whereClause, SinglyLinkedList<string>& headers, map<string, mutex>& tableMutexes) {
    string result;

    if (tables.size() == 0) {
        result = "No tables specified.\n";
        return result;
    }

    // Список заблокированных таблиц
    SinglyLinkedList<string> lockedTables;

    // Загружаем первую таблицу и её заголовки
    SinglyLinkedList<string>::FLNode* tableNode = tables.head;
    fs::path tablePath = fs::path(schemaName) / tableNode->value;
    fs::path filePath = tablePath / "1.csv";

    lock_guard<mutex> lock(tableMutexes[tableNode->value]);
    lockedTables.addToTail(tableNode->value);

    SinglyLinkedList<SinglyLinkedList<string>> resultTable = readRowsFromFile(filePath.string(), headers);

    // Если запрошена всего одна таблица, просто выводим её данные
    if (tables.size() == 1) {
        // Определяем индексы нужных колонок
        SinglyLinkedList<int> columnIndices;
        SinglyLinkedList<string>::FLNode* columnNode = columns.head;
        while (columnNode) {
            string column = columnNode->value;
            size_t dotPos = column.find('.');
            if (dotPos != string::npos) {
                string tableName = column.substr(0, dotPos);
                string columnName = column.substr(dotPos + 1);

                // Ищем индекс колонки в заголовках
                SinglyLinkedList<string>::FLNode* headerNode = headers.head;
                int index = 0;
                while (headerNode) {
                    if (headerNode->value == columnName) {
                        columnIndices.addToTail(index);
                        break;
                    }
                    headerNode = headerNode->next;
                    index++;
                }
            }
            columnNode = columnNode->next;
        }

        // Выводим заголовки нужных колонок
        SinglyLinkedList<string>::FLNode* headerNode = headers.head;
        int index = 0;
        while (headerNode) {
            if (columnIndices.search(index)) {
                result += headerNode->value + " ";
            }
            headerNode = headerNode->next;
            index++;
        }
        result += "\n";

        // Выводим результирующую таблицу с нужными колонками
        SinglyLinkedList<SinglyLinkedList<string>>::FLNode* rowNode = resultTable.head;
        while (rowNode) {
            if (!whereClause || whereClause->evaluate(rowNode->value, headers)) {
                SinglyLinkedList<string>::FLNode* itemNode = rowNode->value.head;
                index = 0;
                while (itemNode) {
                    if (columnIndices.search(index)) {
                        result += itemNode->value + " ";
                    }
                    itemNode = itemNode->next;
                    index++;
                }
                result += "\n";
            }
            rowNode = rowNode->next;
        }

        return result;
    }

    // Выполняем cross join для каждой следующей таблицы
    tableNode = tableNode->next;
    while (tableNode) {
        fs::path nextFilePath = fs::path(schemaName) / tableNode->value / "1.csv";
        SinglyLinkedList<string> nextHeaders;
        SinglyLinkedList<SinglyLinkedList<string>> nextTable = readRowsFromFile(nextFilePath.string(), nextHeaders);

        lock_guard<mutex> lock(tableMutexes[tableNode->value]);
        lockedTables.addToTail(tableNode->value);

        // Объединяем заголовки
        SinglyLinkedList<string>::FLNode* headerNode = nextHeaders.head;
        while (headerNode) {
            headers.addToTail(headerNode->value);
            headerNode = headerNode->next;
        }

        // Выполняем cross join
        resultTable = crossJoin(resultTable, nextTable);
        tableNode = tableNode->next;
    }

    // Определяем индексы нужных колонок
    SinglyLinkedList<int> columnIndices;
    SinglyLinkedList<string>::FLNode* columnNode = columns.head;
    while (columnNode) {
        string column = columnNode->value;
        size_t dotPos = column.find('.');
        if (dotPos != string::npos) {
            string tableName = column.substr(0, dotPos);
            string columnName = column.substr(dotPos + 1);

            // Ищем индекс колонки в заголовках
            SinglyLinkedList<string>::FLNode* headerNode = headers.head;
            int index = 0;
            while (headerNode) {
                if (headerNode->value == columnName) {
                    columnIndices.addToTail(index);
                    break;
                }
                headerNode = headerNode->next;
                index++;
            }
        }
        columnNode = columnNode->next;
    }

    // Выводим заголовки нужных колонок
    SinglyLinkedList<string>::FLNode* headerNode = headers.head;
    int index = 0;
    while (headerNode) {
        if (columnIndices.search(index)) {
            result += headerNode->value + " ";
        }
        headerNode = headerNode->next;
        index++;
    }
    result += "\n";

    // Выводим результирующую таблицу с нужными колонками
    SinglyLinkedList<SinglyLinkedList<string>>::FLNode* rowNode = resultTable.head;
    while (rowNode) {
        if (!whereClause || whereClause->evaluate(rowNode->value, headers)) {
            SinglyLinkedList<string>::FLNode* itemNode = rowNode->value.head;
            index = 0;
            while (itemNode) {
                if (columnIndices.search(index)) {
                    result += itemNode->value + " ";
                }
                itemNode = itemNode->next;
                index++;
            }
            result += "\n";
        }
        rowNode = rowNode->next;
    }

    return result;
}

void handleRequest(int clientSocket, const string& schemaName, const json& structure, map<string, mutex>& tableMutexes) {
    try {
        while (true) {
            char buffer[1024] = {0};
            int valread = read(clientSocket, buffer, 1024);
            if (valread <= 0) {
                // Если не удалось прочитать данные, закрываем соединение
                cout << "Client disconnected. Socket: " << clientSocket << endl;
                close(clientSocket);
                return;
            }

            string request(buffer, valread);

            // Удаляем лишние символы \r\n
            request.erase(remove(request.begin(), request.end(), '\r'), request.end());
            request.erase(remove(request.begin(), request.end(), '\n'), request.end());

            cout << "Request from " << clientSocket << ": " << request << endl;

            // Проверяем, является ли запрос командой exit
            if (request == "exit") {
                cout << "Client requested to exit. Closing connection. Socket: " << clientSocket << endl;
                close(clientSocket);
                return;
            }

            // Обработка запроса
            if (request.substr(0, 12) == "INSERT INTO ") {
                string tableName;
                SinglyLinkedList<string> values;
                parseInsertCommand(request, tableName, values);
                insertIntoTable(schemaName, tableName, values, tableMutexes);
            } else if (request.substr(0, 12) == "DELETE FROM ") {
                string tableName;
                Expression* whereClause = nullptr;
                SinglyLinkedList<string> headers; // Добавляем список заголовков
                parseDeleteCommand(request, tableName, whereClause, headers);
                deleteFromTable(schemaName, tableName, whereClause, tableMutexes);
            } else if (request.substr(0, 7) == "SELECT ") {
                SinglyLinkedList<string> columns;
                SinglyLinkedList<string> tables;
                Expression* whereClause = nullptr;
                SinglyLinkedList<string> headers; // Добавляем список заголовков
                parseSelectCommand(request, columns, tables, whereClause, headers);
                string result = selectFromTables(schemaName, columns, tables, whereClause, headers, tableMutexes);

                // Отправка результата клиенту
                send(clientSocket, result.c_str(), result.size(), 0);
            } else {
                string response = "Unknown command\n";
                send(clientSocket, response.c_str(), response.size(), 0);
            }
        }
    } catch (exception& e) {
        cerr << "Exception in thread: " << e.what() << endl;
        close(clientSocket);
    }
}

int main() {
    // Путь к файлу конфигурации
    string configFilePath = "schema.json";

    // Открываем файл конфигурации
    ifstream configFile(configFilePath);

    // Читаем JSON из файла
    json config;
    configFile >> config;

    // Получаем название схемы и структуру таблиц
    string schemaName = config["name"];
    json structure = config["structure"];

    // Создаем директории и файлы
    //createDirectoryAndFiles(schemaName, structure);
    
    // Map будет хранить мьютексы для каждой таблицы
    map<string, mutex> tableMutexes;

    // Инициализируем мьютексы для каждой таблицы
    for (const auto& table : structure) {
        string tableName = table["name"];
        tableMutexes[tableName]; // Создаем мьютекс для каждой таблицы
    }

    //// Ожидаем ввода команд из консоли
    //while (true) {
    //    cout << "Enter command (or 'exit' to quit): ";
    //    string command;
    //    getline(cin, command);
//
    //    if (command == "exit") {
    //        break;
    //    }
//
    //    if (command.substr(0, 12) == "INSERT INTO ") {
    //        string tableName;
    //        SinglyLinkedList<string> values;
    //        parseInsertCommand(command, tableName, values);
    //        insertIntoTable(schemaName, tableName, values, tableMutexes);
    //    } else if (command.substr(0, 12) == "DELETE FROM ") {
    //        string tableName;
    //        Expression* whereClause = nullptr;
    //        SinglyLinkedList<string> headers; // Добавляем список заголовков
    //        parseDeleteCommand(command, tableName, whereClause, headers);
    //        deleteFromTable(schemaName, tableName, whereClause, tableMutexes);
    //    } else if (command.substr(0, 7) == "SELECT ") {
    //        SinglyLinkedList<string> columns;
    //        SinglyLinkedList<string> tables;
    //        Expression* whereClause = nullptr;
    //        SinglyLinkedList<string> headers; // Добавляем список заголовков
    //        parseSelectCommand(command, columns, tables, whereClause, headers);
    //        string result = selectFromTables(schemaName, columns, tables, whereClause, headers, tableMutexes);
    //        cout << result;
    //    } else {
    //        cout << "Unknown command" << endl;
    //    }
    //}

    // Создаем TCP-сервер
    int serverSocket, newSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Создаем сокет
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Привязываем сокет к порту 7432
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(7432);

    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Начинаем прослушивание
    if (listen(serverSocket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Server is running and waiting for connections..." << endl;

    while (true) {
        if ((newSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Создаем новый поток для обработки запроса
        cout << "Client connected. Socket: " << newSocket << endl;
        thread(handleRequest, newSocket, schemaName, structure, ref(tableMutexes)).detach();
    }

    return 0;
}