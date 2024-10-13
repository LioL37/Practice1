#include <iostream>
#include <fstream>
#include <sstream>
#include "CompleteBinaryTree.h"
using namespace std;

// Основной цикл программы
int main() {
    string filename;

    cout << "Enter filename: ";
    cin >> filename;

    while (true) {
        string command;
        cout << "Enter command: ";
        cin >> command;

        if (command == "TINSERT") {
            string treeName;
            int value;
            cin >> treeName >> value;

            CompleteBinaryTree tree = readTreeFromFile(filename, treeName);
            tree.insert(value);
            writeTreeToFile(filename, treeName, tree);
        } else if (command == "TSEARCH") {
            string treeName;
            int value;
            cin >> treeName >> value;

            CompleteBinaryTree tree = readTreeFromFile(filename, treeName);
            cout << "Index: " << tree.search(value) << endl;
        } else if (command == "TREMOVE") {
            string treeName;
            int value;
            cin >> treeName >> value;

            CompleteBinaryTree tree = readTreeFromFile(filename, treeName);
            try {
                tree.remove(value);
                writeTreeToFile(filename, treeName, tree);
            } catch (const out_of_range& e) {
                cout << e.what() << endl;
            }
        } else if (command == "TGET") {
            string treeName;
            int index;
            cin >> treeName >> index;

            CompleteBinaryTree tree = readTreeFromFile(filename, treeName);
            try {
                int value = tree.get(index);
                cout << "Value at index " << index << ": " << value << endl;
            } catch (const out_of_range& e) {
                cout << e.what() << endl;
            }
        } else if (command == "TPRINT") {
            string treeName;
            cin >> treeName;

            CompleteBinaryTree tree = readTreeFromFile(filename, treeName);
            tree.print();
        } else if (command == "EXIT") {
            break;
        } else {
            cout << "Unknown command." << endl;
        }
    }

    return 0;
}