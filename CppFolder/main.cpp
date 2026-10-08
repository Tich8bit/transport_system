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
#include "collection.h"
#include "algs.h"
#include "exceptions.h"

using namespace std;

void printMenu() {
    cout << "_________________________________________\n";
    cout << "|             ЛР №5 —  Шаблоны          |\n";
    cout << "|_______________________________________|\n";
    cout << "|1. Показать размер коллекции           |\n";
    cout << "|2. Получить элемент по индексу         |\n";
    cout << "|3. Удалить по индексу                  |\n";
    cout << "|4. Удалить по критерию                 |\n";
    cout << "|5. Поиск по критерию                   |\n";
    cout << "|6. Средняя метрика                     |\n";
    cout << "|7. Сортировка по компаратору           |\n";
    cout << "|8. Вывод коллекции                     |\n";
    cout << "|_______________________________________|\n";
    cout << "|             ЛР №6 —  Исключения       |\n";
    cout << "|_______________________________________|\n";
    cout << "|9.  InvalidDataException               |\n";
    cout << "|10. LimitExceededException             |\n";
    cout << "|11. DuplicateIdException               |\n";
    cout << "|12. ObjectNotFoundException            |\n";
    cout << "|13. OutOfRangeException                |\n";
    cout << "|14. InvalidOperationException          |\n";
    cout << "|15. RelationException                  |\n";
    cout << "|_______________________________________|\n";
    cout << "|0. Выход                               |\n";
    cout << "|_______________________________________|\n";
}


Collection<Driver> buildDriverCollection(const vector<shared_ptr<Driver>>& drivers) {
    Collection<Driver> coll;
    for (const auto& d : drivers) {
        coll.add(d);
    }
    return coll;
}

Collection<Vehicle> buildVehicleCollection(const vector<shared_ptr<Vehicle>>& vehicles) {
    Collection<Vehicle> coll;
    for (const auto& v : vehicles) {
        coll.add(v);
    }
    return coll;
}

void demoGetByIndex(const Collection<Vehicle>& vehicleColl,
                    const Collection<Driver>& driverColl,
                    int vIndex, int dIndex) {
    cout << "Vehicle[" << vIndex << "]:\n";
    if (auto v = vehicleColl.get(vIndex); v) {
        cout << *v << '\n';
    }

    cout << "Driver[" << dIndex << "]:\n";
    if (auto d = driverColl.get(dIndex); d) {
        cout << *d << '\n';
    }
}

void demoFind(const Collection<Vehicle>& vehicleColl,
              const Collection<Driver>& driverColl) {
    cout << "Поиск автобуса:\n";
    if (auto busPtr = vehicleColl.find(
            [](const shared_ptr<Vehicle>& v) {
                return v->getType() == "Автобус";
            }); busPtr) {
        cout << *busPtr << '\n';
    } else {
        cout << "Не найдено\n";
    }

    cout << "\nПоиск водителя 'Петров':\n";
    if (auto driverPtr = driverColl.find(
            [](const shared_ptr<Driver>& d) {
                return d->getFullName() == "Петров Пётр Петрович";
            }); driverPtr) {
        cout << *driverPtr << '\n';
    } else {
        cout << "Не найдено\n";
    }
}

