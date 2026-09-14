#include<windows.h>
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
    cout << "|3. Показать информацию о конкретном маршруте |\n";
    cout << "|_____________________________________________|\n";
    cout << "|4. Закрепить транспорт за маршрутом          |\n";
    cout << "|_____________________________________________|\n";
    cout << "|5. Изменить данные транспорта                |\n";
    cout << "|_____________________________________________|\n";
    cout << "|6. Сменить водителя у транспорта             |\n";
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

string inputString(string_view message) {
    string input;
    cout << message.data();
    getline(cin, input);
    return input;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    TransportSystem system;

    Driver driver4("Иванов Иван Иванович", 15);
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
            case 1: {
                system.printAllRoutes();
                break;
            }
            case 2: {
                system.printAllVehicles();
                break;
            }
            case 3: {
                if (system.getRoutes().empty()) {
                    cout << "Список маршрутов пуст!\n";
                } else {
                    system.printAllRoutes();
                    int num = inputInt("Введите номер маршрута: ");
                    auto route = system.findRouteByNumber(num);
                    if (route) {
                        route->printRouteInformation();
                    } else {
                        cout << "Маршрут с таким номером не найден!\n";
                    }
                }
                break;
            }
            case 4: {
                if (system.getVehicles().empty() || system.getRoutes().empty()) {
                    cout << "Сначала добавьте транспорт и маршруты!\n";
                } else {
                    cout << "\n--- Доступный транспорт ---\n";
                    const auto& vehicles = system.getVehicles();
                    for (size_t i = 0; i < vehicles.size(); ++i) {
                        cout << i + 1 << ". ";
                        vehicles[i]->printVehicleInformation();
                    }

                    int vIdx = inputInt("Введите номер ТС из списка: ");
                    if (vIdx < 1 || vIdx > (int)vehicles.size()) {
                        cout << "Неверный выбор!\n";
                    } else {
                        cout << "\n--- Доступные маршруты ---\n";
                        const auto& routes = system.getRoutes();
                        for (size_t i = 0; i < routes.size(); ++i) {
                            cout << i + 1 << ". Маршрут №" << routes[i]->getNumber()
                                 << " \"" << routes[i]->getName()
                                 << "\" (мин. " << routes[i]->getMinCapacity() << " чел.)\n";
                        }

                        int rIdx = inputInt("Введите номер маршрута из списка: ");
                        if (rIdx < 1 || rIdx > (int)routes.size()) {
                            cout << "Неверный выбор!\n";
                        } else {
                            routes[rIdx - 1]->addVehicle(vehicles[vIdx - 1]);
                        }
                    }
                }
                break;
            }
            case 5: {
                if (system.getVehicles().empty()) {
                    cout << "Список транспорта пуст!\n";
                } else {
                    system.printAllVehicles();
                    int vIdx = inputInt("Введите номер ТС (по списку): ");
                    if (vIdx < 1 || vIdx > (int)system.getVehicles().size()) {
                        cout << "Неверный выбор!\n";
                    } else {
                        auto vehicle = system.getVehicles()[vIdx - 1];

                        cout << "1. Изменить год выпуска\n";
                        cout << "2. Изменить вместимость\n";
                        int sub = inputInt("Выбор: ");

                        if (sub == 1) {
                            int newYear = inputInt("Введите новый год: ");
                            vehicle->setYear(newYear);
                            cout << "Год изменён!\n";
                        } else if (sub == 2) {
                            int newCap = inputInt("Введите новую вместимость: ");
                            vehicle->setCapacity(newCap);
                            cout << "Вместимость изменена!\n";
                        } else {
                            cout << "Неверный выбор!\n";
                        }

                        vehicle->printVehicleInformation();
                    }
                }
                break;
            }
            case 6: {
                if (system.getVehicles().empty()) {
                    cout << "Список транспорта пуст!\n";
                } else {
                    system.printAllVehicles();
                    int vIdx = inputInt("Введите номер ТС (по списку): ");
                    if (vIdx < 1 || vIdx > (int)system.getVehicles().size()) {
                        cout << "Неверный выбор!\n";
                    } else {
                        auto vehicle = system.getVehicles()[vIdx - 1];

                        string fio = inputString("Введите ФИО нового водителя: ");
                        int exp = inputInt("Введите стаж (лет): ");
                        auto newDriver = make_shared<Driver>(fio, exp);
                        vehicle->setDriver(newDriver);
                        cout << "Водитель заменён!\n";
                        vehicle->printVehicleInformation();
                    }
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

//g++ main.cpp driver.cpp vehicle.cpp route.cpp transport_system.cpp -o transport.exe