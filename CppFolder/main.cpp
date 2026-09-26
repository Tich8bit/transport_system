#include <iostream>
#include <memory>
#include <vector>
#include <locale>
#include "driver.h"
#include "vehicle.h"
#include "bus.h"
#include "trolleybus.h"
#include "tram.h"
#include "basefunction.h"

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

    auto driver1 = make_shared<Driver>("Иванов Иван Иванович", 15);
    auto driver2 = make_shared<Driver>("Петров Пётр Петрович", 8);

    auto bus = make_shared<Bus>("А123ВС", "ПАЗ-3205", 2015, 30, 80, driver1, "Дизель", 100.0);
    auto trolley = make_shared<Trolleybus>("В456ЕК", "АКСМ-321", 2018, 90, 70, driver2, 550, 45.0);
    auto tram = make_shared<Tram>("С789МН", "БКМ-843", 2020, 150, 60, driver1, 1524, 60.0);

    vector<shared_ptr<Vehicle>> vehicles = {bus, trolley, tram};

    int choice = -1;

    do {
        printMenu();
        choice = inputInt("Выберите пункт меню: ");

        switch (choice) {
            case 1: {
                cout << "\n=== УНАСЛЕДОВАННЫЕ ПОЛЯ ===\n";
                for (const auto& v : vehicles) cout << *v << endl;
                break;
            }
            case 2: {
                cout << "\n=== СПЕЦИФИЧНЫЕ ПОЛЯ ПРОИЗВОДНЫХ ТИПОВ ===\n";
                for (const auto& v : vehicles) {
                    cout << "\n--- " << v->getType() << " ---" << endl;
                    v->printInfo();   
                }
                break;
            }
            case 3: {
                cout << "\n=== УНАСЛЕДОВАННЫЙ СЕТТЕР ===\n";
                bus->setCapacity(35);
                trolley->setCapacity(95);
                tram->setCapacity(160);
                for (const auto& v : vehicles) {
                    cout << "\n--- " << v->getType() << " ---" << endl;
                    v->printInfo();   
                }   
                break;
            }
            case 4: {
                cout << "\n=== УНАСЛЕДОВАННЫЙ ГЕТТЕР (getYear) ===\n";
                for (const auto& v : vehicles) {
                    cout << v->getType() << ": " << v->getYear() << endl;
                }
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