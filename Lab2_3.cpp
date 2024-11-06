#include <iostream>
#include <cmath>
#include "Vector.h"

using namespace std;

// Функция для генерации всех подмножеств вектора
template <typename T>
void generateSubsets(const Vector<T>& vec) {
    int n = vec.getSize();  // Получаем размер вектора
    int totalSubsets = pow(2, n); // Вычисляем общее количество подмножеств (2^n)

    // Перебираем все возможные подмножества
    for (int i = 0; i < totalSubsets; ++i) {
        cout << "{";
        // Перебираем все элементы вектора
        for (int j = 0; j < n; ++j) {
            // Проверяем, включен ли j-й элемент в текущее подмножество
            if (i & (1 << j)) {
                cout << vec[j] << ", ";
            }
        }
        cout << "}" << endl;
    }
}

int main() {
    Vector<string> vec = {"a", "b", "c"};
    generateSubsets(vec);
    return 0;
}