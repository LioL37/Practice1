#include <iostream>
#include <memory>

using namespace std;

// Интерфейс для персонажа
class Character {
public:
    virtual void display() const = 0;
};

// Интерфейс для фабрики персонажей
class CharacterFactory {
public:
    virtual unique_ptr<Character> createMage() = 0;
    virtual unique_ptr<Character> createWarrior() = 0;
};

// Конкретный класс для мага стихии Земли
class EarthMage : public Character {
public:
    void display() const override {
        cout << "Earth Mage: Наколдовал кучу камней!" << endl;
    }
};

// Конкретный класс для воина стихии Земли
class EarthWarrior : public Character {
public:
    void display() const override {
        cout << "Earth Warrior: Бросился землей!" << endl;
    }
};

// Конкретный класс для мага стихии Воды
class WaterMage : public Character {
public:
    void display() const override {
        cout << "Water Mage: Попил водически!" << endl;
    }
};

// Конкретный класс для воина стихии Воды
class WaterWarrior : public Character {
public:
    // Реализация метода display для воина стихии Воды
    void display() const override {
        cout << "Water Warrior: Дерется сосулькой!" << endl;
    }
};

// Фабрика для создания персонажей стихии Земли
class EarthCharFactory : public CharacterFactory {
public:
    // Создание мага стихии Земли
    unique_ptr<Character> createMage() override {
        return make_unique<EarthMage>();
    }

    // Создание воина стихии Земли
    unique_ptr<Character> createWarrior() override {
        return make_unique<EarthWarrior>();
    }
};

// Фабрика для создания персонажей стихии Воды
class WaterCharFactory : public CharacterFactory {
public:
    // Создание мага стихии Воды
    unique_ptr<Character> createMage() override {
        return make_unique<WaterMage>();
    }

    // Создание воина стихии Воды
    unique_ptr<Character> createWarrior() override {
        return make_unique<WaterWarrior>();
    }
};

// Пример использования
int main() {
    // Создаем фабрику для стихии Земли
    unique_ptr<CharacterFactory> earthFactory = make_unique<EarthCharFactory>();

    // Создаем мага и воина стихии Земли
    unique_ptr<Character> earthMage = earthFactory->createMage();
    unique_ptr<Character> earthWarrior = earthFactory->createWarrior();

    // Выводим характеристики мага и воина стихии Земли
    earthMage->display();
    earthWarrior->display();

    // Создаем фабрику для стихии Воды
    unique_ptr<CharacterFactory> waterFactory = make_unique<WaterCharFactory>();

    // Создаем мага и воина стихии Воды
    unique_ptr<Character> waterMage = waterFactory->createMage();
    unique_ptr<Character> waterWarrior = waterFactory->createWarrior();

    // Выводим характеристики мага и воина стихии Воды
    waterMage->display();
    waterWarrior->display();

    return 0;
}