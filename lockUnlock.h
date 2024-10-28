#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

// Функция для блокировки таблицы
bool lockTable(const fs::path& tablePath, const string& tableName) {
    fs::path lockPath = tablePath / (tableName + "_lock");
    ifstream lockFile(lockPath);
    string lockStatus;
    lockFile >> lockStatus;
    lockFile.close();

    if (lockStatus == "locked") {
        return false; // Таблица уже заблокирована
    }

    ofstream lockFileOut(lockPath);
    if (lockFileOut.is_open()) {
        lockFileOut << "locked" << endl;
        lockFileOut.close();
        return true; // Таблица успешно заблокирована
    } else {
        cerr << "Failed to lock table: " << tableName << endl;
        return false;
    }
}
// Функция для разблокировки таблицы
void unlockTable(const fs::path& tablePath, const string& tableName) {
    fs::path lockPath = tablePath / (tableName + "_lock");
    ofstream lockFileOut(lockPath);
    if (lockFileOut.is_open()) {
        lockFileOut << "unlocked" << endl;
        lockFileOut.close();
    } else {
        cerr << "Failed to unlock table: " << tableName << endl;
    }
}
// Функция для чтения таблицы из файла и разбиения строк на элементы
SinglyLinkedList<SinglyLinkedList<string>> readRowsFromFile(const string& filename, SinglyLinkedList<string>& headers) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        throw runtime_error("File open error");
    }

    string line;
    SinglyLinkedList<SinglyLinkedList<string>> table;

    // Читаем первую строку как заголовки
    if (getline(file, line)) {
        stringstream ss(line);
        string item;
        while (getline(ss, item, ',')) {
            headers.addToTail(item);
        }
    }

    // Читаем файл построчно
    while (getline(file, line)) {
        SinglyLinkedList<string> row;
        stringstream ss(line);
        string item;

        // Разбиваем строку на элементы по запятым
        while (getline(ss, item, ',')) {
            row.addToTail(item);
        }

        // Добавляем строку в таблицу
        table.addToTail(row);
    }

    file.close();
    return table;
}