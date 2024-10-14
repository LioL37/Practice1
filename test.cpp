#include <iostream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

template<typename T>
struct CustomVector {
    T* data;           // Указатель на элементы
    size_t size;        // Текущий размер массива
    size_t capacity;    // Вместимость массива

    // Увеличиваем вместимость массива, если необходимо
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

    // Конструктор по умолчанию
    CustomVector() : data(nullptr), size(0), capacity(0) {}

    // Конструктор с начальной вместимостью
    CustomVector(size_t initial_capacity) : data(new T[initial_capacity]), size(0), capacity(initial_capacity) {}

    // Деструктор
    //~CustomVector() {
    //    if (data != nullptr) {
    //        delete[] data;
    //        data = nullptr;
    //    }
    //}

    // Добавление элемента в конец массива
    void push_back(const T& value) {
        if (size == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;
    }

    // Добавление элемента по индексу
    void insert(size_t index, const T& value) {
        if (index > size) {
            throw out_of_range("Index out of range");
        }
        if (size == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        for (size_t i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        ++size;
    }

    // Получение элемента по индексу
    T get(size_t index) const {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    // Удаление элемента по индексу
    void remove(size_t index) {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        --size;
    }

    // Замена элемента по индексу
    void set(size_t index, const T& value) {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        data[index] = value;
    }

    // Длина массива
    size_t length() const {
        return size;
    }

    // Чтение массива
    void print() const {
        for (size_t i = 0; i < size; ++i) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
    void begin(){

    }
};

// Структура для хранения пары ключ-значение
struct KeyValuePair {
    string key;
    CustomVector<string> value;  // Используем CustomVector для хранения значений
    KeyValuePair* next;

    KeyValuePair(const string& k) : key(k), next(nullptr) {}
};

// Динамический массив для хранения указателей на KeyValuePair
struct DynamicArray {
    KeyValuePair** array;
    int size;
    int capacity;

    DynamicArray(int initialCapacity = 10) : size(0), capacity(initialCapacity) {
        array = new KeyValuePair*[capacity];
        for (int i = 0; i < capacity; ++i) {
            array[i] = nullptr;
        }
    }

    //~DynamicArray() {
    //    delete[] array;
    //}

    void resize(int newCapacity) {
        KeyValuePair** newArray = new KeyValuePair*[newCapacity];
        for (int i = 0; i < size; ++i) {
            newArray[i] = array[i];
        }
        for (int i = size; i < newCapacity; ++i) {
            newArray[i] = nullptr;
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    void add(KeyValuePair* element) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        array[size++] = element;
    }

    KeyValuePair* get(int index) const {
        if (index < 0 || index >= capacity) {
            return nullptr;
        }
        return array[index];
    }

    void set(int index, KeyValuePair* element) {
        if (index < 0 || index >= capacity) {
            return;
        }
        array[index] = element;
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }
};

struct HashTable {
    DynamicArray table; // Динамический массив для хранения списков KeyValuePair

    // Хеш-функция
    int hashFunction(const string& key) const {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % table.getCapacity();
    }

    HashTable() : table(10) {} // Инициализация с начальной емкостью 10

    // Вставка пары ключ-значение в хеш-таблицу
    void insert(const string& key, const string& value) {
        int index = hashFunction(key);
        KeyValuePair* current = table.get(index);

        // Проверяем, существует ли уже элемент с таким ключом
        while (current != nullptr) {
            if (current->key == key) {
                // Если ключ уже существует, добавляем значение в массив
                current->value.push_back(value);
                return;
            }
            current = current->next;
        }

        // Если ключ не найден, добавляем новый элемент
        KeyValuePair* newPair = new KeyValuePair(key);
        newPair->value.push_back(value);
        if (table.get(index) == nullptr) {
            // Если ячейка пуста, просто добавляем новую пару
            table.set(index, newPair);
        } else {
            // Если ячейка занята, добавляем новую пару в начало списка
            newPair->next = table.get(index);
            table.set(index, newPair);
        }
    }

    // Получение значений по ключу
    CustomVector<string> get(const string& key) const {
        int index = hashFunction(key);
        KeyValuePair* current = table.get(index);

        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        return CustomVector<string>(); // Возвращаем пустой массив, если ключ не найден
    }

    // Проверка наличия ключа
    bool contains(const string& key) const {
        int index = hashFunction(key);
        KeyValuePair* current = table.get(index);

        while (current != nullptr) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }

        return false;
    }

    // Удаление пары ключ-значение по ключу
    void remove(const string& key) {
        int index = hashFunction(key);
        KeyValuePair* current = table.get(index);
        KeyValuePair* prev = nullptr;

        while (current != nullptr && current->key != key) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            // Ключ не найден
            return;
        }

        if (prev == nullptr) {
            // Если удаляем первый элемент в списке
            table.set(index, current->next);
        } else {
            // Если удаляем не первый элемент в списке
            prev->next = current->next;
        }

        delete current;
    }

    // Удаление строк, соответствующих условию
    void deleteRows(const string& columnName, const string& value) {
        for (int i = 0; i < table.getCapacity(); ++i) {
            KeyValuePair* current = table.get(i);
            while (current != nullptr) {
                if (current->key == columnName) {
                    CustomVector<string>& values = current->value;
                    for (size_t j = 0; j < values.length(); ++j) {
                        if (values.get(j) == value) {
                            // Удаляем строку, соответствующую условию
                            for (int k = 0; k < table.getCapacity(); ++k) {
                                KeyValuePair* col = table.get(k);
                                while (col != nullptr) {
                                    col->value.remove(j);
                                    col = col->next;
                                }
                            }
                            --j; // Уменьшаем индекс, так как размер массива уменьшился
                        }
                    }
                }
                current = current->next;
            }
        }
    }

    // Выполнение cross join и создание новой таблицы
    HashTable crossJoin(const HashTable& otherTable, const string& column1, const string& column2) const {
        HashTable resultTable;

        if (!contains(column1) || !otherTable.contains(column2)) {
            cerr << "Column not found in one of the tables." << endl;
            return resultTable;
        }

        CustomVector<string> values1 = get(column1);
        CustomVector<string> values2 = otherTable.get(column2);

        for (size_t i = 0; i < values1.length(); ++i) {
            for (size_t j = 0; j < values2.length(); ++j) {
                resultTable.insert(column1, values1.get(i));
                resultTable.insert(column2, values2.get(j));
            }
        }

        return resultTable;
    }

    // Деструктор для очистки памяти
    //~HashTable() {
    //    for (int i = 0; i < table.getCapacity(); ++i) {
    //        KeyValuePair* current = table.get(i);
    //        while (current != nullptr) {
    //            KeyValuePair* next = current->next;
    //            delete current;
    //            current = next;
    //        }
    //    }
    //}
};

// Функция для чтения хеш-таблицы из файла в формате .csv
HashTable readHashTableFromCSVFile(const string& filename) {
    ifstream file(filename);
    string line;
    HashTable hashTable;
    CustomVector<string> headers;

    // Читаем заголовки
    if (getline(file, line)) {
        stringstream ss(line);
        string header;
        while (getline(ss, header, ',')) {
            headers.push_back(header);
        }
    }

    // Читаем данные
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        int columnIndex = 0;
        while (getline(ss, value, ',')) {
            if (columnIndex < headers.length()) {
                hashTable.insert(headers.get(columnIndex), value);
            }
            columnIndex++;
        }
    }

    file.close();
    return hashTable;
}

// Функция для записи хеш-таблицы в файл в формате .csv
void writeHashTableToCSVFile(const string& filename, const HashTable& hashTable) {
    ofstream file(filename);
    CustomVector<string> headers;
    CustomVector<CustomVector<string>> columns;

    // Собираем заголовки и столбцы
    for (int i = 0; i < hashTable.table.getCapacity(); ++i) {
        KeyValuePair* current = hashTable.table.get(i);
        while (current) {
            headers.push_back(current->key);
            columns.push_back(current->value);
            current = current->next;
        }
    }

    // Записываем заголовки
    for (size_t i = 0; i < headers.length(); ++i) {
        file << headers.get(i);
        if (i < headers.length() - 1) {
            file << ",";
        }
    }
    file << endl;

    // Записываем данные
    size_t maxRows = 0;
    for (size_t i = 0; i < columns.length(); ++i) {
        if (columns.get(i).length() > maxRows) {
            maxRows = columns.get(i).length();
        }
    }

    for (size_t row = 0; row < maxRows; ++row) {
        for (size_t col = 0; col < columns.length(); ++col) {
            if (row < columns.get(col).length()) {
                file << columns.get(col).get(row);
            }
            if (col < columns.length() - 1) {
                file << ",";
            }
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
        CustomVector<string> columns;

        // Заполняем CustomVector названиями колонок
        for (const auto& column : table["columns"]) {
            columns.push_back(column);
        }

        // Создаем поддиректорию для таблицы
        fs::path tablePath = fs::path(schemaName) / tableName;
        fs::create_directory(tablePath);

        // Создаем файл 1.csv в поддиректории таблицы
        fs::path filePath = tablePath / "1.csv";
        ofstream file(filePath);
        if (file.is_open()) {
            // Записываем названия колонок в файл
            for (size_t i = 0; i < columns.length(); ++i) {
                file << columns.get(i);
                if (i < columns.length() - 1) {
                    file << ",";
                }
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
void parseInsertCommand(const string& command, string& tableName, CustomVector<string>& values) {
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
                values.push_back(substr);
                substr.clear(); // Очищаем substr для следующего значения
            }
        }
        i++;
    }

    // Добавляем последнее значение, если оно есть
    if (!substr.empty()) {
        values.push_back(substr);
    }
}

// Функция для вставки данных в таблицу
void insertIntoTable(const string& schemaName, const string& tableName, const CustomVector<string>& values) {
    fs::path filePath = fs::path(schemaName) / tableName / "1.csv";
    ofstream file(filePath, ios::app); // Открываем файл в режиме добавления
    if (file.is_open()) {
        for (size_t i = 0; i < values.length(); ++i) {
            file << values.get(i);
            if (i < values.length() - 1) {
                file << ",";
            }
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
    hashTable.deleteRows(columnName, value);
    writeHashTableToCSVFile(filePath.string(), hashTable);
    cout << "Deleted from table: " << tableName << endl;
}

// Функция для парсинга команды SELECT FROM
void parseSelectCommand(const string& command, CustomVector<string>& columns, CustomVector<string>& tables) {
    int i = 0;
    string column;
    string table;

    // Пропускаем "SELECT "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Получаем колонки
    while (command[i] != '\0' && command[i] != ' ') {
        if (command[i] == ',') {
            columns.push_back(column);
            column.clear();
        } else if (command[i] == '.') {
            table = column;
            column.clear();
        } else {
            column += command[i];
        }
        i++;
    }
    columns.push_back(column);

    // Пропускаем "FROM "
    while (command[i] != '\0' && command[i] != ' ') i++;
    i++;

    // Получаем таблицы
    while (command[i] != '\0') {
        if (command[i] == ',') {
            tables.push_back(table);
            table.clear();
        } else {
            table += command[i];
        }
        i++;
    }
    tables.push_back(table);
}

// Функция для выполнения команды SELECT FROM
void selectFromTables(const string& schemaName, const CustomVector<string>& columns, const CustomVector<string>& tables) {
    CustomVector<HashTable> hashTables;
    CustomVector<string> headers;

    // Загружаем таблицы
    for (size_t i = 0; i < tables.length(); ++i) {
        fs::path filePath = fs::path(schemaName) / tables.get(i) / "1.csv";
        hashTables.push_back(readHashTableFromCSVFile(filePath.string()));
    }

    // Создаем заголовки
    for (size_t i = 0; i < columns.length(); ++i) {
        headers.push_back(columns.get(i));
    }

    // Выводим заголовки
    for (size_t i = 0; i < headers.length(); ++i) {
        cout << headers.get(i);
        if (i < headers.length() - 1) {
            cout << ",";
        }
    }
    cout << endl;

    // Сопоставляем колонки с таблицами
    CustomVector<string> tableNames;
    CustomVector<string> columnNames;
    for (size_t i = 0; i < columns.length(); ++i) {
        string column = columns.get(i);
        size_t dotPos = -1;
        for (size_t j = 0; j < column.length(); ++j) {
            if (column[j] == '.') {
                dotPos = j;
                break;
            }
        }
        if (dotPos != -1) {
            string tableName = column.substr(0, dotPos);
            string columnName = column.substr(dotPos + 1);
            tableNames.push_back(tableName);
            columnNames.push_back(columnName);
        } else {
            // Если имя колонки не содержит точки, считаем, что это колонка из первой таблицы
            tableNames.push_back(tables.get(0));
            columnNames.push_back(column);
        }
    }

    // Выводим данные с cross join
    size_t maxRows = 0;
    for (size_t i = 0; i < tableNames.length(); ++i) {
        string tableName = tableNames.get(i);
        string columnName = columnNames.get(i);
        for (size_t j = 0; j < hashTables.length(); ++j) {
            if (tables.get(j) == tableName) {
                CustomVector<string> values = hashTables.get(j).get(columnName);
                if (values.length() > maxRows) {
                    maxRows = values.length();
                }
            }
        }
    }

    for (size_t row = 0; row < maxRows; ++row) {
        for (size_t i = 0; i < tableNames.length(); ++i) {
            string tableName = tableNames.get(i);
            string columnName = columnNames.get(i);
            for (size_t j = 0; j < hashTables.length(); ++j) {
                if (tables.get(j) == tableName) {
                    CustomVector<string> values = hashTables.get(j).get(columnName);
                    if (row < values.length()) {
                        cout << values.get(row);
                    } else {
                        cout << ""; // Если строки не хватает, выводим пустую строку
                    }
                    if (i < tableNames.length() - 1) {
                        cout << ",";
                    }
                }
            }
        }
        cout << endl;
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
            CustomVector<string> values;
            parseInsertCommand(command, tableName, values);
            insertIntoTable(schemaName, tableName, values);
        } else if (command.substr(0, 12) == "DELETE FROM ") {
            string tableName, columnName, value;
            parseDeleteCommand(command, tableName, columnName, value);
            deleteFromTable(schemaName, tableName, columnName, value);
        } else if (command.substr(0, 7) == "SELECT ") {
            CustomVector<string> columns;
            CustomVector<string> tables;
            parseSelectCommand(command, columns, tables);
            selectFromTables(schemaName, columns, tables);
        } else {
            cout << "Unknown command" << endl;
        }
    }

    return 0;
}