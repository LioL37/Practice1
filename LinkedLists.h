#include <iostream>

using namespace std;
struct SinglyLinkedList {
    struct FLNode {
        FLNode* next;
        string value;
        FLNode(const string& val) : value(val), next(nullptr) {}
    };
    FLNode* head;
    SinglyLinkedList() : head(nullptr) {}

    ~SinglyLinkedList() {
        while (head) {
            FLNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Добавление элемента в голову
    void addToHead(const string& value) {
        FLNode* newNode = new FLNode(value);
        newNode->next = head;
        head = newNode;
    }

    // Добавление элемента в хвост (сложность O(n))
    void addToTail(const string& value) {
        FLNode* newNode = new FLNode(value);
        if (!head) {
            head = newNode;
        } else {
            FLNode* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Удаление элемента с головы
    void removeFromHead() {
        if (!head) {
            throw runtime_error("List is empty");
        }
        FLNode* temp = head;
        head = head->next;
        delete temp;
    }

    // Удаление элемента с хвоста (сложность O(n))
    void removeFromTail() {
        if (!head) {
            throw runtime_error("List is empty");
        }
        if (!head->next) {
            delete head;
            head = nullptr;
        } else {
            FLNode* temp = head;
            while (temp->next->next) {
                temp = temp->next;
            }
            delete temp->next;
            temp->next = nullptr;
        }
    }

    // Удаление элемента по значению
    void removeByValue(const string& value) {
        FLNode* current = head;
        FLNode* previous = nullptr;
        while (current) {
            if (current->value == value) {
                if (previous) {
                    previous->next = current->next;
                } else {
                    head = current->next;
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
        FLNode* current = head;
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
        FLNode* current = head;
        while (current) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }
};

struct DoublyLinkedList {
    struct DLNode {
        string value;
        DLNode* prev;
        DLNode* next;
        DLNode(const string& val) : value(val), prev(nullptr), next(nullptr) {}
    };

    DLNode* head;
    DLNode* tail;

    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~DoublyLinkedList() {
        while (head) { // Пока head не nullptr будем удалять узлы
            DLNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Добавление элемента в голову
    void addToHead(const string& value) {
        DLNode* newNode = new DLNode(value);
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
        DLNode* newNode = new DLNode(value);
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
        DLNode* temp = head;
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
        DLNode* temp = tail;
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
        DLNode* current = head;
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
        DLNode* current = head;
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
        DLNode* current = head;
        while (current) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }
};
// Функция для чтения односвязного списка из файла
SinglyLinkedList readSinglyLinkedListFromFile(const string& filename, const string& listName) {
    ifstream file(filename);
    string line;
    SinglyLinkedList list;

    while (getline(file, line)) {
        if (line.find(listName + "=") == 0) {
            string data = line.substr(listName.length() + 1);
            stringstream ss(data);
            string item;
            while (getline(ss, item, ',')) {
                list.addToTail(item);
            }
            break;
        }
    }

    file.close();
    return list;
}

// Функция для записи односвязного списка в файл
void writeSinglyLinkedListToFile(const string& filename, const string& listName, const SinglyLinkedList& list) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(listName + "=") == 0) {
            buffer << listName << "=";
            SinglyLinkedList::FLNode* current = list.head;
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
        buffer << listName << "=";
        SinglyLinkedList::FLNode* current = list.head;
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