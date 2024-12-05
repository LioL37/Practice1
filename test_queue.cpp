#include <gtest/gtest.h>
#include "Queue.h"  // Подключаем ваш класс Queue

// Тест конструктора по умолчанию
TEST(QueueTest, DefaultConstructor) {
    Queue<int> queue;
    EXPECT_TRUE(queue.isEmpty());  // Проверяем, что очередь пуста
}

// Тест добавления элемента в конец очереди
TEST(QueueTest, Push) {
    Queue<int> queue;
    queue.push(1);  // Добавляем элемент 1 в конец очереди
    queue.push(2);  // Добавляем элемент 2 в конец очереди
    EXPECT_EQ(queue.peek(), 1);  // Проверяем, что первый элемент в очереди равен 1
}

// Тест удаления элемента из начала очереди
TEST(QueueTest, Pop) {
    Queue<int> queue;
    queue.push(1);  // Добавляем элемент 1 в конец очереди
    queue.push(2);  // Добавляем элемент 2 в конец очереди
    EXPECT_EQ(queue.pop(), 1);  // Проверяем, что удаленный элемент равен 1
    EXPECT_EQ(queue.peek(), 2);  // Проверяем, что теперь первый элемент в очереди равен 2
}

// Тест чтения элемента из начала очереди
TEST(QueueTest, Peek) {
    Queue<int> queue;
    queue.push(1);  // Добавляем элемент 1 в конец очереди
    queue.push(2);  // Добавляем элемент 2 в конец очереди
    EXPECT_EQ(queue.peek(), 1);  // Проверяем, что первый элемент в очереди равен 1
    EXPECT_EQ(queue.peek(), 1);  // Проверяем, что первый элемент в очереди остался 1 (peek не удаляет элемент)
}

// Тест проверки, пуста ли очередь
TEST(QueueTest, IsEmpty) {
    Queue<int> queue;
    EXPECT_TRUE(queue.isEmpty());  // Проверяем, что очередь пуста
    queue.push(1);  // Добавляем элемент 1 в конец очереди
    EXPECT_FALSE(queue.isEmpty());  // Проверяем, что очередь не пуста
    queue.pop();  // Удаляем элемент из начала очереди
    EXPECT_TRUE(queue.isEmpty());  // Проверяем, что очередь снова пуста
}

// Тест добавления и удаления нескольких элементов
TEST(QueueTest, MultiplePushPop) {
    Queue<int> queue;
    queue.push(1);  // Добавляем элемент 1 в конец очереди
    queue.push(2);  // Добавляем элемент 2 в конец очереди
    queue.push(3);  // Добавляем элемент 3 в конец очереди
    EXPECT_EQ(queue.pop(), 1);  // Проверяем, что удаленный элемент равен 1
    EXPECT_EQ(queue.pop(), 2);  // Проверяем, что удаленный элемент равен 2
    EXPECT_EQ(queue.pop(), 3);  // Проверяем, что удаленный элемент равен 3
    EXPECT_TRUE(queue.isEmpty());  // Проверяем, что очередь пуста
}

// Тест добавления элемента в пустую очередь
TEST(QueueTest, PushToEmptyQueue) {
    Queue<int> queue;
    queue.push(1);  // Добавляем элемент 1 в конец очереди
    EXPECT_EQ(queue.peek(), 1);  // Проверяем, что первый элемент в очереди равен 1
}

// Тест удаления элемента из пустой очереди
TEST(QueueTest, PopFromEmptyQueue) {
    Queue<int> queue;
    EXPECT_THROW(queue.pop(), runtime_error);  // Проверяем, что попытка удаления из пустой очереди вызывает исключение
}

// Тест чтения элемента из пустой очереди
TEST(QueueTest, PeekFromEmptyQueue) {
    Queue<int> queue;
    EXPECT_THROW(queue.peek(), runtime_error);  // Проверяем, что попытка чтения из пустой очереди вызывает исключение
}

// Тест добавления и удаления элементов в цикле
TEST(QueueTest, PushPopLoop) {
    Queue<int> queue;
    for (int i = 0; i < 10; ++i) {
        queue.push(i);  // Добавляем элементы от 0 до 9 в конец очереди
    }
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(queue.pop(), i);  // Проверяем, что удаленные элементы равны 0, 1, 2, ..., 9
    }
    EXPECT_TRUE(queue.isEmpty());  // Проверяем, что очередь пуста
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}