int main() {  
    setlocale(LC_ALL, "ru_RU.UTF-8");

    auto driver1 = make_shared<Driver>("Иванов Иван Иванович", 15);
    auto driver2 = make_shared<Driver>("Петров Пётр Петрович", 8);
    auto driver3 = make_shared<Driver>("Сидоров Сидор Сидорович", 12);
    auto driver4 = make_shared<Driver>("Кузнецов Кузьма Кузьмич", 5);

    vector<shared_ptr<Driver>> drivers = {driver1, driver2, driver3, driver4};
    Collection<Driver> driverColl = buildDriverCollection(drivers);

    auto bus = make_shared<Bus>("А123ВС", "ПАЗ-3205", 2015, 30, driver1, "Дизель", 100.0);
    auto trolley = make_shared<Trolleybus>("В456ЕК", "АКСМ-321", 2018, 90, driver2, 550, 45.0);
    auto tram = make_shared<Tram>("С789МН", "БКМ-843", 2020, 150, driver3, 1524, 60.0);

    vector<shared_ptr<Vehicle>> vehicles = {bus, trolley, tram};
    Collection<Vehicle> vehicleColl = buildVehicleCollection(vehicles);

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
        choice = inputInt("Выберите пункт меню: ", 0, 11);

        switch (choice) {
            case 1: {
                cout << "Vehicle:  " << vehicleColl.size() << '\n';
                cout << "Driver:   " << driverColl.size() << '\n';
                break;
            }
            case 2: {
                demoGetByIndex(vehicleColl, driverColl, 1, 2);   
                break;
            }
            case 3: {
                cout << "Vehicle: удаляем [1]\n";
                vehicleColl.removeAt(1);
                vehicleColl.print();
                cout << "\nDriver: удаляем [0]\n";
                driverColl.removeAt(0);
                driverColl.print();
                break;
            }
            case 4: {
                cout << "Удаляем все трамваи:\n";
                size_t removed1 = vehicleColl.removeIf(
                    [](const shared_ptr<Vehicle>& v) {
                        return v->getType() == "Трамвай";
                    });
                cout << "Удалено: " << removed1 << '\n';
                vehicleColl.print();
                cout << "\nУдаляем водителей со стажем < 10:\n";
                size_t removed2 = driverColl.removeIf(
                    [](const shared_ptr<Driver>& d) {
                        return d->getExperienceYears() < 10;
                    });
                cout << "Удалено: " << removed2 << '\n';
                driverColl.print();
                break;
            }
            case 5: {
            demoFind(vehicleColl, driverColl);
            break;
            }
            case 6: {
            cout << "Средняя метрика ТС:       "
                << averageMetric(vehicleColl) << '\n';
            cout << "Средняя метрика водителей: "
                << averageMetric(driverColl) << '\n';
            break;
        }
        case 7: {
            auto sortedVehicles = sortBy(vehicleColl,
                [](const shared_ptr<Vehicle>& a, const shared_ptr<Vehicle>& b) {
                    return a->calculateMetric() > b->calculateMetric();
                });
            for (const auto& v : sortedVehicles) {
                cout << v->getType() << " (" << v->getRegNumber() << ") --- "
                    << v->calculateMetric() << '\n';
            }
            auto sortedDrivers = sortBy(driverColl,
                [](const shared_ptr<Driver>& a, const shared_ptr<Driver>& b) {
                    return a->getExperienceYears() > b->getExperienceYears();
                });
            for (const auto& d : sortedDrivers) {
                cout << d->getFullName() << " --- стаж: "
                    << d->getExperienceYears() << " лет\n";
            }
            break;
        }
         case 8: {
            cout << "--- Collection<Vehicle> ---\n";
            vehicleColl.print(cout);
            cout << "\n--- Collection<Driver> ---\n";
            driverColl.print(cout);
            break;
         }
        case 9: {
            try {
                auto badBus = make_shared<Bus>("А000ХХ", "Test", -100, 30,
                                                driver1, "Test", 100.0);
            }
            catch (const InvalidDataException& e) {
                cout << "InvalidDataException: " << e.what() << '\n';
            }
            break;
        }
        case 10: {
            try {
                bus->applyEffect(100000);
            }
            catch (const LimitExceededException& e) {
                cout << "LimitExceededException: " << e.what() << '\n';
            }
            break;
        }
        case 11: {
            try {
                auto bus2 = make_shared<Bus>("А123ВС", "Test", 2020, 30,
                                            driver1, "Test", 100.0);
                system.addVehicle(bus2);
            }
            catch (const DuplicateIdException& e) {
                cout << "DuplicateIdException: " << e.what() << '\n';
            }
            break;
        }
        case 12: {
            try {
                auto ghost = make_shared<Bus>("ZZZZZZ", "Ghost", 2020, 30,
                                            driver1, "Test", 100.0);
                system.removeVehicle(ghost);
            }
            catch (const ObjectNotFoundException& e) {
                cout << "ObjectNotFoundException: " << e.what() << '\n';
            }
            break;
        }
        case 13: {
            try {
                auto v = vehicleColl.get(100);
                cout << *v << '\n';
            }
            catch (const OutOfRangeException& e) {
                cout << "OutOfRangeException: " << e.what() << '\n';
            }
            break;
        }
        case 14: {
            try {
                tram->applyEffect(-5);
            }
            catch (const InvalidOperationException& e) {
                cout << "InvalidOperationException: " << e.what() << '\n';
            }
            break;
        }
        case 15: {
            try {
                auto tinyBus = make_shared<Bus>("МАЛ777", "Малый", 2020, 5,
                                                driver4, "Газ", 50.0);
                *route1 += tinyBus;
            }
            catch (const RelationException& e) {
                cout << "RelationException: " << e.what() << '\n';
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

//g++ -std=c++20 CppFolder/*.cpp -IHeaderFolder -o transport.exe