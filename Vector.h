#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class Vector {
private:
    T* array;       // Указатель на массив
    int size;       // Текущий размер массива
    int capacity;   // Вместимость массива

public:
    // Конструктор по умолчанию
    Vector(int initialCapacity = 10) : size(0), capacity(initialCapacity) {
        if (initialCapacity <= 0) {
            throw invalid_argument("Capacity must be > 0.");
        }
        array = new T[capacity];
    }

    // Конструктор со списком инициализации
    Vector(initializer_list<T> init) : size(init.size()), capacity(init.size()) {
        array = new T[capacity];
        int index = 0;
        for (const T& element : init) {
            array[index++] = element;
        }
    }

    // Конструктор с заданным размером и заполнением указанным значением
    Vector(int initialSize, const T& fillValue) : size(initialSize), capacity(initialSize) {
        if (initialSize <= 0) {
            throw invalid_argument("Size must be > 0.");
        }
        array = new T[capacity];
        for (int i = 0; i < size; ++i) {
            array[i] = fillValue; // Заполняем указанным значением
        }
    }

    // Конструктор копирования
    Vector(const Vector& other) : size(other.size), capacity(other.capacity) {
        array = new T[capacity];
        for (int i = 0; i < size; ++i) {
            array[i] = other.array[i];
        }
    }

    // Оператор присваивания
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] array;
            size = other.size;
            capacity = other.capacity;
            array = new T[capacity];
            for (int i = 0; i < size; ++i) {
                array[i] = other.array[i];
            }
        }
        return *this;
    }

    // Добавление элемента в конец массива
    void push_back(const T& element) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        array[size++] = element;
    }

    // Удаление последнего элемента массива
    void pop_back() {
        if (size > 0) {
            --size;
        }
    }

    // Доступ к элементу по индексу
    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range.");
        }
        return array[index];
    }

    // Доступ к элементу по индексу (константная версия)
    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range.");
        }
        return array[index];
    }
    
    // Получение текущего размера массива
    int getSize() const {
        return size;
    }

    // Получение текущей вместимости массива
    int getCapacity() const {
        return capacity;
    }

    // Изменение размера массива
    void resize(int newCapacity) {
        if (newCapacity <= 0) {
            throw invalid_argument("New capacity must be greater than zero.");
        }
        T* newArray = new T[newCapacity];
        for (int i = 0; i < size; ++i) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    // Проверка на пустоту массива
    bool isEmpty() const {
        return size == 0;
    }

    // Очистка массива
    void clear() {
        size = 0;
    }

    T* begin() { 
        return array;
    }

    T* end() { 
        return array + size; 
    }

    // Метод erase для удаления элемента по индексу
    void erase(int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range.");
        }
        for (int i = index; i < size - 1; ++i) {
            array[i] = array[i + 1];
        }
        --size;
    }

    // Метод erase для удаления элемента по итератору
    T* erase(T* position) {
        if (position < begin() || position >= end()) {
            throw out_of_range("Iterator out of range.");
        }
        int index = position - begin();
        for (int i = index; i < size - 1; ++i) {
            array[i] = array[i + 1];
        }
        --size;
        return position;
    }
    
    // Вывод всех элементов массива
    void print() const {
        for (int i = 0; i < size; ++i) {
            cout << array[i] << " ";
        }
        cout << endl;
    }
};