#include <iostream>
#include <algorithm>
#include "Vector.h"

using namespace std;

// Шаблонная структура для хранения пары ключ-значение
template <typename T>
struct KeyValuePair {
    string key; // Ключ (строка)
    T value;    // Значение (тип T)
    KeyValuePair* next; // Указатель на следующий элемент в списке

    // Конструктор для инициализации пары ключ-значение
    KeyValuePair(const string& k, const T& v) : key(k), value(v), next(nullptr) {}
};

// Шаблонная структура для динамического массива указателей на KeyValuePair
template <typename T>
struct DynamicArray {
    KeyValuePair<T>** array; // Массив указателей на KeyValuePair
    int size; // Текущий размер массива
    int capacity; // Вместимость массива

    // Конструктор для инициализации динамического массива
    DynamicArray(int initialCapacity = 10) : size(0), capacity(initialCapacity) {
        array = new KeyValuePair<T>*[capacity];
        for (int i = 0; i < capacity; ++i) {
            array[i] = nullptr;
        }
    }

    // Метод для изменения размера массива
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

    // Метод для добавления элемента в массив
    void add(KeyValuePair<T>* element) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        array[size++] = element;
    }

    // Метод для получения элемента по индексу
    KeyValuePair<T>* get(int index) const {
        if (index < 0 || index >= capacity) {
            return nullptr;
        }
        return array[index];
    }

    // Метод для установки элемента по индексу
    void set(int index, KeyValuePair<T>* element) {
        if (index < 0 || index >= capacity) {
            return;
        }
        array[index] = element;
    }

    // Метод для получения текущего размера массива
    int getSize() const {
        return size;
    }

    // Метод для получения вместимости массива
    int getCapacity() const {
        return capacity;
    }
};

// Шаблонная структура для хеш-таблицы
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

    // Конструктор для инициализации хеш-таблицы с начальной емкостью 10
    HashTable() : table(10) {}

    // Метод для вставки пары ключ-значение в хеш-таблицу
    void insert(const string& key, const T& value) {
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

    // Метод для получения значения по ключу
    T get(const string& key) const {
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

    // Метод для удаления пары ключ-значение по ключу
    void remove(const string& key) {
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

// Структура для представления человека в генеалогическом дереве
struct Person {
    string name; // Имя человека
    int descendants; // Количество потомков
    Vector<Person*> children; // Вектор указателей на детей

    // Конструктор для инициализации человека с именем
    Person(const string& n) : name(n), descendants(0) {}
};

// Функция для получения указателя на человека по имени
Person* getPerson(HashTable<Person*>& tree, const string& name) {
    Person* person = tree.get(name);
    if (person == nullptr) {
        person = new Person(name);
        tree.insert(name, person);
    }
    return person;
}

// Рекурсивная функция для обновления количества потомков у каждого человека
void updateDescendants(Person* person) {
    int count = 0;
    for (Person* child : person->children) {
        updateDescendants(child);
        count += 1 + child->descendants;
    }
    person->descendants = count;
}

int main() {
    int N;
    cin >> N;
    cin.ignore();

    HashTable<Person*> tree; // Хеш-таблица для хранения генеалогического дерева
    Vector<string> names; // Вектор для хранения имен

    // Считываем пары "ребенок-родитель" и строим дерево
    for (int i = 0; i < N - 1; ++i) {
        string child, parent;
        cin >> child >> parent;
        cin.ignore();

        Person* childPerson = getPerson(tree, child);
        Person* parentPerson = getPerson(tree, parent);

        parentPerson->children.push_back(childPerson);

        names.push_back(child);
        names.push_back(parent);
    }

    // Удаляем дубликаты имен
    sort(names.begin(), names.end());
    for (int i = 1; i < names.getSize(); i++) {
        if (names[i] == names[i - 1]) {
            names.erase(names.begin() + i);
            i--; // Уменьшаем индекс, так как вектор сдвигается влево после erase
        }
    }

    // Находим родоначальника (у кого нет родителя)
    string root;
    for (const string& name : names) {
        bool isRoot = true;
        for (int i = 0; i < tree.table.getCapacity(); ++i) {
            KeyValuePair<Person*>* current = tree.table.get(i);
            while (current != nullptr) {
                for (Person* child : current->value->children) {
                    if (child->name == name) {
                        isRoot = false;
                        break;
                    }
                }
                if (!isRoot) break;
                current = current->next;
            }
            if (!isRoot) break;
        }
        if (isRoot) {
            root = name;
            break;
        }
    }

    // Считаем потомков для каждого элемента
    updateDescendants(tree.get(root));

    // Выводим результат
    for (const string& name : names) {
        cout << name << " " << tree.get(name)->descendants << endl;
    }

    return 0;
}