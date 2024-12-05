#include <iostream>
#include <fstream>
#include "CompleteBinaryTree.h"
#include "HashTable.h"

using namespace std;


int main() {
    // Пример использования с полным бинарным деревом
    CompleteBinaryTree tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.remove(1);

    // Сериализация дерева
    tree.serialize("tree.bin");

    // Десериализация дерева
    CompleteBinaryTree tree2;
    tree2.deserialize("tree.bin");

    // Вывод дерева в консоль
    tree2.print();

    return 0;
}