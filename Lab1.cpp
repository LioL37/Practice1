#include <iostream>
#include <sstream>
#include <fstream>
#include "Array.h"
#include "LinkedLists.h"
#include "Queue.h"
#include "Stack.h"
#include "HashTable.h"
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

        if (command == "MPUSH") {
            string arrayName, value;
            cin >> arrayName >> value;

            Array array = readArrayFromFile(filename, arrayName);
            array.push_back(value);
            writeArrayToFile(filename, arrayName, array);
        } else if (command == "MPOP") {
            string arrayName;
            cin >> arrayName;

            Array array = readArrayFromFile(filename, arrayName);
            if (array.length() > 0) {
                array.remove(array.length() - 1);
                writeArrayToFile(filename, arrayName, array);
            } else {
                cout << "Array is empty." << endl;
            }
        } else if (command == "MGET") {
            string arrayName;
            size_t index;
            cin >> arrayName >> index;

            Array array = readArrayFromFile(filename, arrayName);
            if (index < array.length()) {
                cout << array.get(index) << endl;
            } else {
                cout << "Index out of range." << endl;
            }
        } else if (command == "MINS") {
            string arrayName, value;
            size_t index;
            cin >> arrayName >> index >> value;

            Array array = readArrayFromFile(filename, arrayName);
            try {
                array.insert(index, value);
                writeArrayToFile(filename, arrayName, array);
            } catch (const out_of_range& e) {
                cout << e.what() << endl;
            }
        } else if (command == "MREM") {
            string arrayName;
            size_t index;
            cin >> arrayName >> index;

            Array array = readArrayFromFile(filename, arrayName);
            try {
                array.remove(index);
                writeArrayToFile(filename, arrayName, array);
            } catch (const out_of_range& e) {
                cout << e.what() << endl;
            }
        } else if (command == "MSET") {
            string arrayName, value;
            size_t index;
            cin >> arrayName >> index >> value;

            Array array = readArrayFromFile(filename, arrayName);
            try {
                array.set(index, value);
                writeArrayToFile(filename, arrayName, array);
            } catch (const out_of_range& e) {
                cout << e.what() << endl;
            }
        } else if (command == "MPRINT") {
            string arrayName;
            cin >> arrayName;

            Array array = readArrayFromFile(filename, arrayName);
            array.print();
        } else if (command == "LADDH") {
            string listName, value;
            cin >> listName >> value;

            SinglyLinkedList list = readSinglyLinkedListFromFile(filename, listName);
            list.addToHead(value);
            writeSinglyLinkedListToFile(filename, listName, list);
        } else if (command == "LADDT") {
            string listName, value;
            cin >> listName >> value;

            SinglyLinkedList list = readSinglyLinkedListFromFile(filename, listName);
            list.addToTail(value);
            writeSinglyLinkedListToFile(filename, listName, list);
        } else if (command == "LREMH") {
            string listName;
            cin >> listName;

            SinglyLinkedList list = readSinglyLinkedListFromFile(filename, listName);
            try {
                list.removeFromHead();
                writeSinglyLinkedListToFile(filename, listName, list);
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
        } else if (command == "LREMT") {
            string listName;
            cin >> listName;

            SinglyLinkedList list = readSinglyLinkedListFromFile(filename, listName);
            try {
                list.removeFromTail();
                writeSinglyLinkedListToFile(filename, listName, list);
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
        } else if (command == "LREMV") {
            string listName, value;
            cin >> listName >> value;

            SinglyLinkedList list = readSinglyLinkedListFromFile(filename, listName);
            try {
                list.removeByValue(value);
                writeSinglyLinkedListToFile(filename, listName, list);
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
        } else if (command == "LSEARCH") {
            string listName, value;
            cin >> listName >> value;

            SinglyLinkedList list = readSinglyLinkedListFromFile(filename, listName);
            if (list.search(value)) {
                cout << "Value found." << endl;
            } else {
                cout << "Value not found." << endl;
            }
        } else if (command == "LPRINT") {
            string listName;
            cin >> listName;

            SinglyLinkedList list = readSinglyLinkedListFromFile(filename, listName);
            list.print();
        } else if (command == "QPUSH") {
            string queueName, value;
            cin >> queueName >> value;

            Queue queue = readQueueFromFile(filename, queueName);
            queue.push(value);
            writeQueueToFile(filename, queueName, queue);
        } else if (command == "QPOP") {
            string queueName;
            cin >> queueName;

            Queue queue = readQueueFromFile(filename, queueName);
            if (!queue.isEmpty()) {
                string value = queue.pop();
                writeQueueToFile(filename, queueName, queue);
                cout << "Popped value: " << value << endl;
            } else {
                cout << "Queue is empty." << endl;
            }
        } else if (command == "QPEEK") {
            string queueName;
            cin >> queueName;

            Queue queue = readQueueFromFile(filename, queueName);
            if (!queue.isEmpty()) {
                cout << "Peeked value: " << queue.peek() << endl;
            } else {
                cout << "Queue is empty." << endl;
            }
        } else if (command == "QISEMPTY") {
            string queueName;
            cin >> queueName;

            Queue queue = readQueueFromFile(filename, queueName);
            if (queue.isEmpty()) {
                cout << "Queue is empty." << endl;
            } else {
                cout << "Queue is not empty." << endl;
            }
        } else if (command == "QPRINT") {
            string queueName;
            cin >> queueName;

            Queue queue = readQueueFromFile(filename, queueName);
            queue.list.print();
        } else if (command == "SPUSH") {
            string stackName, value;
            cin >> stackName >> value;

            Stack stack = readStackFromFile(filename, stackName);
            stack.push(value);
            writeStackToFile(filename, stackName, stack);
        } else if (command == "SPOP") {
            string stackName;
            cin >> stackName;

            Stack stack = readStackFromFile(filename, stackName);
            if (!stack.isEmpty()) {
                string value = stack.pop();
                writeStackToFile(filename, stackName, stack);
                cout << "Popped value: " << value << endl;
            } else {
                cout << "Stack is empty." << endl;
            }
        } else if (command == "SPEEK") {
            string stackName;
            cin >> stackName;

            Stack stack = readStackFromFile(filename, stackName);
            if (!stack.isEmpty()) {
                cout << "Peeked value: " << stack.peek() << endl;
            } else {
                cout << "Stack is empty." << endl;
            }
        } else if (command == "SPRINT") {
            string stackName;
            cin >> stackName;
            Stack stack = readStackFromFile(filename, stackName);
            stack.list.print();
        } else if (command == "HINSERT") {
            string hashTableName, key, value;
            cin >> hashTableName >> key >> value;

            HashTable hashTable = readHashTableFromFile(filename, hashTableName);
            hashTable.insert(key, value);
            writeHashTableToFile(filename, hashTableName, hashTable);
        } else if (command == "HGET") {
            string hashTableName, key;
            cin >> hashTableName >> key;

            HashTable hashTable = readHashTableFromFile(filename, hashTableName);
            string value = hashTable.get(key);
            cout << "Value: " << value << endl;
        } else if (command == "HREMOVE") {
            string hashTableName, key;
            cin >> hashTableName >> key;

            HashTable hashTable = readHashTableFromFile(filename, hashTableName);
            hashTable.remove(key);
            writeHashTableToFile(filename, hashTableName, hashTable);
        } else if (command == "HPRINT") {
            string hashTableName;
            cin >> hashTableName;

            HashTable hashTable = readHashTableFromFile(filename, hashTableName);
            for (int i = 0; i < hashTable.table.getCapacity(); ++i) {
                KeyValuePair* current = hashTable.table.get(i);
                while (current) {
                    cout << current->key << ":" << current->value << " ";
                    current = current->next;
                }
            }
            cout << endl;
        } else if (command == "TINSERT") {
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