#include <iostream>
#include "Vector.h"  // Подключаем заголовочный файл для работы с вектором
#include "Set.h"     // Подключаем заголовочный файл для работы с множеством

using namespace std;

// Функция для проверки, можно ли разбить множество на k подмножеств с равной суммой
bool canPartition(Set<int>& elements, Vector<int>& subsetSums, int targetSum, int k, int index) {
    // Если мы прошли все элементы, проверяем, равны ли суммы всех подмножеств целевой сумме
    if (index == elements.getSize()) {
        for (int sum : subsetSums) {
            if (sum != targetSum) {
                return false;
            }
        }
        return true;
    }

    // Получаем текущий элемент
    int element = elements[index];

    // Пробуем добавить текущий элемент в каждое из k подмножеств
    for (int i = 0; i < k; ++i) {
        // Если добавление элемента не превышает целевую сумму, добавляем его
        if (subsetSums[i] + element <= targetSum) {
            subsetSums[i] += element;
            // Рекурсивно проверяем, можно ли разбить оставшиеся элементы
            if (canPartition(elements, subsetSums, targetSum, k, index + 1)) {
                return true;
            }
            // Если не удалось, откатываем изменения
            subsetSums[i] -= element;
        }

        // Если подмножество пустое, не пробуем добавлять в другие пустые подмножества
        if (subsetSums[i] == 0) {
            break;
        }
    }

    // Если не удалось добавить элемент ни в одно из подмножеств, возвращаем false
    return false;
}

// Функция для разбиения множества на k подмножеств с равной суммой
Vector<Set<int>> partitionSet(Set<int>& elements, int k) {
    int totalSum = 0;
    // Вычисляем общую сумму элементов
    for (int i = 0; i < elements.getSize(); i++) {
        totalSum += elements[i];
    }

    if (totalSum % k != 0) {
        cout << "Невозможно разбить множество на " << k << " подмножеств с равной суммой." << endl;
        return {};
    }

    // Вычисляем сумму для каждого подмножества (общая сумма на количество подмножеств)
    int targetSum = totalSum / k;
    // Хранение сумм подмножеств
    Vector<int> subsetSums(k, 0);
    // Хранение подмножеств
    Vector<Set<int>> subsets(k, Set<int>());

    // Проверяем, можно ли разбить множество на k подмножеств с равной суммой
    if (canPartition(elements, subsetSums, targetSum, k, 0)) {
        // Распределяем элементы по подмножествам
        Vector<int> used(elements.getSize(), false);
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < elements.getSize(); ++j) {
                // Если элемент еще не использован и добавление его не превышает целевую сумму, добавляем его
                if (!used[j] && subsetSums[i] - elements[j] >= 0) {
                    subsets[i].add(elements[j]);
                    subsetSums[i] -= elements[j];
                    used[j] = true;
                }
            }
        }
    } else {
        cout << "Невозможно найти подмножества с равной суммой." << endl;
        return {};
    }

    return subsets;
}

int main() {
    Set<int> nums;
    nums.add(4);
    nums.add(10);
    nums.add(5);
    nums.add(1);
    nums.add(3);
    nums.add(7);

    int k = 3; // Количество подмножеств

    Vector<Set<int>> result = partitionSet(nums, k);

    if (!result.isEmpty()) {
        for (int i = 0; i < result.getSize(); ++i) {
            cout << "Подмножество " << i + 1 << ": ";
            for (int j = 0; j < result[i].getSize(); j++) {
                cout << result[i][j] << " ";
            }
            cout << endl;
        }
    }

    return 0;
}