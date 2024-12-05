#include <gtest/gtest.h>
#include "HashTable.h"  // Подключаем ваш хеш-таблицу

// Тест на вставку и получение элемента
TEST(HashTableTest, InsertAndGet) {
    HashTable<std::string> ht;
    ht.insert("key1", "value1");
    ht.insert("key2", "value2");

    EXPECT_EQ(ht.get("key1"), "value1");
    EXPECT_EQ(ht.get("key2"), "value2");
    EXPECT_EQ(ht.get("key3"), "");  // Ключ не существует
}

// Тест на обновление значения по существующему ключу
TEST(HashTableTest, UpdateValue) {
    HashTable<std::string> ht;
    ht.insert("key1", "value1");
    ht.insert("key1", "newValue1");

    EXPECT_EQ(ht.get("key1"), "newValue1");
}

// Тест на удаление элемента
TEST(HashTableTest, RemoveElement) {
    HashTable<std::string> ht;
    ht.insert("key1", "value1");
    ht.insert("key2", "value2");

    ht.remove("key1");
    EXPECT_EQ(ht.get("key1"), "");
    EXPECT_EQ(ht.get("key2"), "value2");
}

// Тест на обработку коллизий
TEST(HashTableTest, CollisionHandling) {
    HashTable<std::string> ht;
    // Вставляем элементы, которые должны попасть в одну и ту же ячейку
    ht.insert("key1", "value1");
    ht.insert("key11", "value11");  // key1 и key11 должны попасть в одну ячейку

    EXPECT_EQ(ht.get("key1"), "value1");
    EXPECT_EQ(ht.get("key11"), "value11");

    // Удаляем один из элементов
    ht.remove("key1");
    EXPECT_EQ(ht.get("key1"), "");
    EXPECT_EQ(ht.get("key11"), "value11");
}

// Тест на работу с числами
TEST(HashTableTest, NumericKeys) {
    HashTable<int> ht;
    ht.insert(1, 100);
    ht.insert(11, 200);  // 1 и 11 должны попасть в одну ячейку

    EXPECT_EQ(ht.get(1), 100);
    EXPECT_EQ(ht.get(11), 200);

    ht.remove(1);
    EXPECT_EQ(ht.get(1), 0);
    EXPECT_EQ(ht.get(11), 200);
}

// Тест на автоматическое увеличение размера таблицы
TEST(HashTableTest, AutoResize) {
    HashTable<std::string> ht;
    for (int i = 0; i < 20; ++i) {
        ht.insert("key" + std::to_string(i), "value" + std::to_string(i));
    }

    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(ht.get("key" + std::to_string(i)), "value" + std::to_string(i));
    }
}

// Тест на получение значения по несуществующему ключу
TEST(HashTableTest, GetNonExistentKey) {
    HashTable<std::string> ht;
    EXPECT_EQ(ht.get("nonExistentKey"), "");
}

// Тест на удаление несуществующего ключа
TEST(HashTableTest, RemoveNonExistentKey) {
    HashTable<std::string> ht;
    ht.remove("nonExistentKey");  // Не должно вызывать ошибок
}

// Тест на вставку и удаление большого количества элементов
TEST(HashTableTest, LargeInsertAndRemove) {
    HashTable<std::string> ht;
    const int numElements = 1000;

    for (int i = 0; i < numElements; ++i) {
        ht.insert("key" + std::to_string(i), "value" + std::to_string(i));
    }

    for (int i = 0; i < numElements; ++i) {
        EXPECT_EQ(ht.get("key" + std::to_string(i)), "value" + std::to_string(i));
    }

    for (int i = 0; i < numElements; ++i) {
        ht.remove("key" + std::to_string(i));
    }

    for (int i = 0; i < numElements; ++i) {
        EXPECT_EQ(ht.get("key" + std::to_string(i)), "");
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}