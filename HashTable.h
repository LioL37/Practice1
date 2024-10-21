#include <iostream>
#include <string>

using namespace std;

// Структура для хранения пары ключ-значение
struct KeyValuePair {
    string key;
    string value;
    KeyValuePair* next;

    KeyValuePair(const string& k, const string& v) : key(k), value(v), next(nullptr) {}
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
                // Если ключ уже существует, обновляем значение
                current->value = value;
                return;
            }
            current = current->next;
        }

        // Если ключ не найден, добавляем новый элемент
        KeyValuePair* newPair = new KeyValuePair(key, value);
        if (table.get(index) == nullptr) {
            // Если ячейка пуста, просто добавляем новую пару
            table.set(index, newPair);
        } else {
            // Если ячейка занята, добавляем новую пару в начало списка
            newPair->next = table.get(index);
            table.set(index, newPair);
        }
    }

    // Получение значения по ключу
    string get(const string& key) const {
        int index = hashFunction(key);
        KeyValuePair* current = table.get(index);

        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        return "Key not found";
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
            bool firstElement = true;
            for (int i = 0; i < hashTable.table.getCapacity(); ++i) {
                KeyValuePair* current = hashTable.table.get(i);
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
            KeyValuePair* current = hashTable.table.get(i);
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