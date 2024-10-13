#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

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

    ~DynamicArray() {
        delete[] array;
    }

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

    // Деструктор для очистки памяти
    ~HashTable() {
        for (int i = 0; i < table.getCapacity(); ++i) {
            KeyValuePair* current = table.get(i);
            while (current != nullptr) {
                KeyValuePair* next = current->next;
                delete current;
                current = next;
            }
        }
    }
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

int main() {
    HashTable table;

    // Добавляем данные в таблицу
    table.insert("Author", "John Doe");
    table.insert("Author", "Jane Smith");
    table.insert("Year", "2020");
    table.insert("Year", "2021");

    // Записываем таблицу в файл
    writeHashTableToCSVFile("data.csv", table);

    // Читаем таблицу из файла
    HashTable readTable = readHashTableFromCSVFile("data.csv");

    // Выводим данные
    CustomVector<string> authors = readTable.get("Author");
    CustomVector<string> years = readTable.get("Year");

    cout << "Authors: ";
    authors.print();

    cout << "Years: ";
    years.print();

    return 0;
}