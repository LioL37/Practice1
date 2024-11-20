#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

struct Domino {
    int first;
    int second;

    bool operator==(const Domino& other) const {
        return (first == other.first && second == other.second) ||
               (first == other.second && second == other.first);
    }

    bool operator<(const Domino& other) const {
        if (first != other.first) return first < other.first;
        return second < other.second;
    }
};

vector<Domino> GenerateFullSet(int dotsOnDomino) {
    vector<Domino> fullSet;
    for (int i = 0; i < dotsOnDomino; ++i) {
        for (int j = i; j < dotsOnDomino; ++j) {
            fullSet.push_back({i, j});
        }
    }
    return fullSet;
}

vector<Domino> GenerateRandomSet(const vector<Domino>& fullSet, int size) {
    vector<Domino> randomSet = fullSet; // Копируем полный набор
    random_device rd;
    mt19937 g(rd());
    shuffle(randomSet.begin(), randomSet.end(), g); // Перемешиваем набор домино
    randomSet.resize(size); // Урезаем набор домино
    return randomSet;
}

vector<Domino> FindMissingDominoes(const vector<Domino>& fullSet, const vector<Domino>& randomSet) {
    vector<Domino> missingDominoes;
    for (const auto& domino : fullSet) {
        // Если значение не найдено, то find вернет указатель на конец набора
        if (find(randomSet.begin(), randomSet.end(), domino) == randomSet.end()) {
            missingDominoes.push_back(domino);
        }
    }
    return missingDominoes;
}

mutex mtx;

void FindMissingDominoesParallel(const vector<Domino>& fullSet, const vector<Domino>& randomSet, vector<Domino>& missingDominoes, int start, int end) {
    for (int i = start; i < end; ++i) {
        if (find(randomSet.begin(), randomSet.end(), fullSet[i]) == randomSet.end()) {
            lock_guard<mutex> lock(mtx);
            missingDominoes.push_back(fullSet[i]);
        }
    }
}

vector<Domino> FindMissingDominoesParallelWrapper(const vector<Domino>& fullSet, const vector<Domino>& randomSet, int threadCount) {
    vector<Domino> missingDominoes;
    vector<thread> threads;
    int chunkSize = fullSet.size() / threadCount; // Делим задачу между потоками

    for (int i = 0; i < threadCount; ++i) {
        int start = i * chunkSize; // Каждому потоку отводится свой участок поиска
        int end = (i == threadCount - 1) ? fullSet.size() : start + chunkSize;
        // emplace_back позволяет создать элемент в конце вектора, без необходимости предварительного создания этого элемента
        // Поток будет выполнять функцию поиска на выделенном ему участке набора
        threads.emplace_back(FindMissingDominoesParallel, ref(fullSet), ref(randomSet), ref(missingDominoes), start, end);
        // ref это ссылки на объект
    }

    for (auto& thread : threads) {
        thread.join(); // Ждем завершения остальных потоков
    }

    return missingDominoes;
}

template<typename T>
void MeasureTime(const string& name, T function) {
    auto start = chrono::high_resolution_clock::now();
    function();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << name << " took " << duration.count() << " seconds\n";
}

int main() {
    srand(static_cast<unsigned>(time(0)));  // Инициализация генератора случайных чисел
    int threadCount = 4;
    int dotsOnDomino = 7; // Сколько точек может быть на половинке кости домино
                          // количество домино при 7 (0,...,6) будет 28
    int amountOfDominos = dotsOnDomino * (dotsOnDomino + 1) / 2; // Вычисление количества костяшек домино
    int randomSetSize = amountOfDominos / 2; // Будем считать что не хватает половины костяшек

    auto fullSet = GenerateFullSet(dotsOnDomino);
    auto randomSet = GenerateRandomSet(fullSet, randomSetSize);

    vector<Domino> missingDominoes;

    // Однопоточная обработка
    MeasureTime("Single-threaded", [&]() {
        missingDominoes = FindMissingDominoes(fullSet, randomSet);
    });

    //// Вывод результатов
    //cout << "Missing dominoes:\n";
    //for (const auto& domino : missingDominoes) {
    //    cout << "(" << domino.first << ", " << domino.second << ")\n";
    //}

    // Многопоточная обработка
    MeasureTime("Multi-threaded", [&]() {
        missingDominoes = FindMissingDominoesParallelWrapper(fullSet, randomSet, threadCount);
    });

    // Вывод результатов
    cout << "Missing dominoes:\n";
    for (const auto& domino : missingDominoes) {
        cout << "(" << domino.first << ", " << domino.second << ")\n";
    }

    return 0;
}