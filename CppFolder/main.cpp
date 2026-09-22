#include <Windows.h>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <locale>
#include <sstream>
#include <string_view>

#include "driver.h"
#include "vehicle.h"
#include "route.h"
#include "transport_system.h"
#include "basefunction.h"

using namespace std;

void printMenu() {
    cout << "\n";
    cout << "_______________________________________________\n";
    cout << "|                   МЕНЮ                      |\n";
    cout << "|_____________________________________________|\n";
    cout << "|1. (<<) Показать весь транспорт              |\n";
    cout << "|_____________________________________________|\n";
    cout << "|2. (+=) Закрепить транспорт за маршрутом     |\n";
    cout << "|_____________________________________________|\n";
    cout << "|3. (-=) Открепить транспорт от маршрута      |\n";
    cout << "|_____________________________________________|\n";
    cout << "|4. (+=) Добавить ТС в систему                |\n";
    cout << "|_____________________________________________|\n";
    cout << "|5. (-=) Удалить ТС из системы                |\n";
    cout << "|_____________________________________________|\n";
    cout << "|6. (==) Сравненить транспорта по рег. номеру |\n";
    cout << "|_____________________________________________|\n";
    cout << "|7. (</<=/>/>=) Сравненить тc. по скорости    |\n";
    cout << "|_____________________________________________|\n";
    cout << "|8. Сравнение транспорта году выпуска         |\n";
    cout << "|_____________________________________________|\n";
    cout << "|0. Выход                                     |\n";
    cout << "|_____________________________________________|\n";
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    bool check;
    TransportSystem system;

    auto driver1 = make_shared<Driver>("Иванов Иван Иванович", 15);
    auto driver2 = make_shared<Driver>("Петров Пётр Петрович", 8);
    auto driver3 = make_shared<Driver>("Сидоров Сидор Сидорович", 22);

    auto bus1 = make_shared<Vehicle>("А123ВС", "ПАЗ-3205", 2015, 30, 20, driver1);
    auto bus2 = make_shared<Vehicle>("В456ЕК", "ЛиАЗ-5292", 2018, 50, 90, driver2);
    auto bus3 = make_shared<Vehicle>("С789МН", "Volgabus", 2020, 100, 110, driver3);
    auto bus4 = make_shared<Vehicle>("", "", 0, 0, 0, nullptr);
    auto bus5 = make_shared<Vehicle>("М222ОР", "БКМ-321", 2023, 80, 95, driver2);
    auto bus6 = make_shared<Vehicle>("Н333СТ", "ГАЗель-Next", 2019, 20, 85, driver1);

    system.addVehicle(bus1);
    system.addVehicle(bus2);
    system.addVehicle(bus3);

    auto route1 = make_shared<Route>(1, "Центральный", "Вокзал", "Пл. Мира", 40, 60);
    auto route2 = make_shared<Route>(2, "Экспресс", "Аэропорт", "Автовокзал", 80, 100);
    auto route3 = make_shared<Route>(3, "Ночной", "Депо", "Центр", 25, 50);

    system.addRoute(route1);
    system.addRoute(route2);
    system.addRoute(route3);

    int choice = -1;
    
    do {
        printMenu();
        choice = inputInt("Выберите пункт меню: ");

        switch (choice) {
             case 0: {
                cout << "Выход из программы. До свидания!\n";
                break;
             }
            case 1: {
                system.printAllVehicles();
                break;
            }
          case 2: {
                *route1 += bus1;
                *route1 += bus2;
                *route1 += bus3;
                route1->printRouteInformation();
                break;
            }
            case 3: {
                *route1 -= bus2;
                route1->printRouteInformation();
                break;
            }
            case 4: {
                system += bus5;
                system += bus6;
                system.printAllVehicles();
                break;
            }
            case 5: {
                system -= bus5;
                system -= bus6;
                system.printAllVehicles();
                break;
            }
            case 6: {
                cin >> *bus4;
                cout << *bus4;
                cout << *bus1;
                check = *bus1 == *bus4;
                cout << "\nРег. номер одинаковый?: " << (check ? "True" : "False") << endl;
                break;
            }
            case 7: {
                system.printAllVehicles();
                check = (*bus1 > *bus2);
                cout << "bus1 > bus2: " << (check ? "True" : "False") << endl;
                check = (*bus1 < *bus3);
                cout << "bus1 < bus3: " << (check ? "True" : "False") << endl;
                check = (*bus2 >= *bus1);
                cout << "bus2 >= bus1: " << (check ? "True" : "False") << endl;
                check = (*bus1 <= *bus3);
                cout << "bus1 <= bus3: " << (check ? "True" : "False") << endl;
                break;
            }
            case 8: {
                system.printAllVehicles();
                cout << "\n";
                cout << "bus2 новее чем bus3?: "  << (isNewer(*bus2, *bus3) ? "True" : "False") << endl;
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

//g++ CppFolder/main.cpp CppFolder/driver.cpp CppFolder/vehicle.cpp CppFolder/route.cpp CppFolder/transport_system.cpp CppFolder/basefunction.cpp -IHeaderFolder -o transport.exe