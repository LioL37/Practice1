#include <iostream>
//#include "LinkedLists.h"
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
// Функция для чтения очереди из файла
Queue readQueueFromFile(const string& filename, const string& queueName) {
    ifstream file(filename);
    string line;
    Queue queue;

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
void writeQueueToFile(const string& filename, const string& queueName, const Queue& queue) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(queueName + "=") == 0) {
            buffer << queueName << "=";
            SinglyLinkedList::FLNode* current = queue.list.head;
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
        SinglyLinkedList::FLNode* current = queue.list.head;
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