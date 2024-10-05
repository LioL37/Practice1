#include <iostream>

using namespace std;

struct CompleteBinaryTree {
    int* array;  // Массив для хранения элементов дерева
    int size;    // Текущий размер дерева
    int capacity; // Максимальная емкость дерева

    CompleteBinaryTree(int initialCapacity = 10) : size(0), capacity(initialCapacity) {
        array = new int[capacity];
    }

    ~CompleteBinaryTree() {
        delete[] array;
    }

    // Вставка элемента в дерево
    void insert(int value) {
        if (size == capacity) {
            // Если массив заполнен, увеличиваем его вдвое
            resize(capacity * 2);
        }
        array[size++] = value;
    }

    // Получение элемента по индексу
    int get(int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }
        return array[index];
    }

    // Получение родительского элемента
    int getParent(int index) const {
        if (index <= 0 || index >= size) {
            throw out_of_range("Invalid index for parent");
        }
        return array[(index - 1) / 2];
    }

    // Получение левого дочернего элемента
    int getLeftChild(int index) const {
        int leftChildIndex = 2 * index + 1;
        if (leftChildIndex >= size) {
            throw out_of_range("No left child");
        }
        return array[leftChildIndex];
    }

    // Получение правого дочернего элемента
    int getRightChild(int index) const {
        int rightChildIndex = 2 * index + 2;
        if (rightChildIndex >= size) {
            throw out_of_range("No right child");
        }
        return array[rightChildIndex];
    }

    // Получение текущего размера дерева
    int getSize() const {
        return size;
    }

    // Изменение размера массива
    void resize(int newCapacity) {
        int* newArray = new int[newCapacity];
        for (int i = 0; i < size; ++i) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    // Вывод дерева в виде массива
    void print() const {
        for (int i = 0; i < size; ++i) {
            cout << array[i] << " ";
        }
        cout << endl;
    }
};