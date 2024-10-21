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

// Структура полного бинарного дерева
struct CompleteBinaryTree {
    Node* root; // Указатель на корень дерева

    // Конструктор дерева
    CompleteBinaryTree() : root(nullptr) {}

    // Проверка, является ли дерево полным
    bool isComplete(Node* node, int index, int nodeCount) {
        if (node == nullptr)
            return true;

        if (index >= nodeCount)
            return false;

        return (isComplete(node->left, 2 * index + 1, nodeCount) &&
                isComplete(node->right, 2 * index + 2, nodeCount));
    }

    // Проверка, является ли дерево полным
    bool isComplete() {
        int nodeCount = countNodes(root);
        return isComplete(root, 0, nodeCount);
    }

    // Подсчет количества узлов в дереве
    int countNodes(Node* node) {
        if (node == nullptr)
            return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    // Вставка нового узла в дерево
    void insert(int val) {
        if (root == nullptr) {
            root = new Node(val);
            return;
        }

        Queue<Node*> q;
        q.push(root);

        while (!q.isEmpty()) {
            Node* current = q.pop();

            if (current->left == nullptr) {
                current->left = new Node(val);
                break;
            } else {
                q.push(current->left);
            }

            if (current->right == nullptr) {
                current->right = new Node(val);
                break;
            } else {
                q.push(current->right);
            }
        }
    }

    // Рекурсивное удаление узла по значению
    Node* deleteRecursive(Node* node, int val) {
        if (node == nullptr)
            return node;

        if (val < node->data) {
            node->left = deleteRecursive(node->left, val);
        } else if (val > node->data) {
            node->right = deleteRecursive(node->right, val);
        } else {
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Queue<Node*> q;
            q.push(root);
            Node* lastNode = nullptr;
            Node* parent = nullptr;

            while (!q.isEmpty()) {
                Node* current = q.pop();

                if (current->left != nullptr) {
                    q.push(current->left);
                    parent = current;
                    lastNode = current->left;
                }

                if (current->right != nullptr) {
                    q.push(current->right);
                    parent = current;
                    lastNode = current->right;
                }
            }

            node->data = lastNode->data;

            if (parent->right == lastNode) {
                parent->right = deleteRecursive(lastNode, lastNode->data);
            } else {
                parent->left = deleteRecursive(lastNode, lastNode->data);
            }
        }

        return node;
    }

    // Удаление узла по значению
    void remove(int val) {
        root = deleteRecursive(root, val);
    }

    // Поиск узла по значению
    bool search(int val) {
        Node* current = root;
        while (current != nullptr) {
            if (val == current->data)
                return true;
            else if (val < current->data)
                current = current->left;
            else
                current = current->right;
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
};

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