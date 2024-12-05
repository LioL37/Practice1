#include <iostream>
#include <sstream>
#include <fstream>
#include "Array.h"
#include "LinkedLists.h"
#include "Queue.h"
#include "Stack.h"
#include "Set.h"
#include "CompleteBinaryTree.h"

using namespace std;
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
// Функция для чтения очереди из файла
Queue<string> readQueueFromFile(const string& filename, const string& queueName) {
    ifstream file(filename);
    string line;
    Queue<string> queue;

    while (getline(file, line)) {
        if (line.find(queueName + "=") == 0) {
            string data = line.substr(queueName.length() + 1);
            stringstream ss(data);
            string item;
            while (getline(ss, item, ',')) {
                queue.push(item);
            }
            break;
        }
    }

    file.close();
    return queue;
}

// Функция для записи очереди в файл
void writeQueueToFile(const string& filename, const string& queueName, const Queue<string>& queue) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(queueName + "=") == 0) {
            buffer << queueName << "=";
            DoublyLinkedList<string>::DLNode* current = queue.list.head;
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
        buffer << queueName << "=";
        DoublyLinkedList<string>::DLNode* current = queue.list.head;
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
            SinglyLinkedList<string>::FLNode* current = stack.list.head;
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
        SinglyLinkedList<string>::FLNode* current = stack.list.head;
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
// Функция для чтения хеш-таблицы из файла
HashTable<string> readHashTableFromFile(const string& filename, const string& hashTableName) {
    ifstream file(filename);
    string line;
    HashTable<string> hashTable;

    while (getline(file, line)) {
        if (line.find(hashTableName + "=") == 0) {
            string data = line.substr(hashTableName.length() + 1);
            stringstream ss(data);
            string item;
            while (getline(ss, item, ',')) {
                size_t pos = item.find(':');
                if (pos != string::npos) {
                    string key = item.substr(0, pos);
                    string value = item.substr(pos + 1);
                    hashTable.insert(key, value);
                }
            }
            break;
        }
    }

    file.close();
    return hashTable;
}

// Функция для записи хеш-таблицы в файл
void writeHashTableToFile(const string& filename, const string& hashTableName, const HashTable<string>& hashTable) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(hashTableName + "=") == 0) {
            buffer << hashTableName << "=";
            bool firstElement = true;
            for (int i = 0; i < hashTable.table.getCapacity(); ++i) {
                KeyValuePair<string>* current = hashTable.table.get(i);
                while (current) {
                    if (!firstElement) {
                        buffer << ",";
                    }
                    buffer << current->key << ":" << current->value;
                    firstElement = false;
                    current = current->next;
                }
            }
            buffer << endl;
            found = true;
        } else {
            buffer << line << endl;
        }
    }

    if (!found) {
        buffer << hashTableName << "=";
        bool firstElement = true;
        for (int i = 0; i < hashTable.table.getCapacity(); ++i) {
            KeyValuePair<string>* current = hashTable.table.get(i);
            while (current) {
                if (!firstElement) {
                    buffer << ",";
                }
                buffer << current->key << ":" << current->value;
                firstElement = false;
                current = current->next;
            }
        }
        buffer << endl;
    }

    file.close();

    ofstream outfile(filename);
    outfile << buffer.str();
    outfile.close();
}
// Функция для сохранения дерева в файл
void writeTreeToFile(const string& filename, const string& treeName, CompleteBinaryTree& tree) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    // Читаем файл построчно
    while (getline(file, line)) {
        // Если строка начинается с treeName, обновляем её
        if (line.find(treeName + "=") == 0) {
            buffer << treeName << "=";
            Queue<Node*> q;
            q.push(tree.root);
            while (!q.isEmpty()) {
                Node* current = q.pop();
                buffer << current->data;
                if (current->left != nullptr) {
                    q.push(current->left);
                }
                if (current->right != nullptr) {
                    q.push(current->right);
                }
                if (!q.isEmpty()) {
                    buffer << "-";
                }
            }
            buffer << endl;
            found = true;
        } else {
            buffer << line << endl;
        }
    }

    // Если дерево с таким именем не найдено, добавляем его в конец файла
    if (!found) {
        buffer << treeName << "=";
        Queue<Node*> q;
        q.push(tree.root);
        while (!q.isEmpty()) {
            Node* current = q.pop();
            buffer << current->data;
            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
            if (!q.isEmpty()) {
                buffer << "-";
            }
        }
        buffer << endl;
    }

    file.close();

    // Записываем обновленные данные в файл
    ofstream outfile(filename);
    outfile << buffer.str();
    outfile.close();
}

