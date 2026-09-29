#include <iostream>
#include <memory>
#include <vector>
#include <locale>
#include "driver.h"
#include "vehicle.h"
#include "bus.h"
#include "trolleybus.h"
#include "tram.h"
#include "route.h"
#include "transport_system.h"
#include "basefunction.h"

using namespace std;

void printMenu() {
    cout << "_______________________________________________\n";
    cout << "|1. Показать все маршруты                     |\n";
    cout << "|2. (<<) Показать весь транспорт              |\n";
    cout << "|3. (+=) Закрепить транспорт за маршрутом     |\n";
    cout << "|4. (-=) Открепить транспорт от маршрута      |\n";
    cout << "|5. (+=) Добавить маршрут в систему           |\n";
    cout << "|6. (-=) Удалить маршрут из системы           |\n"; 
    cout << "|7. (==) Сравнить ТС по рег. номеру           |\n";
    cout << "|8. (<=>) Сравнить ТС по метрике              |\n";
    cout << "|             ЛР №4 — Полиморфизм             |\n";
    cout << "|9. Полиморфный подсчёт метрик               |\n";
    cout << "|10. Поиск самого результативного ТС          |\n";
    cout << "|11. Массовое обслуживание всех ТС            |\n";
    cout << "|12. Добавить новое ТС (полиморфный ввод)     |\n";
    cout << "|0. Выход                                     |\n";
    cout << "|_____________________________________________|\n";
}

void findBestVehicle(const vector<shared_ptr<Vehicle>>& vehicles) {
    if (vehicles.empty()) {
        cout << "Список пуст.\n";
        return;
    }

    auto best = vehicles[0];
    for (const auto& v : vehicles) {
        if (v->calculateMetric() > best->calculateMetric()) {
            best = v;
        }
    }

    cout << "Лучшее ТС:\n";
    cout << "  Тип: " << best->getType() << endl;
    cout << "  " << best->getMetricName() << ": "
         << best->calculateMetric() << endl;
    cout << *best << endl;
}

int main() {  
    setlocale(LC_ALL, "ru_RU.UTF-8");

    auto driver1 = make_shared<Driver>("Иванов Иван Иванович", 15);
    auto driver2 = make_shared<Driver>("Петров Пётр Петрович", 8);
    auto driver3 = make_shared<Driver>("Сидоров Сидор Сидорович", 12);
    auto driver4 = make_shared<Driver>("Кузнецов Кузьма Кузьмич", 5);

    auto bus = make_shared<Bus>("А123ВС", "ПАЗ-3205", 2015, 30, driver1, "Дизель", 100.0);
    auto trolley = make_shared<Trolleybus>("В456ЕК", "АКСМ-321", 2018, 90, driver2, 550, 45.0);
    auto tram = make_shared<Tram>("С789МН", "БКМ-843", 2020, 150, driver3, 1524, 60.0);
    auto bus1 = make_shared<Bus>("А123ВС", "ПАЗ-3205", 2015, 30, driver1, "Дизель", 100.0);

    vector<shared_ptr<Vehicle>> vehicles = {bus, trolley, tram};

    auto route1 = make_shared<Route>(1, "Центральный", "Вокзал", "Площадь Победы", 30);
    auto route2 = make_shared<Route>(2, "Северный", "Университет", "ТЦ Экспобел", 50);
    auto route3 = make_shared<Route>(3, "Южный", "Аэропорт", "ЖД Вокзал", 70);

    TransportSystem system;
    system += route1;
    system += route2;

    system.addVehicle(bus);
    system.addVehicle(tram);
    system.addVehicle(trolley);

    int choice = -1;

    do {
        printMenu();
        choice = inputInt("Выберите пункт меню: ", 0, 16);

        switch (choice) {
            case 1: {
                system.printAllRoutes();
                break;
            }
            case 2: {
                system.printAllVehicles();
                break;
            }
            case 3: {
                *route1 += bus;
                *route1 += trolley;
                *route1 += tram;
                route1->printRouteInformation();
                break;
            }
            case 4: {
                *route1 -= trolley;
                route1->printRouteInformation();
                break;
            }
            case 5: {
                system += route3;
                system.printAllRoutes();
                break;
            }
            case 6: {
                system -= route3;
                system.printAllRoutes();
                break;
            }
            case 7: {
                auto bus2 = make_shared<Bus>("А123ВС", "ЛиАЗ-5292", 2022, 110, driver4, "Газ", 250.0);
                cout << "bus == bus2: "
                    << ((*bus == *bus2) ? "True" : "False") << endl;  
                cout << "bus == tram: "
                    << ((*bus == *tram) ? "True" : "False") << endl;  
                break;
            }
            case 8: {
                bool check = (*bus > *trolley);
                cout << "bus > trolley: " << (check ? "True" : "False") << endl;  
                check = (*bus < *tram);
                cout << "bus < tram: " << (check ? "True" : "False") << endl;  
                break;
            }
            case 9: {
                for (const auto& v : vehicles) { 
                    double metric = v->calculateMetric();
                    cout << v->getType() << " (" << v->getRegNumber() << "):\n";
                    cout << "  " << v->getMetricName() << " = " << metric << endl;
                }
                break;
            }
            case 10: {
                findBestVehicle(vehicles);  
                 break; 
            }
            case 11: {
                for (const auto& v : vehicles) {
                    cout << "\n(" << v->getType() << ")" << endl;
                    v->applyEffect(10);
                }
                break;
            }
            case 12: {
                bus1->readBaseFrom();
                bus1->readFrom();
                bus1->printBaseInfo();
                cout << *bus1;
                system.addVehicle(bus1);
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

//g++ -std=c++20 CppFolder/main.cpp CppFolder/driver.cpp CppFolder/vehicle.cpp CppFolder/bus.cpp CppFolder/trolleybus.cpp CppFolder/tram.cpp CppFolder/route.cpp CppFolder/transport_system.cpp CppFolder/basefunction.cpp -IHeaderFolder -o transport.exe