#include <iostream>

using namespace std;

struct Array {
    string* data;       // Указатель на строку
    size_t size;        // Текущий размер массива
    size_t capacity;    // Вместимость массива

    // Увеличиваем вместимость массива, если необходимо
    void resize(size_t new_capacity) {
        string* new_data = new string[new_capacity];
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

    // Конструктор по умолчанию
    Array() : data(nullptr), size(0), capacity(0) {}

    // Конструктор с начальной вместимостью
    Array(size_t initial_capacity) : data(new string[initial_capacity]), size(0), capacity(initial_capacity) {}

    // Деструктор
    ~Array() {
        delete[] data;
    }

    // Добавление элемента в конец массива
    void push_back(const string& value) {
        if (size == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;
    }

    // Добавление элемента по индексу
    void insert(size_t index, const string& value) {
        if (index > size) {
            throw out_of_range("Index out of range");
        }
        if (size == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        for (size_t i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        ++size;
    }

    // Получение элемента по индексу
    string get(size_t index) const {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    // Удаление элемента по индексу
    void remove(size_t index) {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        --size;
    }

    // Замена элемента по индексу
    void set(size_t index, const string& value) {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        data[index] = value;
    }

    // Длина массива
    size_t length() const {
        return size;
    }

    // Чтение массива
    void print() const {
        for (size_t i = 0; i < size; ++i) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
};
// Функция для чтения массива из файла
Array readArrayFromFile(const string& filename, const string& arrayName) {
    ifstream file(filename);
    string line;
    Array array;

    while (getline(file, line)) {
        if (line.find(arrayName + "=") == 0) {
            string data = line.substr(arrayName.length() + 1);
            stringstream ss(data);
            string item;
            while (getline(ss, item, ',')) {
                array.push_back(item);
            }
            break;
        }
    }

    file.close();
    return array;
}

// Функция для записи массива в файл
void writeArrayToFile(const string& filename, const string& arrayName, const Array& array) {
    ifstream file(filename);
    stringstream buffer;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find(arrayName + "=") == 0) {
            buffer << arrayName << "=";
            for (size_t i = 0; i < array.length(); ++i) {
                buffer << array.get(i);
                if (i < array.length() - 1) {
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
        buffer << arrayName << "=";
        for (size_t i = 0; i < array.length(); ++i) {
            buffer << array.get(i);
            if (i < array.length() - 1) {
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
