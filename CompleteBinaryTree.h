#include <iostream>

using namespace std;

struct CompleteBinaryTree {
    int* array;  // Массив для хранения элементов дерева
    int size;    // Текущий размер дерева
    int capacity; // Максимальная емкость дерева

    CompleteBinaryTree(int initialCapacity = 10) : size(0), capacity(initialCapacity) {
        array = new int[capacity];
    }

    ~CompleteBinaryTree() {
        delete[] array;
    }

    // Вставка элемента в дерево
    void insert(int value) {
        if (size == capacity) {
            // Если массив заполнен, увеличиваем его вдвое
            resize(capacity * 2);
        }
        array[size++] = value;
    }

    // Поиск элемента в дереве
    int search(int value) const {
        for (int i = 0; i < size; ++i) {
            if (array[i] == value) {
                return i;
            }
        }
        return -1;
    }

    // Удаление элемента с сохранением структуры дерева
    void remove(int value) {
        int index = -1;
        for (int i = 0; i < size; ++i) {
            if (array[i] == value) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            throw out_of_range("Element not found");
        }

        // Заменяем удаляемый элемент последним элементом
        array[index] = array[size - 1];
        size--;
    }

    // Получение элемента по индексу
    int get(int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }
        return array[index];
    }
    // Изменение размера массива
    void resize(int newCapacity) {
        int* newArray = new int[newCapacity];
        for (int i = 0; i < size; ++i) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    // Вывод дерева в виде массива
    void print() const {
        for (int i = 0; i < size; ++i) {
            cout << array[i] << " ";
        }
        cout << endl;
    }

    // Сериализация дерева в строку
    string serialize() const {
        string result;
        for (int i = 0; i < size; ++i) {
            result += to_string(array[i]);
            if (i < size - 1) {
                result += "-";
            }
        }
        return result;
    }

    // Десериализация строки в дерево
    void deserialize(const string& data) {
        stringstream ss(data);
        string item;
        while (getline(ss, item, '-')) {
            try {
                insert(stoi(item));
            } catch (const invalid_argument& e) {
                cerr << "Invalid argument: " << e.what() << endl;
            }
        }
    }
};

// Функция для чтения дерева из файла
CompleteBinaryTree readTreeFromFile(const string& filename, const string& treeName) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        if (line.find(treeName + "=") == 0) {
            string data = line.substr(treeName.length() + 1);
            file.close();
            CompleteBinaryTree tree;
            tree.deserialize(data);
            return tree;
        }
    }

    file.close();
    return CompleteBinaryTree();
}

// Функция для записи дерева в файл
void writeTreeToFile(const string& filename, const string& treeName, const CompleteBinaryTree& tree) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(treeName + "=") == 0) {
            buffer << treeName << "=" << tree.serialize() << endl;
            found = true;
        } else {
            buffer << line << endl;
        }
    }

    if (!found) {
        buffer << treeName << "=" << tree.serialize() << endl;
    }

    file.close();

    ofstream outfile(filename);
    outfile << buffer.str();
    outfile.close();
}
