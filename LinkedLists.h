#include <iostream>

using namespace std;

template<typename T>
struct SinglyLinkedList {
    struct FLNode {
        FLNode* next;
        T value;
        FLNode(const T& val) : value(val), next(nullptr) {}
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
    void addToHead(const T& value) {
        FLNode* newNode = new FLNode(value);
        newNode->next = head; // Если список пуст, то новый узел и голова, и хвост
        head = newNode;
    }

    // Добавление элемента в хвост (сложность O(n))
    void addToTail(const T& value) {
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
    void removeByValue(const T& value) {
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
    bool search(const T& value) const {
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

template<typename T>
struct DoublyLinkedList {
    struct DLNode {
        T value;
        DLNode* prev;
        DLNode* next;
        DLNode(const T& val) : value(val), prev(nullptr), next(nullptr) {}
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
    void addToHead(const T& value) {
        DLNode* newNode = new DLNode(value);
        if (!head) {
            head = tail = newNode; // Если список пуст, то новый узел и голова, и хвост
        } else {
            newNode->next = head; // Новый узел указывает next на предыдущую голову
            head->prev = newNode; // Указатель prev предыдущей головы теперь указывает на новый узел
            head = newNode; // Головой становится новый узел
        }
    }

    // Добавление элемента в хвост
    void addToTail(const T& value) {
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
    void removeByValue(const T& value) {
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
    bool search(const T& value) const {
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
SinglyLinkedList<string> readSinglyLinkedListFromFile(const string& filename, const string& listName) {
    ifstream file(filename);
    string line;
    SinglyLinkedList<string> list;

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
void writeSinglyLinkedListToFile(const string& filename, const string& listName, const SinglyLinkedList<string>& list) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(listName + "=") == 0) {
            buffer << listName << "=";
            SinglyLinkedList<string>::FLNode* current = list.head;
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
        SinglyLinkedList<string>::FLNode* current = list.head;
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
// Функция для чтения двусвязного списка из файла
DoublyLinkedList<string> readDoublyLinkedListFromFile(const string& filename, const string& listName) {
    ifstream file(filename);
    string line;
    DoublyLinkedList<string> list;

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

// Функция для записи двусвязного списка в файл
void writeDoublyLinkedListToFile(const string& filename, const string& listName, const DoublyLinkedList<string>& list) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(listName + "=") == 0) {
            buffer << listName << "=";
            DoublyLinkedList<string>::DLNode* current = list.head;
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
        DoublyLinkedList<string>::DLNode* current = list.head;
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