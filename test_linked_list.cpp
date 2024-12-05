#include <gtest/gtest.h>
#include "LinkedLists.h"  // Подключаем ваши классы SinglyLinkedList и DoublyLinkedList

// Тесты для односвязного списка

// Тест конструктора по умолчанию
TEST(SinglyLinkedListTest, DefaultConstructor) {
    SinglyLinkedList<int> list;
    EXPECT_EQ(list.head, nullptr);  // Проверяем, что голова списка равна nullptr
}

// Тест добавления элемента в голову списка
TEST(SinglyLinkedListTest, AddToHead) {
    SinglyLinkedList<int> list;
    list.addToHead(1);  // Добавляем элемент 1 в голову
    list.addToHead(2);  // Добавляем элемент 2 в голову
    EXPECT_EQ(list.head->value, 2);  // Проверяем, что голова списка равна 2
    EXPECT_EQ(list.head->next->value, 1);  // Проверяем, что следующий элемент после головы равен 1
}

// Тест добавления элемента в хвост списка
TEST(SinglyLinkedListTest, AddToTail) {
    SinglyLinkedList<int> list;
    list.addToTail(1);  // Добавляем элемент 1 в хвост
    list.addToTail(2);  // Добавляем элемент 2 в хвост
    EXPECT_EQ(list.head->value, 1);  // Проверяем, что голова списка равна 1
    EXPECT_EQ(list.head->next->value, 2);  // Проверяем, что следующий элемент после головы равен 2
}

// Тест удаления элемента с головы списка
TEST(SinglyLinkedListTest, RemoveFromHead) {
    SinglyLinkedList<int> list;
    list.addToHead(1);  // Добавляем элемент 1 в голову
    list.addToHead(2);  // Добавляем элемент 2 в голову
    list.removeFromHead();  // Удаляем элемент с головы
    EXPECT_EQ(list.head->value, 1);  // Проверяем, что голова списка равна 1
}

// Тест удаления элемента с хвоста списка
TEST(SinglyLinkedListTest, RemoveFromTail) {
    SinglyLinkedList<int> list;
    list.addToTail(1);  // Добавляем элемент 1 в хвост
    list.addToTail(2);  // Добавляем элемент 2 в хвост
    list.removeFromTail();  // Удаляем элемент с хвоста
    EXPECT_EQ(list.head->value, 1);  // Проверяем, что голова списка равна 1
    EXPECT_EQ(list.head->next, nullptr);  // Проверяем, что следующий элемент после головы равен nullptr
}

// Тест удаления элемента по значению
TEST(SinglyLinkedListTest, RemoveByValue) {
    SinglyLinkedList<int> list;
    list.addToTail(1);  // Добавляем элемент 1 в хвост
    list.addToTail(2);  // Добавляем элемент 2 в хвост
    list.addToTail(3);  // Добавляем элемент 3 в хвост
    list.removeByValue(2);  // Удаляем элемент со значением 2
    EXPECT_EQ(list.head->value, 1);  // Проверяем, что голова списка равна 1
    EXPECT_EQ(list.head->next->value, 3);  // Проверяем, что следующий элемент после головы равен 3
}

// Тест поиска элемента по значению
TEST(SinglyLinkedListTest, Search) {
    SinglyLinkedList<int> list;
    list.addToTail(1);  // Добавляем элемент 1 в хвост
    list.addToTail(2);  // Добавляем элемент 2 в хвост
    EXPECT_TRUE(list.search(1));  // Проверяем, что элемент 1 найден
    EXPECT_TRUE(list.search(2));  // Проверяем, что элемент 2 найден
    EXPECT_FALSE(list.search(3));  // Проверяем, что элемент 3 не найден
}

// Тест вывода пустого списка
TEST(SinglyLinkedListTest, PrintEmptyList) {
    SinglyLinkedList<int> list;
    testing::internal::CaptureStdout();  // Перехватываем вывод в stdout
    list.print();  // Выводим список
    std::string output = testing::internal::GetCapturedStdout();  // Получаем вывод
    EXPECT_EQ(output, "\n");  // Проверяем, что вывод пустой
}

