#include <iostream>
#include <fstream>
#include <filesystem>
#include "json.hpp"
#include "structures.h"
#include "writeNReadCSV.h"
#include "lockUnlock.h"

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
void insertIntoTable(const string& schemaName, const string& tableName, const SinglyLinkedList<string>& values) {
    fs::path tablePath = fs::path(schemaName) / tableName;
    fs::path filePath = tablePath / "1.csv";
    fs::path pkSequencePath = tablePath / (tableName + "_pk_sequence");

    // Блокируем таблицу
    if (!lockTable(tablePath, tableName)) {
        cerr << "Table is locked. Cannot insert." << endl;
        return;
    }

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
        unlockTable(tablePath, tableName);
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

    // Разблокируем таблицу
    unlockTable(tablePath, tableName);
}

// Функция для парсинга команды DELETE FROM
void parseDeleteCommand(const string& command, string& tableName, string& columnName, string& value) {
    int i = 0;

    // Пропускаем "DELETE FROM "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Получаем имя таблицы
    int start = i;
    while (command[i] != '\0' && command[i] != ' ') i++;
    tableName = command.substr(start, i - start);

    // Пропускаем " WHERE "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Получаем имя колонки
    start = i;
    while (command[i] != '\0' && command[i] != ' ') i++;
    columnName = command.substr(start, i - start);
    i++;

    // Пропускаем " = "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Получаем значение
    if (command[i] == '\''){
        i++;
        string subStr;
        while (command[i] != '\0' && command[i] != '\'') {
            subStr += command[i];
            i++;
        }
        value = subStr;
    }
}
void deleteFromTable(const string& schemaName, const string& tableName, const string& columnName, const string& value) {
    fs::path tablePath = fs::path(schemaName) / tableName;
    fs::path filePath = tablePath / "1.csv";

    // Блокируем таблицу
    if (!lockTable(tablePath, tableName)) {
        cerr << "Table is locked. Cannot delete." << endl;
        return;
    }

    // Читаем таблицу из файла
    SinglyLinkedList<string> headers;
    SinglyLinkedList<SinglyLinkedList<string>> table = readTableFromFile(filePath.string(), headers);

    // Удаляем строки, соответствующие условию
    SinglyLinkedList<SinglyLinkedList<string>>::FLNode* currentRow = table.head;
    SinglyLinkedList<SinglyLinkedList<string>>::FLNode* prevRow = nullptr;

    while (currentRow) {
        SinglyLinkedList<string>::FLNode* currentCell = currentRow->value.head;
        SinglyLinkedList<string>::FLNode* headerNode = headers.head;
        int index = 0;

        // Находим индекс колонки по имени
        while (headerNode && headerNode->value != columnName) {
            headerNode = headerNode->next;
            index++;
        }

        // Проверяем значение в найденной колонке
        if (headerNode) {
            // Перемещаемся к нужному элементу в строке
            for (int i = 0; i < index && currentCell; ++i) {
                currentCell = currentCell->next;
            }

            // Проверяем значение в найденной колонке
            if (currentCell && currentCell->value == value) {
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
        }

        prevRow = currentRow;
        currentRow = currentRow->next;
    }

    // Записываем обновленную таблицу обратно в файл
    writeTableToFile(filePath.string(), table, headers);

    // Разблокируем таблицу
    unlockTable(tablePath, tableName);
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

void parseSelectCommand(const string& command, SinglyLinkedList<string>& columns, SinglyLinkedList<string>& tables, Expression*& whereClause, const SinglyLinkedList<string>& headers) {
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
SinglyLinkedList<SinglyLinkedList<string>> crossJoin(const SinglyLinkedList<SinglyLinkedList<string>>& table1, const SinglyLinkedList<SinglyLinkedList<string>>& table2) {
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

void selectFromTables(const string& schemaName, const SinglyLinkedList<string>& columns, const SinglyLinkedList<string>& tables, Expression* whereClause, SinglyLinkedList<string>& headers) {
    if (tables.size() == 0) {
        cout << "No tables specified." << endl;
        return;
    }

    // Список заблокированных таблиц
    SinglyLinkedList<string> lockedTables;

    // Загружаем первую таблицу и её заголовки
    SinglyLinkedList<string>::FLNode* tableNode = tables.head;
    fs::path tablePath = fs::path(schemaName) / tableNode->value;
    fs::path filePath = tablePath / "1.csv";

    // Блокируем таблицу
    if (!lockTable(tablePath, tableNode->value)) {
        cerr << "Table is locked. Cannot select." << endl;
        return;
    }
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
                cout << headerNode->value << " ";
            }
            headerNode = headerNode->next;
            index++;
        }
        cout << endl;

        // Выводим результирующую таблицу с нужными колонками
        SinglyLinkedList<SinglyLinkedList<string>>::FLNode* rowNode = resultTable.head;
        while (rowNode) {
            if (!whereClause || whereClause->evaluate(rowNode->value, headers)) {
                SinglyLinkedList<string>::FLNode* itemNode = rowNode->value.head;
                index = 0;
                while (itemNode) {
                    if (columnIndices.search(index)) {
                        cout << itemNode->value << " ";
                    }
                    itemNode = itemNode->next;
                    index++;
                }
                cout << endl;
            }
            rowNode = rowNode->next;
        }

        // Разблокируем таблицу
        unlockTable(tablePath, tableNode->value);
        return;
    }

    // Выполняем cross join для каждой следующей таблицы
    tableNode = tableNode->next;
    while (tableNode) {
        fs::path nextFilePath = fs::path(schemaName) / tableNode->value / "1.csv";
        SinglyLinkedList<string> nextHeaders;
        SinglyLinkedList<SinglyLinkedList<string>> nextTable = readRowsFromFile(nextFilePath.string(), nextHeaders);

        // Блокируем таблицу
        if (!lockTable(fs::path(schemaName) / tableNode->value, tableNode->value)) {
            cerr << "Table is locked. Cannot select." << endl;
            // Разблокируем все заблокированные таблицы
            SinglyLinkedList<string>::FLNode* lockedTableNode = lockedTables.head;
            while (lockedTableNode) {
                unlockTable(fs::path(schemaName) / lockedTableNode->value, lockedTableNode->value);
                lockedTableNode = lockedTableNode->next;
            }
            return;
        }
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
            cout << headerNode->value << " ";
        }
        headerNode = headerNode->next;
        index++;
    }
    cout << endl;

    // Выводим результирующую таблицу с нужными колонками
    SinglyLinkedList<SinglyLinkedList<string>>::FLNode* rowNode = resultTable.head;
    while (rowNode) {
        if (!whereClause || whereClause->evaluate(rowNode->value, headers)) {
            SinglyLinkedList<string>::FLNode* itemNode = rowNode->value.head;
            index = 0;
            while (itemNode) {
                if (columnIndices.search(index)) {
                    cout << itemNode->value << " ";
                }
                itemNode = itemNode->next;
                index++;
            }
            cout << endl;
        }
        rowNode = rowNode->next;
    }

    // Разблокируем все заблокированные таблицы
    SinglyLinkedList<string>::FLNode* lockedTableNode = lockedTables.head;
    while (lockedTableNode) {
        unlockTable(fs::path(schemaName) / lockedTableNode->value, lockedTableNode->value);
        lockedTableNode = lockedTableNode->next;
    }

    // Освобождаем память, выделенную для дерева выражений
    delete whereClause;
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

    // Ожидаем ввода команд из консоли
    while (true) {
        cout << "Enter command (or 'exit' to quit): ";
        string command;
        getline(cin, command);

        if (command == "exit") {
            break;
        }

        if (command.substr(0, 12) == "INSERT INTO ") {
            string tableName;
            SinglyLinkedList<string> values;
            parseInsertCommand(command, tableName, values);
            insertIntoTable(schemaName, tableName, values);
        } else if (command.substr(0, 12) == "DELETE FROM ") {
            string tableName, columnName, value;
            parseDeleteCommand(command, tableName, columnName, value);
            deleteFromTable(schemaName, tableName, columnName, value);
        } else if (command.substr(0, 7) == "SELECT ") {
            SinglyLinkedList<string> columns;
            SinglyLinkedList<string> tables;
            Expression* whereClause = nullptr;
            SinglyLinkedList<string> headers; // Добавляем список заголовков
            parseSelectCommand(command, columns, tables, whereClause, headers);
            selectFromTables(schemaName, columns, tables, whereClause, headers);
        } else {
            cout << "Unknown command" << endl;
        }
    }
    return 0;
}