#include <iostream>
#include "LinkedLists.h"
using namespace std;

struct Queue {
    SinglyLinkedList list;

    // Добавление элемента в конец очереди
    void push(const string& item) {
        list.addToTail(item);
    }

    // Удаление элемента из начала очереди
    string pop() {
        string value = list.head->value;
        list.removeFromHead();
        return value;
    }

    // Чтение элемента из начала очереди
    string peek() const {
        return list.head->value;
    }

    // Проверка, пуста ли очередь
    bool isEmpty() const {
        return list.head == nullptr;
    }
};