// Функция для загрузки дерева из файла
CompleteBinaryTree readTreeFromFile(const string& filename, const string& treeName) {
    CompleteBinaryTree tree;
    ifstream file(filename);
    string line;

    // Читаем файл построчно
    while (getline(file, line)) {
        // Если строка начинается с treeName, парсим её
        if (line.find(treeName + "=") == 0) {
            string data = line.substr(treeName.length() + 1);
            stringstream ss(data);
            string item;
            while (getline(ss, item, '-')) {
                tree.insert(stoi(item));
            }
            break;
        }
    }

    file.close();
    return tree;
}
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
        } else if (command == "DLADDH") {
            string listName, value;
            cin >> listName >> value;

            DoublyLinkedList<string> list = readDoublyLinkedListFromFile(filename, listName);
            list.addToHead(value);
            writeDoublyLinkedListToFile(filename, listName, list);
        } else if (command == "DLADDT") {
            string listName, value;
            cin >> listName >> value;

            DoublyLinkedList<string> list = readDoublyLinkedListFromFile(filename, listName);
            list.addToTail(value);
            writeDoublyLinkedListToFile(filename, listName, list);
        } else if (command == "DLREMH") {
            string listName;
            cin >> listName;

            DoublyLinkedList<string> list = readDoublyLinkedListFromFile(filename, listName);
            try {
                list.removeFromHead();
                writeDoublyLinkedListToFile(filename, listName, list);
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
        } else if (command == "DLREMT") {
            string listName;
            cin >> listName;

            DoublyLinkedList<string> list = readDoublyLinkedListFromFile(filename, listName);
            try {
                list.removeFromTail();
                writeDoublyLinkedListToFile(filename, listName, list);
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
        } else if (command == "DLREMV") {
            string listName, value;
            cin >> listName >> value;

            DoublyLinkedList<string> list = readDoublyLinkedListFromFile(filename, listName);
            try {
                list.removeByValue(value);
                writeDoublyLinkedListToFile(filename, listName, list);
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
        } else if (command == "DLSEARCH") {
            string listName, value;
            cin >> listName >> value;

            DoublyLinkedList<string> list = readDoublyLinkedListFromFile(filename, listName);
            if (list.search(value)) {
                cout << "Value found." << endl;
            } else {
                cout << "Value not found." << endl;
            }
        } else if (command == "DLPRINT") {
            string listName;
            cin >> listName;

            DoublyLinkedList<string> list = readDoublyLinkedListFromFile(filename, listName);
            list.print();
        }  else if (command == "QPUSH") {
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

            HashTable<string> hashTable = readHashTableFromFile(filename, hashTableName);
            hashTable.insert(key, value);
            writeHashTableToFile(filename, hashTableName, hashTable);
        } else if (command == "HGET") {
            string hashTableName, key;
            cin >> hashTableName >> key;

            HashTable<string> hashTable = readHashTableFromFile(filename, hashTableName);
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

            HashTable<string> hashTable = readHashTableFromFile(filename, hashTableName);
            for (int i = 0; i < hashTable.table.getCapacity(); ++i) {
                KeyValuePair<string>* current = hashTable.table.get(i);
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
        } else if (command == "TPRINT") {
            string treeName;
            cin >> treeName;

            CompleteBinaryTree tree = readTreeFromFile(filename, treeName);
            tree.print();
        }   else if (command == "STADD") {
            string setName, value;
            cin >> setName >> value;

            Set<string> set = readSetFromFile<string>(filename, setName);
            set.add(value);
            writeSetToFile<string>(filename, setName, set);
        } else if (command == "STREM") {
            string setName, value;
            cin >> setName >> value;

            Set<string> set = readSetFromFile<string>(filename, setName);
            set.remove(value);
            writeSetToFile<string>(filename, setName, set);
        } else if (command == "STSEARCH") {
            string setName, value;
            cin >> setName >> value;

            Set<string> set = readSetFromFile<string>(filename, setName);
            if (set.contains(value)) {
                cout << "Value found." << endl;
            } else {
                cout << "Value not found." << endl;
            }
        } else if (command == "STPRINT") {
            string setName;
            cin >> setName;

            Set<string> set = readSetFromFile<string>(filename, setName);
            set.print();
        } else if (command == "EXIT") {
            break;
        } else {
            cout << "Unknown command." << endl;
        }
    }
    return 0;
}