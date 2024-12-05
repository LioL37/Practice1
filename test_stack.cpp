#include <gtest/gtest.h>
#include "Stack.h"  // Подключаем ваш класс Stack

// Тест конструктора по умолчанию
TEST(StackTest, DefaultConstructor) {
    Stack stack;
    EXPECT_TRUE(stack.isEmpty());  // Проверяем, что стек пуст
}

// Тест добавления элемента в стек
TEST(StackTest, Push) {
    Stack stack;
    stack.push("item1");  // Добавляем элемент "item1" в стек
    stack.push("item2");  // Добавляем элемент "item2" в стек
    EXPECT_EQ(stack.peek(), "item2");  // Проверяем, что вершина стека равна "item2"
}

// Тест удаления элемента из стека
TEST(StackTest, Pop) {
    Stack stack;
    stack.push("item1");  // Добавляем элемент "item1" в стек
    stack.push("item2");  // Добавляем элемент "item2" в стек
    EXPECT_EQ(stack.pop(), "item2");  // Проверяем, что удаленный элемент равен "item2"
    EXPECT_EQ(stack.peek(), "item1");  // Проверяем, что теперь вершина стека равна "item1"
}

// Тест чтения элемента с вершины стека
TEST(StackTest, Peek) {
    Stack stack;
    stack.push("item1");  // Добавляем элемент "item1" в стек
    stack.push("item2");  // Добавляем элемент "item2" в стек
    EXPECT_EQ(stack.peek(), "item2");  // Проверяем, что вершина стека равна "item2"
    EXPECT_EQ(stack.peek(), "item2");  // Проверяем, что вершина стека осталась "item2" (peek не удаляет элемент)
}

// Тест проверки, пуст ли стек
TEST(StackTest, IsEmpty) {
    Stack stack;
    EXPECT_TRUE(stack.isEmpty());  // Проверяем, что стек пуст
    stack.push("item1");  // Добавляем элемент "item1" в стек
    EXPECT_FALSE(stack.isEmpty());  // Проверяем, что стек не пуст
    stack.pop();  // Удаляем элемент из стека
    EXPECT_TRUE(stack.isEmpty());  // Проверяем, что стек снова пуст
}

// Тест добавления и удаления нескольких элементов
TEST(StackTest, MultiplePushPop) {
    Stack stack;
    stack.push("item1");  // Добавляем элемент "item1" в стек
    stack.push("item2");  // Добавляем элемент "item2" в стек
    stack.push("item3");  // Добавляем элемент "item3" в стек
    EXPECT_EQ(stack.pop(), "item3");  // Проверяем, что удаленный элемент равен "item3"
    EXPECT_EQ(stack.pop(), "item2");  // Проверяем, что удаленный элемент равен "item2"
    EXPECT_EQ(stack.pop(), "item1");  // Проверяем, что удаленный элемент равен "item1"
    EXPECT_TRUE(stack.isEmpty());  // Проверяем, что стек пуст
}

// Тест добавления элемента в пустой стек
TEST(StackTest, PushToEmptyStack) {
    Stack stack;
    stack.push("item1");  // Добавляем элемент "item1" в стек
    EXPECT_EQ(stack.peek(), "item1");  // Проверяем, что вершина стека равна "item1"
}

// Тест удаления элемента из пустого стека
TEST(StackTest, PopFromEmptyStack) {
    Stack stack;
    EXPECT_THROW(stack.pop(), std::runtime_error);  // Проверяем, что попытка удаления из пустого стека вызывает исключение
}

// Тест чтения элемента с вершины пустого стека
TEST(StackTest, PeekFromEmptyStack) {
    Stack stack;
    EXPECT_THROW(stack.peek(), std::runtime_error);  // Проверяем, что попытка чтения из пустого стека вызывает исключение
}

// Тест добавления и удаления элементов в цикле
TEST(StackTest, PushPopLoop) {
    Stack stack;
    for (int i = 0; i < 10; ++i) {
        stack.push("item" + std::to_string(i));  // Добавляем элементы от "item0" до "item9" в стек
    }
    for (int i = 9; i >= 0; --i) {
        EXPECT_EQ(stack.pop(), "item" + std::to_string(i));  // Проверяем, что удаленные элементы равны "item9", "item8", ..., "item0"
    }
    EXPECT_TRUE(stack.isEmpty());  // Проверяем, что стек пуст
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}