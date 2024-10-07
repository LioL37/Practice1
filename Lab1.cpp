#include <iostream>
#include "json.hpp"
#include <fstream>
#include "Array.h"

using namespace std;
using json = nlohmann::json;

// Функция для загрузки данных из файла
json loadDataFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return json::object();
    }
    json data;
    file >> data;
    file.close();
    return data;
}

// Функция для сохранения данных в файл
void saveDataToFile(const string& filename, const json& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for writing");
    }
    file << data.dump(4);
    file.close();
}

int main() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    json data = loadDataFromFile(filename);

    string command;
    while (true) {
        cout << "Enter command: ";
        cin >> command;

        string name;
        int index;
        string element;

        switch (command[0]) {
            case 'M':
                if (command == "MPUSH") {
                    cin >> name >> index >> element;
                    if (!data.contains(name)) {
                        data[name] = json::array();
                    }
                    if (index < 0 || index > data[name].size()) {
                        cout << "Invalid index" << endl;
                    } else {
                        data[name].insert(data[name].begin() + index, element);
                        saveDataToFile(filename, data);
                    }
                } else if (command == "MDEL") {
                    cin >> name >> index;
                    if (!data.contains(name) || index < 0 || index >= data[name].size()) {
                        cout << "Invalid array name or index" << endl;
                    } else {
                        data[name].erase(data[name].begin() + index);
                        saveDataToFile(filename, data);
                    }
                } else if (command == "MGET") {
                    cin >> name >> index;
                    if (!data.contains(name) || index < 0 || index >= data[name].size()) {
                        cout << "Invalid array name or index" << endl;
                    } else {
                        cout << "Element at index " << index << ": " << data[name][index] << endl;
                    }
                } else if (command == "MSET") {
                    cin >> name >> index >> element;
                    if (!data.contains(name) || index < 0 || index >= data[name].size()) {
                        cout << "Invalid array name or index" << endl;
                    } else {
                        data[name][index] = element;
                        saveDataToFile(filename, data);
                    }
                } else if (command == "MPL") {
                    cin >> name;
                    if (!data.contains(name)) {
                        cout << "Array not found" << endl;
                    } else {
                        cout << "Array length: " << data[name].size() << endl;
                    }
                } else if (command == "MREAD") {
                    cin >> name;
                    if (!data.contains(name)) {
                        cout << "Array not found" << endl;
                    } else {
                        cout << "Array " << name << ": ";
                        for (const auto& element : data[name]) {
                            cout << element << " ";
                        }
                        cout << endl;
                    }
                } else {
                    cout << "Unknown command" << endl;
                }
                break;
            case 'E':
                if (command == "EXIT") {
                    return 0;
                } else {
                    cout << "Unknown command" << endl;
                }
                break;
            default:
                cout << "Unknown command" << endl;
                break;
        }
    }

    return 0;
}