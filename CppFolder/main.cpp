#include <iostream>
#include <memory>
#include <vector>
#include <locale>
#include "driver.h"
#include "vehicle.h"
#include "bus.h"
#include "trolleybus.h"
#include "tram.h"

using namespace std;

void printMenu() {
    cout << "\n========= МЕНЮ (ЛР №3) =========\n";
    cout << "1. Показать унаследованные поля всех ТС\n";
    cout << "2. Показать специфичные поля производных типов\n";
    cout << "3. Унаследованный сеттер (изменить общее поле)\n";
    cout << "4. Унаследованный геттер (получить общее поле)\n";
    cout << "0. Выход\n";
    cout << "================================\n";
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    // ===== Водители =====
    auto driver1 = make_shared<Driver>("Иванов Иван Иванович", 15);
    auto driver2 = make_shared<Driver>("Петров Пётр Петрович", 8);

    // ===== Объекты производных классов =====
    auto bus = make_shared<Bus>("А123ВС", "ПАЗ-3205", 2015, 30, 80, driver1,
                                 2, "городской");
    auto trolley = make_shared<Trolleybus>("В456ЕК", "АКСМ-321", 2018, 90, 70, driver2,
                                            550, true);
    auto tram = make_shared<Tram>("С789МН", "БКМ-843", 2020, 150, 60, driver1,
                                   5, true);

    // ===== Вектор указателей на БАЗОВЫЙ класс =====
    vector<shared_ptr<Vehicle>> vehicles = {bus, trolley, tram};

    int choice = -1;

    do {
        printMenu();
        cout << "Выберите пункт меню: ";
        cin >> choice;

        switch (choice) {
            // ===== 1. Унаследованные поля =====
            case 1: {
                cout << "\n=== УНАСЛЕДОВАННЫЕ ПОЛЯ (из Vehicle) ===\n";
                for (const auto& v : vehicles) {
                    cout << "Госномер: " << v->getRegNumber()
                         << " | Модель: " << v->getModel()
                         << " | Год: " << v->getYear()
                         << " | Вместимость: " << v->getCapacity() << " чел."
                         << " | Скорость: " << v->getMaxSpeed() << " км/ч"
                         << endl;
                }
                break;
            }

            // ===== 2. Специфичные поля =====
            case 2: {
                cout << "\n=== СПЕЦИФИЧНЫЕ ПОЛЯ ===\n";

                cout << "--- Автобус ---\n";
                cout << "Госномер: " << bus->getRegNumber() << endl;
                cout << "Дверей: " << bus->getDoorCount() << endl;
                cout << "Салон: " << bus->getSalonType() << endl;

                cout << "\n--- Троллейбус ---\n";
                cout << "Госномер: " << trolley->getRegNumber() << endl;
                cout << "Напряжение: " << trolley->getVoltage() << " В" << endl;
                cout << "Аккумулятор: " << (trolley->hasBattery() ? "есть" : "нет") << endl;

                cout << "\n--- Трамвай ---\n";
                cout << "Госномер: " << tram->getRegNumber() << endl;
                cout << "Номер пути: " << tram->getTrackNumber() << endl;
                cout << "Пантограф: " << (tram->hasPantograph() ? "есть" : "нет") << endl;
                break;
            }

            // ===== 3. Унаследованный сеттер =====
            case 3: {
                cout << "\n=== УНАСЛЕДОВАННЫЙ СЕТТЕР ===\n";
                cout << "Меняем ВМЕСТИМОСТЬ у объектов (унаследованный setCapacity):\n";

                // Изменяем через УНАСЛЕДОВАННЫЙ сеттер
                bus->setCapacity(35);
                trolley->setCapacity(95);
                tram->setCapacity(160);

                cout << "\nПосле изменения:\n";
                cout << "Автобус: " << bus->getCapacity() << " чел.\n";
                cout << "Троллейбус: " << trolley->getCapacity() << " чел.\n";
                cout << "Трамвай: " << tram->getCapacity() << " чел.\n";
                break;
            }

            // ===== 4. Унаследованный геттер =====
            case 4: {
                cout << "\n=== УНАСЛЕДОВАННЫЙ ГЕТТЕР ===\n";
                cout << "Получаем ГОД ВЫПУСКА (унаследованный getYear):\n";

                cout << "Автобус: " << bus->getYear() << endl;
                cout << "Троллейбус: " << trolley->getYear() << endl;
                cout << "Трамвай: " << tram->getYear() << endl;
                break;
            }

            case 0: {
                cout << "Выход из программы. До свидания!\n";
                break;
            }

            default: {
                cout << "Неверный пункт меню. Попробуйте снова.\n";
                break;
            }
        }

    } while (choice != 0);

    return 0;
}