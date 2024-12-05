#include <iostream>
#include <stdexcept>  // Для использования std::runtime_error
#include "LinkedLists.h"
using namespace std;

class Stack {
    public:
    SinglyLinkedList<string> list;

    // Добавление элемента в стек
    void push(const string& item) {
        list.addToHead(item);
    }

    // Удаление элемента из стека
    string pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        string value = list.head->value;
        list.removeFromHead();
        return value;
    }

    // Чтение элемента с вершины стека
    string peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return list.head->value;
    }

    // Проверка, пуст ли стек
    bool isEmpty() const {
        return list.head == nullptr;
    }

    // Сериализация в бинарный файл
    void serialize(const string& filename) const {
        list.serialize(filename);
    }

    // Десериализация из бинарного файла
    void deserialize(const string& filename) {
        list.deserialize(filename);
    }
};