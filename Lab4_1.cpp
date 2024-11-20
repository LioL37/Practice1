#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <vector>
#include <random>
#include <functional>

int threadCount = 10;  // Количество потоков
int iterationsPerThread = 10000;  // Количество итераций на каждый поток

// Mutex
std::mutex mutex;  // Объект мьютекса для синхронизации доступа к общему ресурсу

// Semaphore
class Semaphore {
public:
    Semaphore(int count = 0) : count(count) {}

    void acquire() {
        std::unique_lock<std::mutex> lock(mtx);  // Блокировка мьютекса
        cv.wait(lock, [this]() { return count > 0; });  // Ожидание, пока count > 0
        count--;  // Уменьшение счетчика
    }

    void release() {
        std::unique_lock<std::mutex> lock(mtx);  // Блокировка мьютекса
        count++;  // Увеличение счетчика
        cv.notify_one();  // Уведомление одного ожидающего потока
    }

private:
    std::mutex mtx;  // Мьютекс для защиты доступа к счетчику
    std::condition_variable cv;  // Условная переменная для ожидания и уведомления
    int count;  // Счетчик семафора
};

Semaphore semaphore(1);  // Инициализация семафора с начальным значением 1

// SemaphoreSlim
class SemaphoreSlim {
public:
    SemaphoreSlim(int count = 0) : count(count) {}

    void acquire() {
        while (true) {
            int expected = count.load();  // Загрузка текущего значения счетчика
            if (expected > 0 && count.compare_exchange_weak(expected, expected - 1)) {
                break;  // Если счетчик больше 0 и успешно уменьшен, выходим из цикла
            }
            std::this_thread::yield();  // Передача управления другому потоку
        }
    }

    void release() {
        count.fetch_add(1);  // Увеличение счетчика на 1
    }

private:
    std::atomic<int> count;  // Атомарный счетчик
};

SemaphoreSlim semaphoreSlim(1);  // Инициализация SemaphoreSlim с начальным значением 1

// Barrier
class Barrier {
public:
    Barrier(int count) : threshold(count), count(count), generation(0) {}

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);  // Блокировка мьютекса
        auto currentGen = generation;  // Сохранение текущего поколения
        if (--count == 0) {  // Уменьшение счетчика и проверка, достигнут ли барьер (сначала уменьшаем, потом проверяем)
            generation++;  // Переход к следующему поколению
            count = threshold;  // Сброс счетчика
            cv.notify_all();  // Уведомление всех ожидающих потоков
        } else {
            cv.wait(lock, [this, currentGen]() { return currentGen != generation; });  // Ожидание нового поколения
        }
    }

private:
    std::mutex mtx;  // Мьютекс для защиты доступа к счетчику и поколению
    std::condition_variable cv;  // Условная переменная для ожидания и уведомления
    int threshold;  // Пороговое значение счетчика
    int count;  // Текущий счетчик
    int generation;  // Поколение барьера
};

Barrier barrier(threadCount);  // Инициализация барьера с количеством потоков

// SpinLock
std::atomic_flag spinlock = ATOMIC_FLAG_INIT;  // Атомарный флаг для реализации SpinLock

// SpinWait
class SpinWait {
public:
    void lock() {
        int backoff = 1;  // Начальная задержка
        while (flag.test_and_set(std::memory_order_acquire)) {  // Попытка захвата флага
            for (int i = 0; i < backoff; ++i) {
                std::this_thread::yield();  // Передача управления другому потоку
            }
            backoff = std::min(backoff * 2, 1024);  // Увеличение задержки
        }
    }

    void unlock() {
        flag.clear(std::memory_order_release);  // Освобождение флага
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;  // Атомарный флаг для SpinWait
};

SpinWait spinWait;  // Инициализация SpinWait

// Monitor
class Monitor {
public:
    void lock() {
        mtx.lock();  // Блокировка мьютекса
    }

    void unlock() {
        mtx.unlock();  // Разблокировка мьютекса
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);  // Блокировка мьютекса
        cv.wait(lock);  // Ожидание уведомления
    }

    void notify_one() {
        cv.notify_one();  // Уведомление одного ожидающего потока
    }

