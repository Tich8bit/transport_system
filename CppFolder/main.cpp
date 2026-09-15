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

using namespace std;

void printMenu() {
    cout << "\n";
    cout << "_______________________________________________\n";
    cout << "|                   МЕНЮ                      |\n";
    cout << "|_____________________________________________|\n";
    cout << "|1. Показать все маршруты                     |\n";
    cout << "|_____________________________________________|\n";
    cout << "|2. Показать весь транспорт                   |\n";
    cout << "|_____________________________________________|\n";
    cout << "|3. Закрепить транспорт за маршрутом          |\n";
    cout << "|_____________________________________________|\n";
    cout << "|4. Изменить данные транспорта                |\n";
    cout << "|_____________________________________________|\n";
    cout << "|0. Выход                                     |\n";
    cout << "|_____________________________________________|\n";
}

int inputInt(string_view message) {
    string input;
    int number;
    char extra;
    while (true) {
        cout << message.data();
        getline(cin, input);
        if (stringstream ss(input); ss >> number && !(ss >> extra)) {
            return number;
        }
        cout << "Ошибка! Введите целое число.\n";
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    TransportSystem system;

    auto driver1 = make_shared<Driver>("Иванов Иван Иванович", 15);
    auto driver2 = make_shared<Driver>("Петров Пётр Петрович", 8);
    auto driver3 = make_shared<Driver>("Сидоров Сидор Сидорович", 22);

    auto bus1 = make_shared<Vehicle>("А123ВС", "ПАЗ-3205", 2015, 30, driver1);
    auto bus2 = make_shared<Vehicle>("В456ЕК", "ЛиАЗ-5292", 2018, 50, driver2);
    auto bus3 = make_shared<Vehicle>("С789МН", "Volgabus-5270", 2020, 100, driver3);

    system.addVehicle(bus1);
    system.addVehicle(bus2);
    system.addVehicle(bus3);

    auto route1 = make_shared<Route>(1, "Центральный", "Вокзал", "Пл. Мира", 40);
    auto route2 = make_shared<Route>(2, "Экспресс", "Аэропорт", "Автовокзал", 80);
    auto route3 = make_shared<Route>(3, "Ночной", "Депо", "Центр", 25);

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
                system.printAllRoutes();
                break;
            }
            case 2: {
                system.printAllVehicles();
                break;
            }
            case 3: {
                route1->addVehicle(bus3);
                route1->printRouteInformation();
                break;
            }
            case 4: {
                bus1->setCapacity(40);
                bus1->setDriver(driver2);
                bus1->printVehicleInformation();
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

//g++ CppFolder/main.cpp CppFolder/driver.cpp CppFolder/vehicle.cpp CppFolder/route.cpp CppFolder/transport_system.cpp -IHeaderFolder -o transport.exe