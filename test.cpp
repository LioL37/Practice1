#include <iostream>
#include "Vector.h"

using namespace std;

//bool canPartition(Vector<int>& nums, int k, Vector<int>& subVectorSums, Vector<Vector<int>>& subVectors, int targetSum, int index, unordered_Vector<int>& used) {
//    if (index == nums.getSize()) {
//        for (int i = 0; i < subVectorSums.getSize(); i++) {
//            if (subVectorSums[i] != targetSum) {
//                return false;
//            }
//        }
//        return true;
//    }
//
//    int num = nums[index];
//    for (int i = 0; i < k; ++i) {
//        if (subVectorSums[i] + num <= targetSum && used.find(num) == used.end()) {
//            subVectorSums[i] += num;
//            subVectors[i].add(num);
//            used.insert(num);
//            if (canPartition(nums, k, subVectorSums, subVectors, targetSum, index + 1, used)) {
//                return true;
//            }
//            subVectorSums[i] -= num;
//            subVectors[i].remove(num);
//            used.erase(num);
//        }
//        if (subVectorSums[i] == 0) {
//            break;
//        }
//    }
//    return false;
//}

bool canPartition(Vector<int>& nums, int k, Vector<int>& subVectorSums, Vector<Vector<int>>& subVectors, int targetSum, int index) {
    if (index == nums.getSize()) {
        for (int i = 0; i < subVectorSums.getSize(); i++) {
            if (subVectorSums[i] != targetSum) {
                return false;
            }
        }
        return true;
    }

    int num = nums[index];
    for (int i = 0; i < k; ++i) {
        if (subVectorSums[i] + num <= targetSum) {
            subVectorSums[i] += num;
            subVectors[i].push_back(num);
            if (canPartition(nums, k, subVectorSums, subVectors, targetSum, index + 1)) {
                return true;
            }
            subVectorSums[i] -= num;
            subVectors[i].pop_back();
        }
        if (subVectorSums[i] == 0) {
            break;
        }
    }
    return false;
}

bool partition(Vector<int>& nums, int k, Vector<Vector<int>>& result) {
    int totalSum = 0;
    for (int i = 0; i < nums.getSize(); i++) {
        totalSum += nums[i];
    }

    if (totalSum % k != 0) {
        return false;
    }

    int targetSum = totalSum / k;
    Vector<int> subVectorSums(k, 0);
    Vector<Vector<int>> subVectors(k, Vector<int>());

    bool success = canPartition(nums, k, subVectorSums, subVectors, targetSum, 0);
    if (success) {
        result = subVectors;
    }
    return success;
}

int main() {
    Vector<int> vector = {4, 10, 5, 1, 3, 7};

    int k = 3; // Количество подмножеств

    Vector<Vector<int>> result;

    if (partition(vector, k, result)) {
        cout << "Множество можно разбить на " << k << " подмножества с равной суммой:" << endl;
        for (int i = 0; i < result.getSize(); i++) {
            Vector<int> subVector = result[i];
            for (int j = 0; j < subVector.getSize(); j++) {
                cout << subVector[j] << " ";
            }
            cout << endl;
        }
    } else {
        cout << "Множество нельзя разбить на " << k << " подмножества с равной суммой." << endl;
    }

    return 0;
}