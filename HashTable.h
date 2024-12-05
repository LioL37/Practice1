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
class DynamicArray {
    private:
    KeyValuePair<T>** array;
    int size;
    int capacity;

    public:
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

    // Сериализация в бинарный файл
    void serialize(const string& filename) const {
        ofstream outFile(filename, ios::binary);
        if (!outFile) {
            throw runtime_error("Failed to open file for writing.");
        }

        // Записываем размер таблицы
        int capacity = table.getCapacity();
        outFile.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));

        // Записываем каждую пару ключ-значение
        for (int i = 0; i < capacity; ++i) {
            KeyValuePair<T>* current = table.get(i);
            while (current != nullptr) {
                // Записываем ключ
                size_t keyLength = current->key.size();
                outFile.write(reinterpret_cast<const char*>(&keyLength), sizeof(keyLength));
                outFile.write(current->key.c_str(), keyLength);

                // Записываем значение
                size_t valueLength = current->value.size();
                outFile.write(reinterpret_cast<const char*>(&valueLength), sizeof(valueLength));
                outFile.write(current->value.c_str(), valueLength);

                current = current->next;
            }
        }

        outFile.close();
    }

    // Десериализация из бинарного файла
    void deserialize(const string& filename) {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            throw runtime_error("Failed to open file for reading.");
        }

        // Читаем размер таблицы
        int capacity;
        inFile.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
        table = DynamicArray<T>(capacity);

        // Читаем каждую пару ключ-значение
        while (inFile) {
            size_t keyLength;
            inFile.read(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));
            if (inFile.gcount() == 0) break;
            string key(keyLength, '\0');
            inFile.read(&key[0], keyLength);

            size_t valueLength;
            inFile.read(reinterpret_cast<char*>(&valueLength), sizeof(valueLength));
            string value(valueLength, '\0');
            inFile.read(&value[0], valueLength);

            insert(key, value);
        }

        inFile.close();
    }
};
