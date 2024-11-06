#include <iostream>

using namespace std;

// Структура узла бинарного дерева
struct Node {
    int data; // Данные узла
    Node* left; // Указатель на левого потомка
    Node* right; // Указатель на правого потомка

    // Конструктор узла
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Функция для вставки элемента в бинарное дерево поиска
Node* insert(Node* root, int val) {
    // Если дерево пустое, создаем новый узел и возвращаем его
    if (root == nullptr) {
        return new Node(val);
    }

    // Если значение меньше текущего узла, вставляем влево
    if (val < root->data) {
        root->left = insert(root->left, val);
    } 
    // Если значение больше текущего узла, вставляем вправо
    else if (val > root->data) {
        root->right = insert(root->right, val);
    }

    // Возвращаем корень дерева
    return root;
}

// Функция для вычисления высоты дерева
int height(Node* root) {
    // Если дерево пустое, высота равна 0
    if (root == nullptr) {
        return 0;
    }

    // Рекурсивно вычисляем высоту левого и правого поддеревьев
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    // Высота дерева — это максимальная высота поддеревьев плюс 1 (текущий узел)
    return max(leftHeight, rightHeight) + 1;
}

int main() {
    Node* root = nullptr;
    int val;

    cout << "Enter -1 to exit" << endl;
    while (cin >> val) {
        if (val == -1) {
            break;
        }
        root = insert(root, val);
    }
    cout << height(root) << endl;

    return 0;
}