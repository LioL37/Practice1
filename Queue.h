#include <iostream>
//#include "LinkedLists.h"
using namespace std;
template<typename T>
struct Queue {
    DoublyLinkedList<T> list;

    // Добавление элемента в конец очереди
    void push(const T& item) {
        list.addToTail(item);
    }

    // Удаление элемента из начала очереди
    T pop() {
        T value = list.head->value;
        list.removeFromHead();
        return value;
    }

    // Чтение элемента из начала очереди
    T peek() const {
        return list.head->value;
    }

    // Проверка, пуста ли очередь
    bool isEmpty() const {
        return list.head == nullptr;
    }
};

// Функция для чтения очереди из файла
Queue<string> readQueueFromFile(const string& filename, const string& queueName) {
    ifstream file(filename);
    string line;
    Queue<string> queue;

    while (getline(file, line)) {
        if (line.find(queueName + "=") == 0) {
            string data = line.substr(queueName.length() + 1);
            stringstream ss(data);
            string item;
            while (getline(ss, item, ',')) {
                queue.push(item);
            }
            break;
        }
    }

    file.close();
    return queue;
}

// Функция для записи очереди в файл
void writeQueueToFile(const string& filename, const string& queueName, const Queue<string>& queue) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(queueName + "=") == 0) {
            buffer << queueName << "=";
            DoublyLinkedList<string>::DLNode* current = queue.list.head;
            while (current) {
                buffer << current->value;
                if (current->next) {
                    buffer << ",";
                }
                current = current->next;
            }
            buffer << endl;
            found = true;
        } else {
            buffer << line << endl;
        }
    }

    if (!found) {
        buffer << queueName << "=";
        DoublyLinkedList<string>::DLNode* current = queue.list.head;
        while (current) {
            buffer << current->value;
            if (current->next) {
                buffer << ",";
            }
            current = current->next;
        }
        buffer << endl;
    }

    file.close();

    ofstream outfile(filename);
    outfile << buffer.str();
    outfile.close();
}