// Тест вывода списка с одним элементом
TEST(SinglyLinkedListTest, PrintSingleElementList) {
    SinglyLinkedList<int> list;
    list.addToHead(1);  // Добавляем элемент 1 в голову
    testing::internal::CaptureStdout();  // Перехватываем вывод в stdout
    list.print();  // Выводим список
    std::string output = testing::internal::GetCapturedStdout();  // Получаем вывод
    EXPECT_EQ(output, "1 \n");  // Проверяем, что вывод содержит один элемент
}

// Тест добавления элемента в пустой список
TEST(SinglyLinkedListTest, AddToHeadEmptyList) {
    SinglyLinkedList<int> list;
    list.addToHead(1);
    EXPECT_EQ(list.head->value, 1);
    EXPECT_EQ(list.head->next, nullptr);
}

// Тест добавления элемента в хвост пустого списка
TEST(SinglyLinkedListTest, AddToTailEmptyList) {
    SinglyLinkedList<int> list;
    list.addToTail(1);
    EXPECT_EQ(list.head->value, 1);
    EXPECT_EQ(list.head->next, nullptr);
}

// Тест удаления последнего элемента из списка
TEST(SinglyLinkedListTest, RemoveLastElement) {
    SinglyLinkedList<int> list;
    list.addToHead(1);
    list.removeFromHead();
    EXPECT_EQ(list.head, nullptr);
}

// Тест удаления элемента из списка с одним элементом
TEST(SinglyLinkedListTest, RemoveFromSingleElementList) {
    SinglyLinkedList<int> list;
    list.addToHead(1);
    list.removeFromHead();
    EXPECT_EQ(list.head, nullptr);
}

// Тест удаления элемента из списка с несколькими элементами
TEST(SinglyLinkedListTest, RemoveFromMultipleElementList) {
    SinglyLinkedList<int> list;
    list.addToHead(1);
    list.addToHead(2);
    list.removeFromHead();
    EXPECT_EQ(list.head->value, 1);
    EXPECT_EQ(list.head->next, nullptr);
}

// Тест поиска элемента в пустом списке
TEST(SinglyLinkedListTest, SearchInEmptyList) {
    SinglyLinkedList<int> list;
    EXPECT_FALSE(list.search(1));
}

// Тест поиска элемента в списке с одним элементом
TEST(SinglyLinkedListTest, SearchInSingleElementList) {
    SinglyLinkedList<int> list;
    list.addToHead(1);
    EXPECT_TRUE(list.search(1));
    EXPECT_FALSE(list.search(2));
}

// Тест поиска элемента в списке с несколькими элементами
TEST(SinglyLinkedListTest, SearchInMultipleElementList) {
    SinglyLinkedList<int> list;
    list.addToHead(1);
    list.addToHead(2);
    EXPECT_TRUE(list.search(1));
    EXPECT_TRUE(list.search(2));
    EXPECT_FALSE(list.search(3));
}

// Тест удаления элемента с головы из пустого списка
TEST(SinglyLinkedListTest, RemoveFromHeadEmptyList) {
    SinglyLinkedList<int> list;
    EXPECT_THROW(list.removeFromHead(), runtime_error);  // Проверяем, что удаление из пустого списка вызывает исключение
}

// Тест удаления элемента с хвоста из пустого списка
TEST(SinglyLinkedListTest, RemoveFromTailEmptyList) {
    SinglyLinkedList<int> list;
    EXPECT_THROW(list.removeFromTail(), runtime_error);  // Проверяем, что удаление из пустого списка вызывает исключение
}

// Тест удаления элемента по значению из пустого списка
TEST(SinglyLinkedListTest, RemoveByValueEmptyList) {
    SinglyLinkedList<int> list;
    EXPECT_THROW(list.removeByValue(1), runtime_error);  // Проверяем, что удаление из пустого списка вызывает исключение
}

// Тест удаления элемента по значению, которого нет в списке
TEST(SinglyLinkedListTest, RemoveNonExistentValue) {
    SinglyLinkedList<int> list;
    list.addToTail(1);  // Добавляем элемент 1 в хвост
    EXPECT_THROW(list.removeByValue(2), runtime_error);  // Проверяем, что удаление несуществующего элемента вызывает исключение
}

// Тесты для двусвязного списка

// Тест конструктора по умолчанию
TEST(DoublyLinkedListTest, DefaultConstructor) {
    DoublyLinkedList<int> list;
    EXPECT_EQ(list.head, nullptr);  // Проверяем, что голова списка равна nullptr
    EXPECT_EQ(list.tail, nullptr);  // Проверяем, что хвост списка равна nullptr
}

