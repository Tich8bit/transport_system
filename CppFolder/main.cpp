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
    cout << "|5. Изменить данные маршрута                  |\n";
    cout << "|_____________________________________________|\n";
    cout << "|6. Добавить новый транспорт                  |\n";
    cout << "|_____________________________________________|\n";
    cout << "|7. Добавить новый маршрут                    |\n";
    cout << "|_____________________________________________|\n";
    cout << "|8. Добавить нового водителя                  |\n";
    cout << "|_____________________________________________|\n";
    cout << "|0. Выход                                     |\n";
    cout << "|_____________________________________________|\n";
}

string inputString(string_view message) {
    string input;
    cout << message.data();
    getline(cin, input);
    return input;
}

unsigned int inputUnsignedInt(string_view message) {
    string input;
    int number;
    char extra;
    while (true) {
        cout << message.data();
        getline(cin, input);
        if (stringstream ss(input); ss >> number && !(ss >> extra) && number > 0) {
            return static_cast<unsigned int>(number);
        }
        cout << "Ошибка! Введите положительное целое число.\n";
    }
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
    auto bus4 = make_shared<Vehicle>("", "", 0, 0, nullptr);

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
                route2->addVehicle(bus3);
                route2->printRouteInformation();
                break;
            }
            case 4: {
                bus1->setCapacity(40);
                bus1->setDriver(driver2);
                bus1->setRegNumber("550501");
                cout << *bus1 << endl;
                break;
            }
            case 5: {
                route2->setStartStop("Петровщина");
                route2->setEndStop("Уручье");
                route2->setMinCapacity(120);
                route2->printRouteInformation();
                break;
            }
            case 6: {
                cout << "\n=== ДОБАВЛЕНИЕ НОВОГО ТС ===\n";

                string regNumber = inputString("Введите госномер: ");
                string model = inputString("Введите модель: ");
                unsigned int year = inputUnsignedInt("Введите год выпуска: ");
                unsigned int capacity = inputUnsignedInt("Введите вместимость: ");

                cout << "\n--- Ввод водителя ---\n";
                string driverName = inputString("Введите ФИО водителя: ");
                unsigned int driverExperience = inputUnsignedInt("Введите стаж (лет): ");

                auto newDriver = make_shared<Driver>(driverName, driverExperience);
                auto newVehicle = make_shared<Vehicle>(regNumber, model, year, capacity, newDriver);

                system.addVehicle(newVehicle);
                cout << "[УСПЕХ] ТС \"" << model << "\" добавлено в систему!\n";

                break;
            }
            case 7: {
                cout << "\n=== ДОБАВЛЕНИЕ НОВОГО МАРШРУТА ===\n";

                int number = inputInt("Введите номер маршрута: ");
                string name = inputString("Введите название: ");
                string startStop = inputString("Введите начальную остановку: ");
                string endStop = inputString("Введите конечную остановку: ");
                unsigned int minCapacity = inputUnsignedInt("Введите минимальную вместимость: ");

                auto newRoute = make_shared<Route>(number, name, startStop, endStop, minCapacity);
                system.addRoute(newRoute);
                cout << "[УСПЕХ] Маршрут №" << number << " \"" << name << "\" добавлен!\n";

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