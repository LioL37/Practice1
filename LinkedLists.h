#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

template<typename T>
class SinglyLinkedList {
    public:
    struct FLNode {
        FLNode* next;
        T value;
        FLNode(const T& val) : value(val), next(nullptr) {}
    };
    FLNode* head;
    SinglyLinkedList() : head(nullptr) {}

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

    // Сериализация в бинарный файл
    void serialize(const string& filename) const {
        ofstream outFile(filename, ios::binary);
        if (!outFile) {
            throw runtime_error("Failed to open file for writing.");
        }

        FLNode* current = head;
        while (current) {
            size_t length = current->value.size();
            outFile.write(reinterpret_cast<const char*>(&length), sizeof(length));
            outFile.write(current->value.c_str(), length);
            current = current->next;
        }

        outFile.close();
    }

    // Десериализация из бинарного файла
    void deserialize(const string& filename) {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            throw runtime_error("Failed to open file for reading.");
        }

        while (head) {
            removeFromHead();
        }

        while (inFile) {
            size_t length;
            inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
            if (inFile.gcount() == 0) break;
            string value(length, '\0');
            inFile.read(&value[0], length);
            addToTail(value);
        }

        inFile.close();
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

    // Сериализация в бинарный файл
    void serialize(const string& filename) const {
        ofstream outFile(filename, ios::binary);
        if (!outFile) {
            throw runtime_error("Failed to open file for writing.");
        }

        DLNode* current = head;
        while (current) {
            size_t length = current->value.size();
            outFile.write(reinterpret_cast<const char*>(&length), sizeof(length));
            outFile.write(current->value.c_str(), length);
            current = current->next;
        }

        outFile.close();
    }

    // Десериализация из бинарного файла
    void deserialize(const string& filename) {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            throw runtime_error("Failed to open file for reading.");
        }

        while (head) {
            removeFromHead();
        }

        while (inFile) {
            size_t length;
            inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
            if (inFile.gcount() == 0) break;
            string value(length, '\0');
            inFile.read(&value[0], length);
            addToTail(value);
        }

        inFile.close();
    }
};