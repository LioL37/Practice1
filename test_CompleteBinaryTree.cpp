#include <gtest/gtest.h>
#include "CompleteBinaryTree.h"

// Тест на вставку элементов
TEST(CompleteBinaryTreeTest, InsertTest) {
    CompleteBinaryTree tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);

    EXPECT_EQ(tree.root->data, 1);
    EXPECT_EQ(tree.root->left->data, 2);
    EXPECT_EQ(tree.root->right->data, 3);
    EXPECT_EQ(tree.root->left->left->data, 4);
    EXPECT_EQ(tree.root->left->right->data, 5);
    EXPECT_EQ(tree.root->right->left->data, 6);
}

// Тест на удаление элементов
TEST(CompleteBinaryTreeTest, RemoveTest) {
    CompleteBinaryTree tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);

    tree.remove(2);
    EXPECT_EQ(tree.root->left->data, 6);
    EXPECT_EQ(tree.root->left->left->data, 4);
    EXPECT_EQ(tree.root->left->right->data, 5);

    tree.remove(1);
    EXPECT_EQ(tree.root->data, 5);
    EXPECT_EQ(tree.root->left->data, 6);
    EXPECT_EQ(tree.root->right->data, 3);
}

// Тест на поиск элементов
TEST(CompleteBinaryTreeTest, SearchTest) {
    CompleteBinaryTree tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);

    EXPECT_TRUE(tree.search(1));
    EXPECT_TRUE(tree.search(3));
    EXPECT_TRUE(tree.search(6));
    EXPECT_FALSE(tree.search(7));
}

// Тест на проверку полноты дерева
TEST(CompleteBinaryTreeTest, IsCompleteTest) {
    CompleteBinaryTree tree;

    // Создаем узлы вручную
    Node* root = new Node(1);
    Node* node2 = new Node(2);
    Node* node3 = new Node(3);
    Node* node4 = new Node(4);
    Node* node5 = new Node(5);
    Node* node6 = new Node(6);

    // Связываем узлы вручную
    root->left = node2;
    root->right = node3;
    node2->left = node4;
    node2->right = node5;
    node3->left = node6;

    // Устанавливаем ручное дерево в качестве корня
    tree.root = root;

    EXPECT_TRUE(tree.isComplete());

    // Делаем дерево неполным, удаляя один из узлов
    node2->left = nullptr;

    EXPECT_FALSE(tree.isComplete());
}

// Тест на подсчет узлов
TEST(CompleteBinaryTreeTest, CountNodesTest) {
    CompleteBinaryTree tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);

    EXPECT_EQ(tree.countNodes(), 6);

    tree.remove(6);
    EXPECT_EQ(tree.countNodes(), 5);

    tree.remove(1);
    EXPECT_EQ(tree.countNodes(), 4);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}