    void notify_all() {
        cv.notify_all();  // Уведомление всех ожидающих потоков
    }

private:
    std::mutex mtx;  // Мьютекс для защиты доступа
    std::condition_variable cv;  // Условная переменная для ожидания и уведомления
};

Monitor monitor;  // Инициализация монитора

// Функция для запуска потоков с использованием примитива синхронизации
void RunPrimitive(std::function<void()> lock, std::function<void()> unlock) {
    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; i++) {
        threads.emplace_back([&]() {
            for (int j = 0; j < iterationsPerThread; j++) {
                lock();  // Захват примитива синхронизации
                // Симуляция работы с генерацией случайного символа ASCII
                char randomChar = 'A' + rand() % 26;
                std::cout << randomChar;
                unlock();  // Освобождение примитива синхронизации
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();  // Ожидание завершения всех потоков
    }
}

// Тест для Mutex
void MutexTest() {
    RunPrimitive([]() { mutex.lock(); }, []() { mutex.unlock(); });
}

// Тест для Semaphore
void SemaphoreTest() {
    RunPrimitive([]() { semaphore.acquire(); }, []() { semaphore.release(); });
}

// Тест для SemaphoreSlim
void SemaphoreSlimTest() {
    RunPrimitive([]() { semaphoreSlim.acquire(); }, []() { semaphoreSlim.release(); });
}

// Тест для Barrier
void BarrierTest() {
    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; i++) {
        threads.emplace_back([&]() {
            for (int j = 0; j < iterationsPerThread; j++) {
                barrier.wait();  // Ожидание барьера
                // Симуляция работы с генерацией случайного символа ASCII
                char randomChar = 'A' + rand() % 26;
                std::cout << randomChar;
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();  // Ожидание завершения всех потоков
    }
}

// Тест для SpinLock
void SpinLockTest() {
    RunPrimitive([]() { while (spinlock.test_and_set(std::memory_order_acquire)); }, []() { spinlock.clear(std::memory_order_release); });
}

// Тест для SpinWait
void SpinWaitTest() {
    RunPrimitive([]() { spinWait.lock(); }, []() { spinWait.unlock(); });
}

// Тест для Monitor
void MonitorTest() {
    RunPrimitive([]() { monitor.lock(); }, []() { monitor.unlock(); });
}

// Шаблонная функция для измерения времени выполнения
template<typename T>
void MeasureTime(const std::string& name, T function) {
    auto start = std::chrono::high_resolution_clock::now();  // Запуск таймера
    function();  // Выполнение функции
    auto end = std::chrono::high_resolution_clock::now();  // Остановка таймера
    std::chrono::duration<double> duration = end - start;  // Вычисление продолжительности
    std::cout << "\n" << name << " took " << duration.count() << " seconds\n";  // Вывод результата
}

int main() {
    srand(static_cast<unsigned>(time(0)));  // Инициализация генератора случайных чисел
    std::cout << "1) MutexTest" << std::endl
            << "2) SemaphoreTest" << std::endl
            << "3) SemaphoreSlimTest" << std::endl
            << "4) BarrierTest" << std::endl
            << "5) SpinLockTest" << std::endl
            << "6) SpinWaitTest" << std::endl
            << "7) MonitorTest" << std::endl
            << "0 to exit" << std::endl;
    while(true) {
        int choice;
        std::cin >> choice;
        if (choice == 0) {
            break;
        }
        switch (choice) {
        case(1):
            MeasureTime("MutexTest", MutexTest);
            break;
        case(2):
            MeasureTime("SemaphoreTest", SemaphoreTest);
            break;
        case(3):
            MeasureTime("SemaphoreSlimTest", SemaphoreSlimTest);
            break;
        case(4):
            MeasureTime("BarrierTest", BarrierTest);
            break;
        case(5):
            MeasureTime("SpinLockTest", SpinLockTest);
            break;
        case(6):
            MeasureTime("SpinWaitTest", SpinWaitTest);
            break;
        case(7):
            MeasureTime("MonitorTest", MonitorTest);
            break;
        default:
            break;
        }
    } 
    return 0;
}