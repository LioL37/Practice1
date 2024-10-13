#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Array {
    string* data;       // Указатель на строку
    size_t size;        // Текущий размер массива
    size_t capacity;    // Вместимость массива

    // Увеличиваем вместимость массива, если необходимо
    void resize(size_t new_capacity) {
        string* new_data = new string[new_capacity];
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

    // Конструктор по умолчанию
    Array() : data(nullptr), size(0), capacity(0) {}

    // Конструктор с начальной вместимостью
    Array(size_t initial_capacity) : data(new string[initial_capacity]), size(0), capacity(initial_capacity) {}

    // Добавление элемента в конец массива
    void push_back(const string& value) {
        if (size == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;
    }

    // Добавление элемента по индексу
    void insert(size_t index, const string& value) {
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
    string get(size_t index) const {
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
    void set(size_t index, const string& value) {
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
// Функция для чтения массива из файла
Array readArrayFromFile(const string& filename, const string& arrayName) {
    ifstream file(filename);
    string line;
    Array array;

    while (getline(file, line)) {
        if (line.find(arrayName + "=") == 0) {
            string data = line.substr(arrayName.length() + 1);
            stringstream ss(data);
            string item;
            while (getline(ss, item, ',')) {
                array.push_back(item);
            }
            break;
        }
    }

    file.close();
    return array;
}

// Функция для записи массива в файл
void writeArrayToFile(const string& filename, const string& arrayName, const Array& array) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(arrayName + "=") == 0) {
            buffer << arrayName << "=";
            for (size_t i = 0; i < array.length(); ++i) {
                buffer << array.get(i);
                if (i < array.length() - 1) {
                    buffer << ",";
                }
            }
            buffer << endl;
            found = true;
        } else {
            buffer << line << endl;
        }
    }

    if (!found) {
        buffer << arrayName << "=";
        for (size_t i = 0; i < array.length(); ++i) {
            buffer << array.get(i);
            if (i < array.length() - 1) {
                buffer << ",";
            }
        }
        buffer << endl;
    }

    file.close();

    ofstream outfile(filename);
    outfile << buffer.str();
    outfile.close();
}

// Структура для хранения пары ключ-значение
struct KeyValuePair {
    string key;
    Array value;  // Используем Array для хранения значений
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
    Array get(const string& key) const {
        int index = hashFunction(key);
        KeyValuePair* current = table.get(index);

        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        return Array(); // Возвращаем пустой массив, если ключ не найден
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

// Функция для чтения хеш-таблицы из файла
HashTable readHashTableFromFile(const string& filename, const string& hashTableName) {
    ifstream file(filename);
    string line;
    HashTable hashTable;

    while (getline(file, line)) {
        if (line.find(hashTableName + "=") == 0) {
            string data = line.substr(hashTableName.length() + 1);
            stringstream ss(data);
            string item;
            while (getline(ss, item, ',')) {
                size_t pos = item.find(':');
                if (pos != string::npos) {
                    string key = item.substr(0, pos);
                    string value = item.substr(pos + 1);
                    hashTable.insert(key, value);
                }
            }
            break;
        }
    }

    file.close();
    return hashTable;
}

// Функция для записи хеш-таблицы в файл
void writeHashTableToFile(const string& filename, const string& hashTableName, const HashTable& hashTable) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(hashTableName + "=") == 0) {
            buffer << hashTableName << "=";
            for (int i = 0; i < hashTable.table.getCapacity(); ++i) {
                KeyValuePair* current = hashTable.table.get(i);
                while (current) {
                    buffer << current->key << ":";
                    for (size_t j = 0; j < current->value.length(); ++j) {
                        buffer << current->value.get(j);
                        if (j < current->value.length() - 1) {
                            buffer << ",";
                        }
                    }
                    if (current->next) {
                        buffer << ",";
                    }
                    current = current->next;
                }
            }
            buffer << endl;
            found = true;
        } else {
            buffer << line << endl;
        }
    }

    if (!found) {
        buffer << hashTableName << "=";
        for (int i = 0; i < hashTable.table.getCapacity(); ++i) {
            KeyValuePair* current = hashTable.table.get(i);
            while (current) {
                buffer << current->key << ":";
                for (size_t j = 0; j < current->value.length(); ++j) {
                    buffer << current->value.get(j);
                    if (j < current->value.length() - 1) {
                        buffer << ",";
                    }
                }
                if (current->next) {
                    buffer << ",";
                }
                current = current->next;
            }
        }
        buffer << endl;
    }

    file.close();

    ofstream outfile(filename);
    outfile << buffer.str();
    outfile.close();
}

int main() {
    HashTable table;

    // Добавляем данные в таблицу
    table.insert("Author", "John Doe");
    table.insert("Author", "Jane Smith");
    table.insert("Year", "2020");
    table.insert("Year", "2021");

    // Записываем таблицу в файл
    writeHashTableToFile("data.txt", "table", table);

    // Читаем таблицу из файла
    HashTable readTable = readHashTableFromFile("data.txt", "table");

    // Выводим данные
    Array authors = readTable.get("Author");
    Array years = readTable.get("Year");

    cout << "Authors: ";
    authors.print();

    cout << "Years: ";
    years.print();

    return 0;
}