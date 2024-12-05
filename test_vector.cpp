#include <gtest/gtest.h>
#include "Vector.h"  // Подключаем ваш класс Vector

// Тест конструктора по умолчанию
TEST(VectorTest, DefaultConstructor) {
    Vector<int> vec;
    EXPECT_EQ(vec.getSize(), 0);
    EXPECT_EQ(vec.getCapacity(), 10);
}

// Тест конструктора со списком инициализации
TEST(VectorTest, InitializerListConstructor) {
    Vector<int> vec = {1, 2, 3};
    EXPECT_EQ(vec.getSize(), 3);
    EXPECT_EQ(vec.getCapacity(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

// Тест конструктора с заданным размером и заполнением указанным значением
TEST(VectorTest, FillConstructor) {
    Vector<int> vec(5, 42);
    EXPECT_EQ(vec.getSize(), 5);
    EXPECT_EQ(vec.getCapacity(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(vec[i], 42);
    }
}

// Тест конструктора копирования
TEST(VectorTest, CopyConstructor) {
    Vector<int> vec1 = {1, 2, 3};
    Vector<int> vec2(vec1);
    EXPECT_EQ(vec2.getSize(), 3);
    EXPECT_EQ(vec2.getCapacity(), 3);
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(vec2[i], vec1[i]);
    }
}

// Тест оператора присваивания
TEST(VectorTest, AssignmentOperator) {
    Vector<int> vec1 = {1, 2, 3};
    Vector<int> vec2;
    vec2 = vec1;
    EXPECT_EQ(vec2.getSize(), 3);
    EXPECT_EQ(vec2.getCapacity(), 3);
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(vec2[i], vec1[i]);
    }
}

// Тест метода push_back
TEST(VectorTest, PushBack) {
    Vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    EXPECT_EQ(vec.getSize(), 3);
    EXPECT_EQ(vec.getCapacity(), 10);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

// Тест метода pop_back
TEST(VectorTest, PopBack) {
    Vector<int> vec = {1, 2, 3};
    vec.pop_back();
    EXPECT_EQ(vec.getSize(), 2);
    EXPECT_EQ(vec.getCapacity(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
}

// Тест оператора доступа по индексу
TEST(VectorTest, IndexOperator) {
    Vector<int> vec = {1, 2, 3};
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

// Тест метода getSize
TEST(VectorTest, GetSize) {
    Vector<int> vec = {1, 2, 3};
    EXPECT_EQ(vec.getSize(), 3);
}

// Тест метода getCapacity
TEST(VectorTest, GetCapacity) {
    Vector<int> vec = {1, 2, 3};
    EXPECT_EQ(vec.getCapacity(), 3);
}

// Тест метода resize
TEST(VectorTest, Resize) {
    Vector<int> vec = {1, 2, 3};
    vec.resize(5);
    EXPECT_EQ(vec.getSize(), 3);
    EXPECT_EQ(vec.getCapacity(), 5);
    vec.resize(2);
    EXPECT_EQ(vec.getSize(), 3);
    EXPECT_EQ(vec.getCapacity(), 2);
}

// Тест метода isEmpty
TEST(VectorTest, IsEmpty) {
    Vector<int> vec;
    EXPECT_TRUE(vec.isEmpty());
    vec.push_back(1);
    EXPECT_FALSE(vec.isEmpty());
}

// Тест метода clear
TEST(VectorTest, Clear) {
    Vector<int> vec = {1, 2, 3};
    vec.clear();
    EXPECT_EQ(vec.getSize(), 0);
    EXPECT_EQ(vec.getCapacity(), 3);
}

// Тест методов begin и end
TEST(VectorTest, BeginEnd) {
    Vector<int> vec = {1, 2, 3};
    EXPECT_EQ(*vec.begin(), 1);
    EXPECT_EQ(*(vec.end() - 1), 3);
}

// Тест метода erase по индексу
TEST(VectorTest, EraseByIndex) {
    Vector<int> vec = {1, 2, 3};
    vec.erase(1);
    EXPECT_EQ(vec.getSize(), 2);
    EXPECT_EQ(vec.getCapacity(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 3);
}

// Тест метода erase по итератору
TEST(VectorTest, EraseByIterator) {
    Vector<int> vec = {1, 2, 3};
    vec.erase(vec.begin() + 1);
    EXPECT_EQ(vec.getSize(), 2);
    EXPECT_EQ(vec.getCapacity(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 3);
}

// Тест метода print
TEST(VectorTest, Print) {
    Vector<int> vec = {1, 2, 3};
    testing::internal::CaptureStdout();
    vec.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 3 \n");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}