// Тест добавления элемента в голову списка
TEST(DoublyLinkedListTest, AddToHead) {
    DoublyLinkedList<int> list;
    list.addToHead(1);  // Добавляем элемент 1 в голову
    list.addToHead(2);  // Добавляем элемент 2 в голову
    EXPECT_EQ(list.head->value, 2);  // Проверяем, что голова списка равна 2
    EXPECT_EQ(list.head->next->value, 1);  // Проверяем, что следующий элемент после головы равен 1
    EXPECT_EQ(list.tail->value, 1);  // Проверяем, что хвост списка равен 1
}

// Тест добавления элемента в хвост списка
TEST(DoublyLinkedListTest, AddToTail) {
    DoublyLinkedList<int> list;
    list.addToTail(1);  // Добавляем элемент 1 в хвост
    list.addToTail(2);  // Добавляем элемент 2 в хвост
    EXPECT_EQ(list.head->value, 1);  // Проверяем, что голова списка равна 1
    EXPECT_EQ(list.head->next->value, 2);  // Проверяем, что следующий элемент после головы равен 2
    EXPECT_EQ(list.tail->value, 2);  // Проверяем, что хвост списка равен 2
}

// Тест удаления элемента с головы списка
TEST(DoublyLinkedListTest, RemoveFromHead) {
    DoublyLinkedList<int> list;
    list.addToHead(1);  // Добавляем элемент 1 в голову
    list.addToHead(2);  // Добавляем элемент 2 в голову
    list.removeFromHead();  // Удаляем элемент с головы
    EXPECT_EQ(list.head->value, 1);  // Проверяем, что голова списка равна 1
    EXPECT_EQ(list.tail->value, 1);  // Проверяем, что хвост списка равен 1
}

// Тест удаления элемента с хвоста списка
TEST(DoublyLinkedListTest, RemoveFromTail) {
    DoublyLinkedList<int> list;
    list.addToTail(1);  // Добавляем элемент 1 в хвост
    list.addToTail(2);  // Добавляем элемент 2 в хвост
    list.removeFromTail();  // Удаляем элемент с хвоста
    EXPECT_EQ(list.head->value, 1);  // Проверяем, что голова списка равна 1
    EXPECT_EQ(list.tail->value, 1);  // Проверяем, что хвост списка равен 1
}

// Тест удаления элемента по значению
TEST(DoublyLinkedListTest, RemoveByValue) {
    DoublyLinkedList<int> list;
    list.addToTail(1);  // Добавляем элемент 1 в хвост
    list.addToTail(2);  // Добавляем элемент 2 в хвост
    list.addToTail(3);  // Добавляем элемент 3 в хвост
    list.removeByValue(2);  // Удаляем элемент со значением 2
    EXPECT_EQ(list.head->value, 1);  // Проверяем, что голова списка равна 1
    EXPECT_EQ(list.head->next->value, 3);  // Проверяем, что следующий элемент после головы равен 3
    EXPECT_EQ(list.tail->value, 3);  // Проверяем, что хвост списка равен 3
}

// Тест поиска элемента по значению
TEST(DoublyLinkedListTest, Search) {
    DoublyLinkedList<int> list;
    list.addToTail(1);  // Добавляем элемент 1 в хвост
    list.addToTail(2);  // Добавляем элемент 2 в хвост
    EXPECT_TRUE(list.search(1));  // Проверяем, что элемент 1 найден
    EXPECT_TRUE(list.search(2));  // Проверяем, что элемент 2 найден
    EXPECT_FALSE(list.search(3));  // Проверяем, что элемент 3 не найден
}

// Тест вывода пустого списка
TEST(DoublyLinkedListTest, PrintEmptyList) {
    DoublyLinkedList<int> list;
    testing::internal::CaptureStdout();  // Перехватываем вывод в stdout
    list.print();  // Выводим список
    std::string output = testing::internal::GetCapturedStdout();  // Получаем вывод
    EXPECT_EQ(output, "\n");  // Проверяем, что вывод пустой
}

// Тест вывода списка с одним элементом
TEST(DoublyLinkedListTest, PrintSingleElementList) {
    DoublyLinkedList<int> list;
    list.addToHead(1);  // Добавляем элемент 1 в голову
    testing::internal::CaptureStdout();  // Перехватываем вывод в stdout
    list.print();  // Выводим список
    std::string output = testing::internal::GetCapturedStdout();  // Получаем вывод
    EXPECT_EQ(output, "1 \n");  // Проверяем, что вывод содержит один элемент
}

