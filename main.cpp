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

    ~SinglyLinkedList() {
        while (head) {
            FLNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

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

    // Выполнение cross join и создание новой таблицы
    //HashTable crossJoin(const HashTable& otherTable, const string& column1, const string& column2) const {
    //    HashTable resultTable;
    //    resultTable.setTableSize(1); // Костыль
//
    //    if (!contains(column1) || !otherTable.contains(column2)) {
    //        cerr << "Column not found in one of the tables." << endl;
    //        return resultTable;
    //    }
//
    //    SinglyLinkedList<string> values1 = get(column1);
    //    SinglyLinkedList<string> values2 = otherTable.get(column2);
//
    //    SinglyLinkedList<string>::FLNode* node1 = values1.head;
    //    while (node1 != nullptr) {
    //        SinglyLinkedList<string>::FLNode* node2 = values2.head;
    //        while (node2 != nullptr) {
    //            resultTable.insert(column1, node1->value);
    //            resultTable.insert(column2, node2->value);
    //            node2 = node2->next;
    //        }
    //        node1 = node1->next;
    //    }
//
    //    return resultTable;
    //}
    // Функция для выполнения cross join двух таблиц
    // Функция для выполнения cross join двух таблиц
    // Функция для выполнения cross join двух таблиц
    HashTable crossJoin(const HashTable& otherTable) const {
        HashTable resultTable;
        resultTable.setTableSize(1);
    
        // Считываем строки из текущей таблицы
        SinglyLinkedList<SinglyLinkedList<string>> rows1;
        SinglyLinkedList<KeyValuePair*>::FLNode* current = table.head;
        while (current) {
            if (current->value) {
                SinglyLinkedList<string>::FLNode* valueNode = current->value->value.head;
                while (valueNode) {
                    SinglyLinkedList<string> row;
                    row.addToTail(valueNode->value);
                    rows1.addToTail(row);
                    valueNode = valueNode->next;
                }
            }
            current = current->next;
        }
    
        // Считываем строки из другой таблицы
        SinglyLinkedList<SinglyLinkedList<string>> rows2;
        current = otherTable.table.head;
        while (current) {
            if (current->value) {
                SinglyLinkedList<string>::FLNode* valueNode = current->value->value.head;
                while (valueNode) {
                    SinglyLinkedList<string> row;
                    row.addToTail(valueNode->value);
                    rows2.addToTail(row);
                    valueNode = valueNode->next;
                }
            }
            current = current->next;
        }
    
        // Создаем все возможные комбинации строк
        SinglyLinkedList<SinglyLinkedList<string>>::FLNode* row1Node = rows1.head;
        while (row1Node) {
            SinglyLinkedList<SinglyLinkedList<string>>::FLNode* row2Node = rows2.head;
            while (row2Node) {
                SinglyLinkedList<string> newRow = row1Node->value;
                SinglyLinkedList<string>::FLNode* valueNode = row2Node->value.head;
                while (valueNode) {
                    newRow.addToTail(valueNode->value);
                    valueNode = valueNode->next;
                }
                // Добавляем новую строку в результирующую таблицу
                SinglyLinkedList<string>::FLNode* newRowNode = newRow.head;
                while (newRowNode) {
                    resultTable.insert(current->value->key, newRowNode->value);
                    newRowNode = newRowNode->next;
                }
                row2Node = row2Node->next;
            }
            row1Node = row1Node->next;
        }
    
        return resultTable;
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

// Функция для создания директорий и файлов по схеме json
void createDirectoryAndFiles(const string& schemaName, const json& structure) {
    // Создаем директорию с названием схемы
    fs::create_directory(schemaName);

    // Проходим по всем таблицам в структуре
    for (const auto& table : structure) {
        string tableName = table["name"];
        SinglyLinkedList<string> columns;

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
    fs::path filePath = fs::path(schemaName) / tableName / "1.csv";
    ofstream file(filePath, ios::app); // Открываем файл в режиме добавления
    if (file.is_open()) {
        SinglyLinkedList<string>::FLNode* valueNode = values.head;
        while (valueNode) {
            file << valueNode->value;
            if (valueNode->next) {
                file << ",";
            }
            valueNode = valueNode->next;
        }
        file << endl; // Добавляем перевод строки в конце
        file.close();
        cout << "Inserted into table: " << tableName << endl;
    } else {
        cerr << "Failed to open file: " << filePath << endl;
    }
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
    fs::path filePath = fs::path(schemaName) / tableName / "1.csv";
    HashTable hashTable = readHashTableFromCSVFile(filePath.string());
    hashTable.printHashTable();
    hashTable.deleteRows(columnName, value);
    writeHashTableToCSVFile(filePath.string(), hashTable);
    cout << "Deleted from table: " << tableName << endl;
}

// Функция для парсинга команды SELECT FROM
void parseSelectCommand(const string& command, SinglyLinkedList<string>& columns, SinglyLinkedList<string>& tables) {
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
    while (command[i] != '\0') {
        if (command[i] == ',') {
            tables.addToTail(table);
            table.clear();
        } else {
            table += command[i];
        }
        i++;
    }
    tables.addToTail(table);
}

// Функция для выполнения команды SELECT FROM
void selectFromTables(const string& schemaName, const SinglyLinkedList<string>& columns, const SinglyLinkedList<string>& tables) {
    if (tables.size() == 0) {
        cout << "No tables specified." << endl;
        return;
    }

    // Загружаем первую таблицу
    SinglyLinkedList<string>::FLNode* tableNode = tables.head;
    fs::path filePath = fs::path(schemaName) / tableNode->value / "1.csv";
    HashTable resultTable = readHashTableFromCSVFile(filePath.string());

    // Если таблица всего одна, выбираем нужные колонки
    if (tables.size() == 1) {
        HashTable selectedColumnsTable;
        selectedColumnsTable.setTableSize(1);

        SinglyLinkedList<string>::FLNode* columnNode = columns.head;
        while (columnNode) {
            string column = columnNode->value;
            size_t dotPos = column.find('.');
            if (dotPos != string::npos) {
                string tableName = column.substr(0, dotPos);
                string columnName = column.substr(dotPos + 1);

                // Проверяем, что таблица соответствует текущей таблице
                if (tableName == tableNode->value) {
                    // Добавляем колонку в результирующую хеш-таблицу
                    SinglyLinkedList<string> values = resultTable.get(columnName);
                    SinglyLinkedList<string>::FLNode* valueNode = values.head;
                    while (valueNode) {
                        selectedColumnsTable.insert(columnName, valueNode->value);
                        valueNode = valueNode->next;
                    }
                }
            }
            columnNode = columnNode->next;
        }

        // Выводим результирующую хеш-таблицу
        selectedColumnsTable.printHashTable();
        return;
    }

    // Выполняем cross join для каждой следующей таблицы
    tableNode = tableNode->next;
    while (tableNode) {
        fs::path nextFilePath = fs::path(schemaName) / tableNode->value / "1.csv";
        HashTable nextTable = readHashTableFromCSVFile(nextFilePath.string());

        // Выполняем cross join
        HashTable tempTable;
        tempTable.setTableSize(1);

        SinglyLinkedList<KeyValuePair*>::FLNode* current = resultTable.table.head;
        while (current) {
            SinglyLinkedList<string>::FLNode* valueNode = current->value->value.head;
            while (valueNode) {
                SinglyLinkedList<KeyValuePair*>::FLNode* nextCurrent = nextTable.table.head;
                while (nextCurrent) {
                    SinglyLinkedList<string>::FLNode* nextValueNode = nextCurrent->value->value.head;
                    while (nextValueNode) {
                        tempTable.insert(current->value->key, valueNode->value);
                        tempTable.insert(nextCurrent->value->key, nextValueNode->value);
                        nextValueNode = nextValueNode->next;
                    }
                    nextCurrent = nextCurrent->next;
                }
                valueNode = valueNode->next;
            }
            current = current->next;
        }

        resultTable = tempTable;
        tableNode = tableNode->next;
    }

    // Выбираем нужные колонки из результата cross join
    HashTable selectedColumnsTable;
    selectedColumnsTable.setTableSize(1);

    SinglyLinkedList<string>::FLNode* columnNode = columns.head;
    while (columnNode) {
        string column = columnNode->value;
        size_t dotPos = column.find('.');
        if (dotPos != string::npos) {
            string tableName = column.substr(0, dotPos);
            string columnName = column.substr(dotPos + 1);

            // Добавляем колонку в результирующую хеш-таблицу
            SinglyLinkedList<string> values = resultTable.get(columnName);
            SinglyLinkedList<string>::FLNode* valueNode = values.head;
            while (valueNode) {
                selectedColumnsTable.insert(columnName, valueNode->value);
                valueNode = valueNode->next;
            }
        }
        columnNode = columnNode->next;
    }

    // Выводим результирующую хеш-таблицу
    selectedColumnsTable.printHashTable();
}

int main() {
    //// Путь к файлу конфигурации
    //string configFilePath = "schema.json";
//
    //// Открываем файл конфигурации
    //ifstream configFile(configFilePath);
//
    //// Читаем JSON из файла
    //json config;
    //configFile >> config;
//
    //// Получаем название схемы и структуру таблиц
    //string schemaName = config["name"];
    //json structure = config["structure"];
//
    //// Создаем директории и файлы
    ////createDirectoryAndFiles(schemaName, structure);
//
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
    //        insertIntoTable(schemaName, tableName, values);
    //    } else if (command.substr(0, 12) == "DELETE FROM ") {
    //        string tableName, columnName, value;
    //        parseDeleteCommand(command, tableName, columnName, value);
    //        deleteFromTable(schemaName, tableName, columnName, value);
    //    } else if (command.substr(0, 7) == "SELECT ") {
    //        SinglyLinkedList<string> columns;
    //        SinglyLinkedList<string> tables;
    //        parseSelectCommand(command, columns, tables);
    //        selectFromTables(schemaName, columns, tables);
    //    } else {
    //        cout << "Unknown command" << endl;
    //    }
    //}
    HashTable tableA;
    tableA.setTableSize(1);
    tableA.insert("ID", "1");
    tableA.insert("Name", "Alice");
    tableA.insert("ID", "2");
    tableA.insert("Name", "Bob");

    HashTable tableB;
    tableB.setTableSize(1);
    tableB.insert("ID", "1");
    tableB.insert("City", "New York");
    tableB.insert("ID", "2");
    tableB.insert("City", "Los Angeles");

    // Выполняем cross join
    HashTable resultTable;
    resultTable.setTableSize(1);
    resultTable = tableA.crossJoin(tableB);

    // Выводим результат
    resultTable.printHashTable();
    return 0;
}