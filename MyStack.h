#include <iostream>
#include "LinkedLists.h"
using namespace std;

struct MyStack {
    SinglyLinkedList list;

    // Добавление элемента в стек
    void push(const string& item) {
        list.addToHead(item);
    }

    // Удаление элемента из стека
    string pop() {
        string value = list.head->value;
        list.removeFromHead();
        return value;
    }

    // Чтение элемента с вершины стека
    string peek() const {
        return list.head->value;
    }

    // Проверка, пуст ли стек
    bool isEmpty() const {
        return list.head == nullptr;
    }
};