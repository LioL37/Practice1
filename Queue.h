#include <iostream>
#include <stdexcept>  // Для использования std::runtime_error
#include "LinkedLists.h"
using namespace std;

template<typename T>
class Queue {
    public:
    DoublyLinkedList<T> list;

    // Добавление элемента в конец очереди
    void push(const T& item) {
        list.addToTail(item);
    }

    // Удаление элемента из начала очереди
    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        T value = list.head->value;
        list.removeFromHead();
        return value;
    }

    // Чтение элемента из начала очереди
    T peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return list.head->value;
    }

    // Проверка, пуста ли очередь
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