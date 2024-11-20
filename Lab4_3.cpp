#include <iostream>
#include <vector>

using namespace std;

class BankersAlgorithm {
private:
    int numProcesses; // Количество процессов
    int numResources; // Количество ресурсов
    vector<vector<int>> max; // Максимальные запросы ресурсов для каждого процесса
    vector<vector<int>> allocation; // Текущее распределение ресурсов для каждого процесса
    vector<vector<int>> need; // Необходимые ресурсы для каждого процесса (max - allocation)
    vector<int> available; // Доступные ресурсы

public:
    // Конструктор класса
    BankersAlgorithm(int n, int m) : numProcesses(n), numResources(m) {
        max.resize(n, vector<int>(m));
        allocation.resize(n, vector<int>(m));
        need.resize(n, vector<int>(m));
        available.resize(m);
    }

    // Метод для ввода данных
    void inputData() {
        cout << "Enter the maximum resources for each process:" << endl;
        for (int i = 0; i < numProcesses; ++i) {
            cout << "Process " << i << ":" << endl;
            for (int j = 0; j < numResources; ++j) {
                cout << "Resource " << j << ": ";
                cin >> max[i][j];
            }
        }

        cout << "Enter the allocated resources for each process:" << endl;
        for (int i = 0; i < numProcesses; ++i) {
            cout << "Process " << i << ":" << endl;
            for (int j = 0; j < numResources; ++j) {
                cout << "Resource " << j << ": ";
                cin >> allocation[i][j];
                need[i][j] = max[i][j] - allocation[i][j]; // Вычисление необходимого количества ресурсов
            }
        }

        cout << "Enter the available resources:" << endl;
        for (int j = 0; j < numResources; ++j) {
            cout << "Resource " << j << ": ";
            cin >> available[j];
        }
    }

    // Метод для проверки, является ли состояние системы безопасным
    bool isSafeState(vector<int>& safeSequence) {
        vector<int> work = available; // Копия доступных ресурсов
        vector<bool> finish(numProcesses, false); // Массив для отслеживания завершения процессов

        while (true) {
            bool found = false;
            for (int i = 0; i < numProcesses; ++i) {
                if (!finish[i]) { // Если процесс еще не завершен
                    bool canAllocate = true;
                    for (int j = 0; j < numResources; ++j) {
                        if (need[i][j] > work[j]) { // Проверка, можно ли выделить ресурсы
                            canAllocate = false;
                            break;
                        }
                    }
                    if (canAllocate) { // Если ресурсы могут быть выделены
                        for (int j = 0; j < numResources; ++j) {
                            work[j] += allocation[i][j]; // Возвращение ресурсов в рабочий набор
                        }
                        safeSequence.push_back(i); // Добавление процесса в безопасную последовательность
                        finish[i] = true; // Пометка процесса как завершенного
                        found = true;
                    }
                }
            }
            if (!found) { // Если не найдено ни одного процесса, который может быть выполнен
                break;
            }
        }

        for (int i = 0; i < numProcesses; ++i) {
            if (!finish[i]) { // Если хотя бы один процесс не завершен
                return false; // Система находится в небезопасном состоянии
            }
        }
        return true; // Все процессы завершены, система находится в безопасном состоянии
    }

    // Метод для запуска алгоритма
    void run() {
        vector<int> safeSequence;
        if (isSafeState(safeSequence)) {
            cout << "System is in a safe state." << endl;
            cout << "Safe sequence: ";
            for (int i : safeSequence) {
                cout << "P" << i << " ";
            }
            cout << endl;
        } else {
            cout << "System is in an unsafe state. Deadlock may occur." << endl;
        }
    }
};

int main() {
    int numProcesses, numResources;
    cout << "Enter the number of processes: ";
    cin >> numProcesses;
    cout << "Enter the number of resources: ";
    cin >> numResources;

    BankersAlgorithm banker(numProcesses, numResources);
    banker.inputData();
    banker.run();

    return 0;
}