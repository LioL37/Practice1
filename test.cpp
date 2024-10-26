#include <iostream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

template<typename T>
struct SinglyLinkedList {
    struct FLNode {
        FLNode* next;
        T value;
        FLNode(const T& val) : value(val), next(nullptr) {}
    };

    FLNode* head;
    SinglyLinkedList() : head(nullptr) {}

    // Добавление элемента в голову
    void addToHead(const T& value) {
        FLNode* newNode = new FLNode(value);
        newNode->next = head;
        head = newNode;
    }

    // Добавление элемента в хвост (сложность O(n))
    void addToTail(const T& value) {
        FLNode* newNode = new FLNode(value);
        if (!head) {
            head = newNode;
        } else {
            FLNode* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Удаление элемента с головы
    void removeFromHead() {
        if (!head) {
            throw runtime_error("List is empty");
        }
        FLNode* temp = head;
        head = head->next;
        delete temp;
    }

    // Удаление элемента с хвоста (сложность O(n))
    void removeFromTail() {
        if (!head) {
            throw runtime_error("List is empty");
        }
        if (!head->next) {
            delete head;
            head = nullptr;
        } else {
            FLNode* temp = head;
            while (temp->next->next) {
                temp = temp->next;
            }
            delete temp->next;
            temp->next = nullptr;
        }
    }

    // Удаление элемента по значению
    void removeByValue(const T& value) {
        FLNode* current = head;
        FLNode* previous = nullptr;
        while (current) {
            if (current->value == value) {
                if (previous) {
                    previous->next = current->next;
                } else {
                    head = current->next;
                }
                delete current;
                return;
            }
            previous = current;
            current = current->next;
        }
        throw runtime_error("Value not found");
    }

    // Поиск элемента по значению
    bool search(const T& value) const {
        FLNode* current = head;
        while (current) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Чтение списка
    void print() const {
        FLNode* current = head;
        while (current) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }

    // Установка элемента по индексу
    void set(int index, const T& value) {
        FLNode* current = head;
        for (int i = 0; i < index && current; ++i) {
            current = current->next;
        }
        if (current) {
            current->value = value;
        }
    }

    // Получение размера списка
    int size() const {
        int count = 0;
        FLNode* current = head;
        while (current) {
            ++count;
            current = current->next;
        }
        return count;
    }
};

// Структура для хранения пары ключ-значение
struct KeyValuePair {
    string key;
    SinglyLinkedList<string> value;  // Используем SinglyLinkedList для хранения значений
    KeyValuePair* next;

    KeyValuePair(const string& k) : key(k), next(nullptr) {}
};

struct HashTable {
    SinglyLinkedList<KeyValuePair*> table; // Односвязный список для хранения списков KeyValuePair
    int tableSize; // Размер таблицы

    // Хеш-функция
    int hashFunction(const string& key) const {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % tableSize;
    }

    HashTable() : tableSize(0) {}

    // Установка размера таблицы
    void setTableSize(int size) {
        tableSize = size;
    }

    // Вставка пары ключ-значение в хеш-таблицу
    void insert(const string& key, const string& value) {
        int index = hashFunction(key);
        SinglyLinkedList<KeyValuePair*>::FLNode* current = table.head;
        SinglyLinkedList<KeyValuePair*>::FLNode* prev = nullptr;

        // Найдем узел, соответствующий хеш-значению
        for (int i = 0; i < index && current; ++i) {
            prev = current;
            current = current->next;
        }

        // Если узел с таким индексом не существует, создаем новый
        if (current == nullptr) {
            KeyValuePair* newPair = new KeyValuePair(key);
            newPair->value.addToTail(value);
            SinglyLinkedList<KeyValuePair*>::FLNode* newNode = new SinglyLinkedList<KeyValuePair*>::FLNode(newPair);

            if (prev == nullptr) {
                // Если это первый элемент в списке
                table.head = newNode;
            } else {
                // Если это не первый элемент в списке
                prev->next = newNode;
            }
        } else {
            // Проверяем, существует ли уже элемент с таким ключом
            while (current != nullptr) {
                if (current->value && current->value->key == key) {
                    // Если ключ уже существует, добавляем значение в список
                    current->value->value.addToTail(value);
                    return;
                }
                prev = current;
                current = current->next;
            }

            // Если ключ не найден, добавляем новый элемент
            KeyValuePair* newPair = new KeyValuePair(key);
            newPair->value.addToTail(value);
            SinglyLinkedList<KeyValuePair*>::FLNode* newNode = new SinglyLinkedList<KeyValuePair*>::FLNode(newPair);

            if (prev == nullptr) {
                // Если это первый элемент в списке
                table.head = newNode;
            } else {
                // Если это не первый элемент в списке
                prev->next = newNode;
            }
        }
    }

    // Получение значений по ключу
    SinglyLinkedList<string> get(const string& key) const {
        int index = hashFunction(key);
        SinglyLinkedList<KeyValuePair*>::FLNode* current = table.head;
        for (int i = 0; i < index && current; ++i) {
            current = current->next;
        }

        while (current != nullptr) {
            if (current->value && current->value->key == key) {
                return current->value->value;
            }
            current = current->next;
        }

        return SinglyLinkedList<string>(); // Возвращаем пустой список, если ключ не найден
    }

    // Проверка наличия ключа
    bool contains(const string& key) const {
        int index = hashFunction(key);
        SinglyLinkedList<KeyValuePair*>::FLNode* current = table.head;
        for (int i = 0; i < index && current; ++i) {
            current = current->next;
        }

        while (current != nullptr) {
            if (current->value && current->value->key == key) {
                return true;
            }
            current = current->next;
        }

        return false;
    }

    // Удаление пары ключ-значение по ключу
    void remove(const string& key) {
        int index = hashFunction(key);
        SinglyLinkedList<KeyValuePair*>::FLNode* current = table.head;
        SinglyLinkedList<KeyValuePair*>::FLNode* prev = nullptr;
        for (int i = 0; i < index && current; ++i) {
            prev = current;
            current = current->next;
        }

        while (current != nullptr && (!current->value || current->value->key != key)) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr || !current->value) {
            // Ключ не найден
            return;
        }

        if (prev == nullptr) {
            // Если удаляем первый элемент в списке
            table.head = current->next;
        } else {
            // Если удаляем не первый элемент в списке
            prev->next = current->next;
        }

        delete current->value;
        delete current;
    }

    // Удаление строк, соответствующих условию
    void deleteRows(const string& columnName, const string& value) {
        SinglyLinkedList<KeyValuePair*>::FLNode* current = table.head;
        SinglyLinkedList<KeyValuePair*>::FLNode* prev = nullptr;

        while (current != nullptr) {
            if (current->value && current->value->key == columnName) {
                SinglyLinkedList<string>& values = current->value->value;
                SinglyLinkedList<string>::FLNode* node = values.head;
                SinglyLinkedList<string>::FLNode* prevNode = nullptr;

                while (node != nullptr) {
                    if (node->value == value) {
                        // Удаляем строку, соответствующую условию
                        if (prevNode == nullptr) {
                            values.head = node->next;
                        } else {
                            prevNode->next = node->next;
                        }
                        SinglyLinkedList<string>::FLNode* temp = node;
                        node = node->next;
                        delete temp;
                    } else {
                        prevNode = node;
                        node = node->next;
                    }
                }
            }

            prev = current;
            current = current->next;
        }
    }
    
    void printHashTable() const {
        SinglyLinkedList<KeyValuePair*>::FLNode* current = table.head;
        while (current) {
            if (current->value) {
                cout << current->value->key << ": ";
                SinglyLinkedList<string>::FLNode* valueNode = current->value->value.head;
                while (valueNode) {
                    cout << valueNode->value;
                    if (valueNode->next) {
                        cout << ", ";
                    }
                    valueNode = valueNode->next;
                }
                cout << endl;
            }
            current = current->next;
        }
        cout << "-----------------------------" << endl;
    }
};

// Функция для чтения хеш-таблицы из файла в формате .csv
HashTable readHashTableFromCSVFile(const string& filename) {
    ifstream file(filename);
    string line;
    HashTable hashTable;
    SinglyLinkedList<string> headers;

    // Читаем заголовки
    if (getline(file, line)) {
        stringstream ss(line);
        string header;
        while (getline(ss, header, ',')) {
            headers.addToTail(header);
        }
    }

    // Устанавливаем размер таблицы
    hashTable.setTableSize(1);// Здесь должно было быть headers.size()); но оно так не работает, поэтому
                              // использую костыль. С единицей читается таблица как надо
                              // Если использую размер, то появляются дубликаты колонок и они забирают себе элементы.
    // Читаем данные
    while (getline(file, line)) {
        stringstream ss(line);
        string value; // Значение из файла
        SinglyLinkedList<string>::FLNode* headerNode = headers.head;
        int index = 0;
        while (getline(ss, value, ',')) { // Разделение по запятым
            if (headerNode != nullptr) {
                // Добавляем значение в хеш-таблицу, используя текущий заголовок
                hashTable.insert(headerNode->value, value);
                headerNode = headerNode->next;
            }
            index++;
        }
    }

    file.close();
    return hashTable;
}
// Функция для записи хеш-таблицы в файл в формате .csv
void writeHashTableToCSVFile(const string& filename, const HashTable& hashTable) {
    ofstream file(filename);
    SinglyLinkedList<string> headers;
    SinglyLinkedList<SinglyLinkedList<string>> columns;

    // Собираем заголовки и столбцы
    SinglyLinkedList<KeyValuePair*>::FLNode* current = hashTable.table.head;
    while (current) {
        headers.addToTail(current->value->key);
        columns.addToTail(current->value->value);
        current = current->next;
    }

    // Записываем заголовки
    SinglyLinkedList<string>::FLNode* headerNode = headers.head;
    while (headerNode) {
        file << headerNode->value;
        if (headerNode->next) {
            file << ",";
        }
        headerNode = headerNode->next;
    }
    file << endl;

    // Записываем данные
    int rowCount = columns.head->value.size();
    for (int i = 0; i < rowCount; ++i) {
        SinglyLinkedList<SinglyLinkedList<string>>::FLNode* columnNode = columns.head;
        while (columnNode) {
            SinglyLinkedList<string>::FLNode* valueNode = columnNode->value.head;
            for (int j = 0; j < i && valueNode; ++j) {
                valueNode = valueNode->next;
            }
            if (valueNode) {
                file << valueNode->value;
            }
            if (columnNode->next) {
                file << ",";
            }
            columnNode = columnNode->next;
        }
        file << endl;
    }

    file.close();
}
// Функция для чтения таблицы из файла и разбиения строк на элементы
SinglyLinkedList<SinglyLinkedList<string>> readRowsFromFile(const string& filename) {
    ifstream file(filename);
    string line;
    SinglyLinkedList<SinglyLinkedList<string>> table;

    // Читаем файл построчно
    while (getline(file, line)) {
        SinglyLinkedList<string> row;
        stringstream ss(line);
        string item;

        // Разбиваем строку на элементы по запятым
        while (getline(ss, item, ',')) {
            row.addToTail(item);
        }

        // Добавляем строку в таблицу
        table.addToTail(row);
    }

    file.close();
    return table;
}
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
// Функция для блокировки таблицы
bool lockTable(const fs::path& tablePath, const string& tableName) {
    fs::path lockPath = tablePath / (tableName + "_lock");
    ifstream lockFile(lockPath);
    string lockStatus;
    lockFile >> lockStatus;
    lockFile.close();

    if (lockStatus == "locked") {
        return false; // Таблица уже заблокирована
    }

    ofstream lockFileOut(lockPath);
    if (lockFileOut.is_open()) {
        lockFileOut << "locked" << endl;
        lockFileOut.close();
        return true; // Таблица успешно заблокирована
    } else {
        cerr << "Failed to lock table: " << tableName << endl;
        return false;
    }
}
// Функция для разблокировки таблицы
void unlockTable(const fs::path& tablePath, const string& tableName) {
    fs::path lockPath = tablePath / (tableName + "_lock");
    ofstream lockFileOut(lockPath);
    if (lockFileOut.is_open()) {
        lockFileOut << "unlocked" << endl;
        lockFileOut.close();
    } else {
        cerr << "Failed to unlock table: " << tableName << endl;
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

    // Пропускаем " = "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Получаем значение
    start = i;
    while (command[i] != '\0' && command[i] != '\'') i++;
    value = command.substr(start, i - start);
}

// Функция для удаления данных из таблицы
void deleteFromTable(const string& schemaName, const string& tableName, const string& columnName, const string& value) {
    fs::path tablePath = fs::path(schemaName) / tableName;
    fs::path filePath = tablePath / "1.csv";

    // Блокируем таблицу
    if (!lockTable(tablePath, tableName)) {
        cerr << "Table is locked. Cannot delete." << endl;
        return;
    }

    HashTable hashTable = readHashTableFromCSVFile(filePath.string());
    hashTable.printHashTable();
    hashTable.deleteRows(columnName, value);
    writeHashTableToCSVFile(filePath.string(), hashTable);
    cout << "Deleted from table: " << tableName << endl;

    // Разблокируем таблицу
    unlockTable(tablePath, tableName);
}

// Функция для чтения таблицы из файла и разбиения строк на элементы
// Функция для чтения таблицы из файла и разбиения строк на элементы
SinglyLinkedList<SinglyLinkedList<string>> readRowsFromFile(const string& filename, SinglyLinkedList<string>& headers) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        throw runtime_error("File open error");
    }

    string line;
    SinglyLinkedList<SinglyLinkedList<string>> table;

    // Читаем первую строку как заголовки
    if (getline(file, line)) {
        stringstream ss(line);
        string item;
        while (getline(ss, item, ',')) {
            headers.addToTail(item);
        }
    }

    // Читаем файл построчно
    while (getline(file, line)) {
        SinglyLinkedList<string> row;
        stringstream ss(line);
        string item;

        // Разбиваем строку на элементы по запятым
        while (getline(ss, item, ',')) {
            row.addToTail(item);
        }

        // Добавляем строку в таблицу
        table.addToTail(row);
    }

    file.close();
    return table;
}

// Структура для хранения условия WHERE
struct WhereCondition {
    string tableName;
    string columnName;
    string operatorType;
    string value;
    bool isAnd;
    bool isOr;

