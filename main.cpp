#include <iostream>
#include <fstream>
#include <filesystem>
#include "json.hpp"
#include "LinkedLists.h"

using namespace std;
namespace fs = filesystem;
using json = nlohmann::json;

// CustomVector для хранения данных в виде списка узлов
struct CustomVector {
    Node* head;
    Node* tail;

    CustomVector() : head(nullptr), tail(nullptr) {}

    ~CustomVector() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void push_back(const string& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    Node* begin() const {
        return head;
    }

    Node* end() const {
        return nullptr;
    }
    string& at(size_t index) {
        Node* current = head;
        for (size_t i = 0; i < index && current != nullptr; ++i) {
            current = current->next;
        }

        if (current == nullptr) {
            throw out_of_range("Index out of range");
        }

        return current->data;
    }
    void insert(size_t index, const string& value) {
        Node* newNode = new Node(value);
        if (index == 0) {
            newNode->next = head;
            head = newNode;
            if (tail == nullptr) {
                tail = newNode;
            }
            return;
        }

        Node* current = head;
        for (size_t i = 0; i < index - 1 && current != nullptr; ++i) {
            current = current->next;
        }

        if (current == nullptr) {
            delete newNode;
            throw out_of_range("Index out of range");
        }

        newNode->next = current->next;
        current->next = newNode;
        if (newNode->next == nullptr) {
            tail = newNode;
        }
    }

    void remove(size_t index) {
        if (head == nullptr) {
            throw out_of_range("Index out of range");
        }

        if (index == 0) {
            Node* temp = head;
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
            }
            delete temp;
            return;
        }

        Node* current = head;
        for (size_t i = 0; i < index - 1 && current != nullptr; ++i) {
            current = current->next;
        }

        if (current == nullptr || current->next == nullptr) {
            throw out_of_range("Index out of range");
        }

        Node* temp = current->next;
        current->next = temp->next;
        if (current->next == nullptr) {
            tail = current;
        }
        delete temp;
    }
};

void createDirectoryAndFiles(const string& schemaName, const json& structure) {
    // Создаем директорию с названием схемы
    fs::create_directory(schemaName);

    // Проходим по всем таблицам в структуре
    for (const auto& table : structure) {
        string tableName = table["name"];
        CustomVector columns;

        // Заполняем CustomVector названиями колонок
        for (const auto& column : table["columns"]) {
            columns.push_back(column);
        }

        // Создаем поддиректорию для таблицы
        fs::path tablePath = fs::path(schemaName) / tableName;
        fs::create_directory(tablePath);

        // Создаем файл 1.csv в поддиректории таблицы
        fs::path filePath = tablePath / "1.csv";
        ofstream file(filePath);
        if (file.is_open()) {
            // Записываем названия колонок в файл
            for (Node* node = columns.begin(); node != columns.end(); node = node->next) {
                file << node->data;
                if (node->next != nullptr) {
                    file << ",";
                }
            }
            file.close();
            cout << "Created file: " << filePath << endl;
        } else {
            cerr << "Failed to create file: " << filePath << endl;
        }
        
    }
}

int main() {
    // Путь к файлу конфигурации
    string configFilePath = "schema.json";

    // Открываем файл конфигурации
    ifstream configFile(configFilePath);

    // Читаем JSON из файла
    json config;
    configFile >> config;

    // Получаем название схемы и структуру таблиц
    string schemaName = config["name"];
    json structure = config["structure"];

    // Создаем директории и файлы
    createDirectoryAndFiles(schemaName, structure);

    return 0;
}