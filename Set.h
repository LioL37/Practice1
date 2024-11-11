#include <iostream>
#include <stdexcept>
#include "HashTable.h"

using namespace std;

// Класс множества на основе хеш-таблицы
template <typename T>
class Set {
private:
    HashTable<T> table; // Хеш-таблица для хранения элементов
    T** elements; // Массив указателей на элементы
    int size;     // Текущий размер множества
    int capacity; // Вместимость массива указателей

public:
    // Конструктор по умолчанию
    Set() : size(0), capacity(10) {
        elements = new T*[capacity];
    }

    // Конструктор копирования
    Set(const Set& other) : size(other.size), capacity(other.capacity) {
        elements = new T*[capacity];
        for (int i = 0; i < size; ++i) {
            elements[i] = new T(*other.elements[i]);
        }
        table = other.table; // Копируем хеш-таблицу
    }

    // Оператор присваивания
    Set& operator=(const Set& other) {
        if (this != &other) {
            // Освобождаем память, занятую текущими элементами
            for (int i = 0; i < size; ++i) {
                delete elements[i];
            }
            delete[] elements;

            // Копируем данные из другого множества
            size = other.size;
            capacity = other.capacity;
            elements = new T*[capacity];
            for (int i = 0; i < size; ++i) {
                elements[i] = new T(*other.elements[i]);
            }
            table = other.table; // Копируем хеш-таблицу
        }
        return *this;
    }

    // Метод для добавления элемента в множество
    void add(const T& item) {
        // Проверяем, есть ли элемент в множестве
        if (!contains(item)) {
            // Добавляем элемент в хеш-таблицу
            table.insert(item, item);
            // Если массив указателей заполнен, увеличиваем его размер
            if (size == capacity) {
                resize(capacity * 2);
            }
            // Добавляем указатель на элемент в массив
            elements[size++] = new T(item);
        }
    }

    // Метод для проверки наличия элемента в множестве
    bool contains(const T& item) const {
        // Проверяем, есть ли элемент в хеш-таблице
        return (table.get(item) != T());
    }

    // Метод для удаления элемента из множества
    void remove(const T& item) {
        // Удаляем элемент из хеш-таблицы
        table.remove(item);
        // Ищем элемент в массиве указателей и удаляем его
        for (int i = 0; i < size; ++i) {
            if (*elements[i] == item) {
                delete elements[i];
                elements[i] = elements[size - 1];
                --size;
                break;
            }
        }
    }

    // Метод для получения текущего размера множества
    int getSize() const {
        return size;
    }

    // Метод для вывода элементов множества
    void print() const {
        for (int i = 0; i < size; ++i) {
            cout << *elements[i] << " ";
        }
        cout << endl;
    }

    // Оператор обращения по индексу для доступа к элементам по индексу
    T& operator[](int index) const {
        // Проверяем, находится ли индекс в допустимых пределах
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range.");
        }
        return *elements[index];
    }

    // Метод at для доступа к элементам по индексу
    T& at(int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range.");
        }
        return *elements[index];
    }

    // Метод для изменения размера массива указателей
    void resize(int newCapacity) {
        T** newElements = new T*[newCapacity];
        for (int i = 0; i < size; ++i) {
            newElements[i] = elements[i];
        }
        delete[] elements;
        elements = newElements;
        capacity = newCapacity;
    }

    // Метод begin() для получения указателя на начало массива
    T** begin() const {
        return elements;
    }

    // Метод end() для получения указателя на конец массива
    T** end() const {
        return elements + size;
    }
};

template <typename T>
Set<T> readSetFromFile(const string& filename, const string& setName) {
    ifstream file(filename);
    Set<T> set;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(setName + ":") == 0) {
            found = true;
            break;
        }
    }

    if (found) {
        stringstream ss(line.substr(setName.length() + 1));
        T item;
        while (ss >> item) {
            set.add(item);
        }
    }

    file.close();
    return set;
}

template <typename T>
void writeSetToFile(const string& filename, const string& setName, const Set<T>& set) {
    ofstream file(filename, ios::app);
    file << setName << ": ";
    for (int i = 0; i < set.getSize(); ++i) {
        file << set[i] << " ";
    }
    file << endl;
    file.close();
}