#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

template <typename T>
// Структура для хранения пары ключ-значение
struct KeyValuePair {
    T key;
    T value;
    KeyValuePair* next;

    KeyValuePair(const T& k, const T& v) : key(k), value(v), next(nullptr) {}
};

template <typename T>
// Динамический массив для хранения указателей на KeyValuePair
struct DynamicArray {
    KeyValuePair<T>** array;
    int size;
    int capacity;

    DynamicArray(int initialCapacity = 10) : size(0), capacity(initialCapacity) {
        array = new KeyValuePair<T>*[capacity];
        for (int i = 0; i < capacity; ++i) {
            array[i] = nullptr;
        }
    }

    void resize(int newCapacity) {
        KeyValuePair<T>** newArray = new KeyValuePair<T>*[newCapacity];
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

    void add(KeyValuePair<T>* element) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        array[size++] = element;
    }

    KeyValuePair<T>* get(int index) const {
        if (index < 0 || index >= capacity) {
            return nullptr;
        }
        return array[index];
    }

    void set(int index, KeyValuePair<T>* element) {
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

template <typename T>
struct HashTable {
    DynamicArray<T> table; // Динамический массив для хранения списков KeyValuePair

    // Хеш-функция для строк
    int hashFunction(const string& key) const {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % table.getCapacity();
    }
    // Хеш-функция для чисел
    int hashFunction(const int& key) const {
        return key % table.getCapacity();
    }
    HashTable() : table(10) {} // Инициализация с начальной емкостью 10

    // Вставка пары ключ-значение в хеш-таблицу
    void insert(const T& key, const T& value) {
        int index = hashFunction(key);
        KeyValuePair<T>* current = table.get(index);

        // Проверяем, существует ли уже элемент с таким ключом
        while (current != nullptr) {
            if (current->key == key) {
                // Если ключ уже существует, обновляем значение
                current->value = value;
                return;
            }
            current = current->next;
        }

        // Если ключ не найден, добавляем новый элемент
        KeyValuePair<T>* newPair = new KeyValuePair<T>(key, value);
        if (table.get(index) == nullptr) {
            // Если ячейка пуста, просто добавляем новую пару
            table.set(index, newPair);
        } else {
            // Если ячейка занята, добавляем новую пару в начало списка
            newPair->next = table.get(index);
            table.set(index, newPair);
        }
    }

    //// Получение значения по ключу
    //T get(const T& key) const {
    //    int index = hashFunction(key);
    //    KeyValuePair<T>* current = table.get(index);
//
    //    while (current != nullptr) {
    //        if (current->key == key) {
    //            return current->value;
    //        }
    //        current = current->next;
    //    }
//
    //    return "Key not found";
    //}
    // Получение значения по ключу
    T get(const T& key) const {
        int index = hashFunction(key);
        KeyValuePair<T>* current = table.get(index);

        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        return T(); // Возвращаем значение по умолчанию, если ключ не найден
    }

    // Удаление пары ключ-значение по ключу
    void remove(const T& key) {
        int index = hashFunction(key);
        KeyValuePair<T>* current = table.get(index);
        KeyValuePair<T>* prev = nullptr;

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
};
// Функция для чтения хеш-таблицы из файла
HashTable<string> readHashTableFromFile(const string& filename, const string& hashTableName) {
    ifstream file(filename);
    string line;
    HashTable<string> hashTable;

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
void writeHashTableToFile(const string& filename, const string& hashTableName, const HashTable<string>& hashTable) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(hashTableName + "=") == 0) {
            buffer << hashTableName << "=";
            bool firstElement = true;
            for (int i = 0; i < hashTable.table.getCapacity(); ++i) {
                KeyValuePair<string>* current = hashTable.table.get(i);
                while (current) {
                    if (!firstElement) {
                        buffer << ",";
                    }
                    buffer << current->key << ":" << current->value;
                    firstElement = false;
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
        bool firstElement = true;
        for (int i = 0; i < hashTable.table.getCapacity(); ++i) {
            KeyValuePair<string>* current = hashTable.table.get(i);
            while (current) {
                if (!firstElement) {
                    buffer << ",";
                }
                buffer << current->key << ":" << current->value;
                firstElement = false;
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