// Тест добавления элемента в пустой список
TEST(DoublyLinkedListTest, AddToHeadEmptyList) {
    DoublyLinkedList<int> list;
    list.addToHead(1);
    EXPECT_EQ(list.head->value, 1);
    EXPECT_EQ(list.tail->value, 1);
    EXPECT_EQ(list.head->next, nullptr);
    EXPECT_EQ(list.head->prev, nullptr);
}

// Тест добавления элемента в хвост пустого списка
TEST(DoublyLinkedListTest, AddToTailEmptyList) {
    DoublyLinkedList<int> list;
    list.addToTail(1);
    EXPECT_EQ(list.head->value, 1);
    EXPECT_EQ(list.tail->value, 1);
    EXPECT_EQ(list.head->next, nullptr);
    EXPECT_EQ(list.head->prev, nullptr);
}

// Тест удаления последнего элемента из списка
TEST(DoublyLinkedListTest, RemoveLastElement) {
    DoublyLinkedList<int> list;
    list.addToHead(1);
    list.removeFromHead();
    EXPECT_EQ(list.head, nullptr);
    EXPECT_EQ(list.tail, nullptr);
}

// Тест удаления элемента из списка с одним элементом
TEST(DoublyLinkedListTest, RemoveFromSingleElementList) {
    DoublyLinkedList<int> list;
    list.addToHead(1);
    list.removeFromHead();
    EXPECT_EQ(list.head, nullptr);
    EXPECT_EQ(list.tail, nullptr);
}

// Тест удаления элемента из списка с несколькими элементами
TEST(DoublyLinkedListTest, RemoveFromMultipleElementList) {
    DoublyLinkedList<int> list;
    list.addToHead(1);
    list.addToHead(2);
    list.removeFromHead();
    EXPECT_EQ(list.head->value, 1);
    EXPECT_EQ(list.tail->value, 1);
    EXPECT_EQ(list.head->next, nullptr);
    EXPECT_EQ(list.head->prev, nullptr);
}

// Тест поиска элемента в пустом списке
TEST(DoublyLinkedListTest, SearchInEmptyList) {
    DoublyLinkedList<int> list;
    EXPECT_FALSE(list.search(1));
}

// Тест поиска элемента в списке с одним элементом
TEST(DoublyLinkedListTest, SearchInSingleElementList) {
    DoublyLinkedList<int> list;
    list.addToHead(1);
    EXPECT_TRUE(list.search(1));
    EXPECT_FALSE(list.search(2));
}

// Тест поиска элемента в списке с несколькими элементами
TEST(DoublyLinkedListTest, SearchInMultipleElementList) {
    DoublyLinkedList<int> list;
    list.addToHead(1);
    list.addToHead(2);
    EXPECT_TRUE(list.search(1));
    EXPECT_TRUE(list.search(2));
    EXPECT_FALSE(list.search(3));
}

// Тест удаления элемента с головы из пустого списка
TEST(DoublyLinkedListTest, RemoveFromHeadEmptyList) {
    DoublyLinkedList<int> list;
    EXPECT_THROW(list.removeFromHead(), runtime_error);  // Проверяем, что удаление из пустого списка вызывает исключение
}

// Тест удаления элемента с хвоста из пустого списка
TEST(DoublyLinkedListTest, RemoveFromTailEmptyList) {
    DoublyLinkedList<int> list;
    EXPECT_THROW(list.removeFromTail(), runtime_error);  // Проверяем, что удаление из пустого списка вызывает исключение
}

// Тест удаления элемента по значению из пустого списка
TEST(DoublyLinkedListTest, RemoveByValueEmptyList) {
    DoublyLinkedList<int> list;
    EXPECT_THROW(list.removeByValue(1), runtime_error);  // Проверяем, что удаление из пустого списка вызывает исключение
}

// Тест удаления элемента по значению, которого нет в списке
TEST(DoublyLinkedListTest, RemoveNonExistentValue) {
    DoublyLinkedList<int> list;
    list.addToTail(1);  // Добавляем элемент 1 в хвост
    EXPECT_THROW(list.removeByValue(2), runtime_error);  // Проверяем, что удаление несуществующего элемента вызывает исключение
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}