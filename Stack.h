#include <iostream>
//#include "LinkedLists.h"
using namespace std;

struct Stack {
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
// Функция для чтения стека из файла
Stack readStackFromFile(const string& filename, const string& stackName) {
    ifstream file(filename);
    string line;
    Stack stack;

    while (getline(file, line)) {
        if (line.find(stackName + "=") == 0) {
            string data = line.substr(stackName.length() + 1);
            stringstream ss(data);
            string item;
            while (getline(ss, item, ',')) {
                stack.push(item);
            }
            break;
        }
    }

    file.close();
    return stack;
}

// Функция для записи стека в файл
void writeStackToFile(const string& filename, const string& stackName, const Stack& stack) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(stackName + "=") == 0) {
            buffer << stackName << "=";
            Stack tempStack;
            SinglyLinkedList::FLNode* current = stack.list.head;
            while (current) {
                tempStack.push(current->value);
                current = current->next;
            }
            while (!tempStack.isEmpty()) {
                buffer << tempStack.pop();
                if (!tempStack.isEmpty()) {
                    buffer << ",";
                }
            }
            buffer << endl;
            found = true;
        } else {
            buffer << line << endl;
        }
    }

    if (!found) {
        buffer << stackName << "=";
        Stack tempStack;
        SinglyLinkedList::FLNode* current = stack.list.head;
        while (current) {
            tempStack.push(current->value);
            current = current->next;
        }
        while (!tempStack.isEmpty()) {
            buffer << tempStack.pop();
            if (!tempStack.isEmpty()) {
                buffer << ",";
            }
        }
        buffer << endl;
    }

    file.close();

    ofstream outfile(filename);
    outfile << buffer.str();
    outfile.close();
}