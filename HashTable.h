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

    KeyValuePair* get(int index) {
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

// Хеш-таблица
struct HashTable {
    DynamicArray table; // Динамический массив для хранения списков KeyValuePair

    // Хеш-функция
    int hashFunction(const string& key) {
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
        KeyValuePair* newPair = new KeyValuePair(key, value);

        KeyValuePair* current = table.get(index);
        if (current == nullptr) {
            // Если ячейка пуста, просто добавляем новую пару
            table.set(index, newPair);
        } else {
            // Если ячейка занята, добавляем новую пару в конец списка
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newPair;
        }
    }

    // Получение значения по ключу
    string get(const string& key) {
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