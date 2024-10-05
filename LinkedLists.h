#include <iostream>

using namespace std;
struct Node {
        string value;
        Node* next;
        Node(const string& val) : value(val), next(nullptr) {}
    };
struct SinglyLinkedList {   
    Node* head;
    Node* tail;
    SinglyLinkedList() : head(nullptr), tail(nullptr) {}

    ~SinglyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Добавление элемента в голову
    void addToHead(const string& value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
    }

    // Добавление элемента в хвост
    void addToTail(const string& value) {
        Node* newNode = new Node(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Удаление элемента с головы
    void removeFromHead() {
        if (!head) {
            throw runtime_error("List is empty");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        if (!head) {
            tail = nullptr;
        }
    }

    // Удаление элемента с хвоста
    void removeFromTail() {
        if (!tail) {
            throw runtime_error("List is empty");
        }
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node* temp = head;
            while (temp->next != tail) {
                temp = temp->next;
            }
            delete tail;
            tail = temp;
            tail->next = nullptr;
        }
    }

    // Удаление элемента по значению
    void removeByValue(const string& value) {
        Node* current = head;
        Node* previous = nullptr;
        while (current) {
            if (current->value == value) {
                if (previous) {
                    previous->next = current->next;
                } else {
                    head = current->next;
                }
                if (current == tail) {
                    tail = previous;
                }
                delete current;
                return;
            }
            previous = current;
            current = current->next;
        }
        throw runtime_error("Value not found");
    }

    // Поиск элемента по значению
    bool search(const string& value) const {
        Node* current = head;
        while (current) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Чтение списка
    void print() const {
        Node* current = head;
        while (current) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }
};

struct DoublyLinkedList {
    struct Node {
        string value;
        Node* prev;
        Node* next;
        Node(const string& val) : value(val), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;

    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~DoublyLinkedList() {
        while (head) { // Пока head не nullptr будем удалять узлы
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Добавление элемента в голову
    void addToHead(const string& value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    // Добавление элемента в хвост
    void addToTail(const string& value) {
        Node* newNode = new Node(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Удаление элемента с головы
    void removeFromHead() {
        if (!head) {
            throw runtime_error("List is empty");
        }
        Node* temp = head;
        head = head->next;
        if (head) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
    }

    // Удаление элемента с хвоста
    void removeFromTail() {
        if (!tail) {
            throw runtime_error("List is empty");
        }
        Node* temp = tail;
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete temp;
    }

    // Удаление элемента по значению
    void removeByValue(const string& value) {
        Node* current = head;
        while (current) {
            if (current->value == value) {
                if (current->prev) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;
                }
                if (current->next) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }
                delete current;
                return;
            }
            current = current->next;
        }
        throw runtime_error("Value not found");
    }

    // Поиск элемента по значению
    bool search(const string& value) const {
        Node* current = head;
        while (current) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Чтение списка
    void print() const {
        Node* current = head;
        while (current) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }
};