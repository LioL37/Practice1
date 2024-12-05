#include <iostream>
#include "Queue.h"
using namespace std;

// Структура узла бинарного дерева
struct Node {
    int data; // Данные узла
    Node* left; // Указатель на левого потомка
    Node* right; // Указатель на правого потомка

    // Конструктор узла
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Структура полного бинарного дерева
class CompleteBinaryTree {
public:
    Node* root; // Указатель на корень дерева

    // Конструктор дерева
    CompleteBinaryTree() : root(nullptr) {}

    // Проверка, является ли дерево полным
    bool isComplete() {
        if (root == nullptr)
            return true;

        Queue<Node*> q;
        q.push(root);

        bool flag = false;

        while (!q.isEmpty()) {
            Node* current = q.pop();

            if (current->left) {
                if (flag)
                    return false;
                q.push(current->left);
            } else {
                flag = true;
            }

            if (current->right) {
                if (flag)
                    return false;
                q.push(current->right);
            } else {
                flag = true;
            }
        }

        return true;
    }

    // Подсчет количества узлов в дереве
    int countNodes() {
        if (root == nullptr)
            return 0;

        int count = 0;
        Queue<Node*> q;
        q.push(root);

        while (!q.isEmpty()) {
            Node* current = q.pop();
            count++;

            if (current->left)
                q.push(current->left);
            if (current->right)
                q.push(current->right);
        }

        return count;
    }

    // Вставка нового узла в дерево
    void insert(int val) {
        Node* newNode = new Node(val);

        if (root == nullptr) {
            root = newNode;
            return;
        }

        Queue<Node*> q;
        q.push(root);

        while (!q.isEmpty()) {
            Node* current = q.pop();

            if (current->left == nullptr) {
                current->left = newNode;
                break;
            } else {
                q.push(current->left);
            }

            if (current->right == nullptr) {
                current->right = newNode;
                break;
            } else {
                q.push(current->right);
            }
        }
    }

    // Удаление узла по значению
    void remove(int val) {
        if (root == nullptr)
            return;

        Node* nodeToDelete = nullptr;
        Node* lastNode = nullptr;
        Node* parentOfLastNode = nullptr;

        Queue<Node*> q;
        q.push(root);

        while (!q.isEmpty()) {
            lastNode = q.pop();

            if (lastNode->data == val)
                nodeToDelete = lastNode;

            if (lastNode->left)
                q.push(lastNode->left);
            if (lastNode->right)
                q.push(lastNode->right);
        }

        if (nodeToDelete == nullptr)
            return; // Узел не найден

        // Находим родителя последнего узла
        q.push(root);
        while (!q.isEmpty()) {
            Node* current = q.pop();

            if (current->left == lastNode || current->right == lastNode) {
                parentOfLastNode = current;
                break;
            }

            if (current->left)
                q.push(current->left);
            if (current->right)
                q.push(current->right);
        }

        // Заменяем значение удаляемого узла на значение последнего узла
        nodeToDelete->data = lastNode->data;

        // Удаляем последний узел
        if (parentOfLastNode->right == lastNode) {
            parentOfLastNode->right = nullptr;
        } else {
            parentOfLastNode->left = nullptr;
        }

        delete lastNode;
    }

    // Поиск узла по значению
    bool search(int val) {
        if (root == nullptr)
            return false;

        Queue<Node*> q;
        q.push(root);

        while (!q.isEmpty()) {
            Node* current = q.pop();

            if (current->data == val)
                return true;

            if (current->left)
                q.push(current->left);
            if (current->right)
                q.push(current->right);
        }

        return false;
    }

    // Вывод дерева в консоль
    void printTree(Node* node, int space) {
        if (node == nullptr)
            return;

        int COUNT = 5;
        space += COUNT;

        printTree(node->right, space);

        cout << endl;
        for (int i = COUNT; i < space; i++)
            cout << " ";
        cout << node->data << "\n";

        printTree(node->left, space);
    }

    // Вывод дерева в консоль
    void print() {
        printTree(root, 0);
    }

    // Сериализация дерева в бинарный файл
    void serialize(const string& filename) const {
        ofstream outFile(filename, ios::binary);
        if (!outFile) {
            throw runtime_error("Failed to open file for writing.");
        }

        if (root == nullptr) {
            outFile.close();
            return;
        }

        Queue<Node*> q;
        q.push(root);

        while (!q.isEmpty()) {
            Node* current = q.pop();
            outFile.write(reinterpret_cast<const char*>(&current->data), sizeof(current->data));

            if (current->left)
                q.push(current->left);
            if (current->right)
                q.push(current->right);
        }

        outFile.close();
    }

    // Десериализация дерева из бинарного файла
    void deserialize(const string& filename) {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            throw runtime_error("Failed to open file for reading.");
        }

        int value;
        if (!inFile.read(reinterpret_cast<char*>(&value), sizeof(value))) {
            root = nullptr;
            inFile.close();
            return;
        }

        root = new Node(value);
        Queue<Node*> q;
        q.push(root);

        while (inFile.read(reinterpret_cast<char*>(&value), sizeof(value))) {
            Node* current = q.pop();

            current->left = new Node(value);
            q.push(current->left);

            if (inFile.read(reinterpret_cast<char*>(&value), sizeof(value))) {
                current->right = new Node(value);
                q.push(current->right);
            } else {
                break;
            }
        }

        inFile.close();
    }
};