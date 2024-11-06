#include <iostream>
#include "LinkedLists.h"
#include "Stack.h"
#include "Vector.h"

using namespace std;

// Функция для проверки корректности XML-строки
bool isCorrectXML(const string& s) {
    Stack tags;  // Стек для хранения тегов
    size_t i = 0;  // Индекс для обхода строки

    // Парсинг строки
    while (i < s.size()) {
        if (s[i] == '<') {
            if (i + 1 < s.size() && s[i + 1] == '/') {
                //  Для закрывающего тега
                size_t j = i + 2;  // Начинаем считывать имя тега после </
                while (j < s.size() && s[j] != '>') {
                    j++;  // Пропускаем символы до закрывающей угловой скобки >
                }
                if (j == s.size()) { // Если > не найдена, строка некорректна
                    return false;
                }
                string tag = s.substr(i + 2, j - i - 2);
                // Проверяем, соответствует ли закрывающий тег открывающему
                if (tags.isEmpty() || tags.peek() != tag) {
                    return false;
                }
                tags.pop();  // Удаляем тег из стека
                i = j + 1;  // Переходим к следующему символу после '>'
            } else {
                // Для открывающего тега
                size_t j = i + 1;
                while (j < s.size() && s[j] != '>') {
                    j++;
                }
                if (j == s.size()) {
                    return false;
                }
                string tag = s.substr(i + 1, j - i - 1);
                tags.push(tag);  // Добавляем тег в стек
                i = j + 1;
            }
        } else {
            i++;
        }
    }

    // Если стек пуст, то все теги закрылись
    return tags.isEmpty();
}

// Функция для восстановления корректной XML-строки после повреждения одного символа
string restoreXML(const string& s) {
    Vector<char> allowedChars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '<', '>', '/'};
    if (isCorrectXML(s)) {
        return s;  // Если строка корректна, возвращаем ее как есть
    }
    // Перебираем каждый символ строки
    for (size_t i = 0; i < s.size(); ++i) {
        // Перебираем все допустимые символы
        for (char c : allowedChars) {
            // Пропускаем, если символ не изменился
            if (s[i] == c) continue;
            string modified = s;  // Создаем копию строки
            modified[i] = c;  // Заменяем символ
            // Проверяем, стала ли строка корректной после замены
            if (isCorrectXML(modified)) {
                return modified;  // Возвращаем восстановленную строку
            }
        }
    } // Здесь мы пробуем менять по одному символу до тех пор пока строка не станет корректной

    return "Не удалось восстановить строку";
}

int main() {
    string stroka;
    getline(cin, stroka);  // Считываем строку из стандартного ввода

    string result = restoreXML(stroka);  // Восстанавливаем корректную XML-строку
    cout << result << endl;  // Выводим результат

    return 0;
}