    WhereCondition(const string& table, const string& column, const string& op, const string& val, bool andOp, bool orOp)
        : tableName(table), columnName(column), operatorType(op), value(val), isAnd(andOp), isOr(orOp) {}
};
struct Expression {
    virtual bool evaluate(const SinglyLinkedList<string>& row, const SinglyLinkedList<string>& headers) const = 0;
    virtual ~Expression() {}
};
struct BinaryExpression : public Expression {
    string op;
    Expression* left;
    Expression* right;

    BinaryExpression(const string& op, Expression* left, Expression* right)
        : op(op), left(left), right(right) {}

    bool evaluate(const SinglyLinkedList<string>& row, const SinglyLinkedList<string>& headers) const override {
        if (op == "AND") {
            return left->evaluate(row, headers) && right->evaluate(row, headers);
        } else if (op == "OR") {
            return left->evaluate(row, headers) || right->evaluate(row, headers);
        }
        return false;
    }

    ~BinaryExpression() {
        delete left;
        delete right;
    }
};

struct ComparisonExpression : public Expression {
    string columnName;
    string operatorType;
    string value;

    ComparisonExpression(const string& column, const string& op, const string& val)
        : columnName(column), operatorType(op), value(val) {}

    bool evaluate(const SinglyLinkedList<string>& row, const SinglyLinkedList<string>& headers) const override {
        // Ищем индекс колонки в заголовках
        int index = 0;
        SinglyLinkedList<string>::FLNode* headerNode = headers.head;
        while (headerNode) {
            if (headerNode->value == columnName) {
                break;
            }
            headerNode = headerNode->next;
            index++;
        }

        // Проверяем значение строки по индексу колонки
        SinglyLinkedList<string>::FLNode* rowNode = row.head;
        for (int i = 0; i < index && rowNode; ++i) {
            rowNode = rowNode->next;
        }
        if (rowNode) {
            if (operatorType == "=") {
                return rowNode->value == value;
            }
        }
        return false;
    }
};

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
            Expression* left = new ComparisonExpression(columnName, operatorType, value);
            Expression* right = parseWhereClause(command, index, headers);
            root = new BinaryExpression("AND", left, right);
            break;
        }

        if (command.substr(index, 2) == "OR") {
            index += 2;
            Expression* left = new ComparisonExpression(columnName, operatorType, value);
            Expression* right = parseWhereClause(command, index, headers);
            root = new BinaryExpression("OR", left, right);
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
        root = new ComparisonExpression(columnName, operatorType, value);
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
    if (command.substr(i, 5) == "WHERE") {
        i += 5;
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

// Функция для выполнения команды SELECT FROM
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