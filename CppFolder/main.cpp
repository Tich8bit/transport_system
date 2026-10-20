#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <locale>
#include <algorithm>
#include <numeric>
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
#include "exceptions/exception.h"
#include "files.h"
#include "analytics.h"

using namespace std;

int chooseStartMode() {
    cout << "_________________________________________\n";
    cout << "|         Выберите режим запуска        |\n";
    cout << "|_______________________________________|\n";
    cout << "|1. Создать тестовую систему            |\n";
    cout << "|2. Загрузить систему из файла          |\n";
    cout << "|0. Выход                               |\n";
    cout << "|_______________________________________|\n";

    int choice = -1;
    while (choice < 0 || choice > 2) {
        choice = inputNumber<int>("Ваш выбор: ", 0, 2);
        if (choice < 0 || choice > 2) {
            cout << "Неверный пункт. Попробуйте снова.\n";
        }
    }
    return choice;
}

void printMenu() {
    cout << "_________________________________________\n";
    cout << "|          Управление системой          |\n";
    cout << "|_______________________________________|\n";
    cout << "|1.  Показать текущее состояние         |\n";
    cout << "|2.  Добавить нового водителя           |\n";
    cout << "|3.  Добавить новое ТС                  |\n";
    cout << "|4.  Добавить новый маршрут             |\n";
    cout << "|5.  Связать ТС с водителем             |\n";
    cout << "|6.  Закрепить ТС за маршрутом          |\n";
    cout << "|_______________________________________|\n";
    cout << "|             ЛР №7 — Файлы             |\n";
    cout << "|_______________________________________|\n";
    cout << "|7.  Сохранить в файл                   |\n";
    cout << "|8.  Добавить запись в журнал           |\n";
    cout << "|9.  Сформировать отчёт                 |\n";
    cout << "|_______________________________________|\n";
    cout << "|             ЛР №8 — STL               |\n";
    cout << "|_______________________________________|\n";
    cout << "|10. Поиск маршрута по номеру (map)     |\n";
    cout << "|11. Уникальные госномера (set)         |\n";
    cout << "|12. Группировка ТС по типу             |\n";
    cout << "|13. Подсчёт ТС с вместимостью > 100    |\n";
    cout << "|14. Min/Max метрики ТС                 |\n";
    cout << "|15. Фильтрация ТС по критерию          |\n";
    cout << "|16. Сумма вместимостей ТС              |\n";
    cout << "|17. Сортировка по 2 критериям          |\n";
    cout << "|18. Топ-2 ТС по метрике                |\n";
    cout << "|_______________________________________|\n";
    cout << "|0. Выход                               |\n";
    cout << "|_______________________________________|\n";
}

void createTestSystem(TransportSystem& system) {
    system.clear();

    auto driver1 = make_shared<Driver>("Иванов Иван Иванович", 15);
    auto driver2 = make_shared<Driver>("Петров Пётр Петрович", 8);
    auto driver3 = make_shared<Driver>("Сидоров Сидор Сидорович", 12);
    auto driver4 = make_shared<Driver>("Кузнецов Кузьма Кузьмич", 5);

    system.addDriver(driver1);
    system.addDriver(driver2);
    system.addDriver(driver3);
    system.addDriver(driver4);

    auto bus = make_shared<Bus>("А123ВС", "ПАЗ-3205", 2015, 30, driver1, "Дизель", 100.0);
    auto trolleybus = make_shared<Trolleybus>("В456ЕК", "АКСМ-321", 2018, 90, driver2, 550, 45.0);
    auto tram = make_shared<Tram>("С789МН", "БКМ-843", 2020, 150, driver3, 1524, 60.0);

    system.addVehicle(bus);
    system.addVehicle(trolleybus);
    system.addVehicle(tram);

    auto route1 = make_shared<Route>(1, "Центральный", "Вокзал", "Площадь Победы", 30);
    auto route2 = make_shared<Route>(2, "Северный", "Университет", "ТЦ Экспобел", 50);

    *route1 += bus;
    *route1 += tram;
    *route2 += trolleybus;

    system += route1;
    system += route2;

    cout << "Тестовая система создана:\n";
    cout << "  Водителей: " << system.getDrivers().size() << '\n';
    cout << "  ТС: " << system.getVehicles().size() << '\n';
    cout << "  Маршрутов: " << system.getRoutes().size() << '\n';
}

void printCurrentState(const TransportSystem& system) {
    cout << "\n--- ТЕКУЩЕЕ СОСТОЯНИЕ СИСТЕМЫ ---\n";

    cout << "Водители:\n";
    const auto& drivers = system.getDrivers();
    for (size_t i = 0; i < drivers.size(); ++i) {
        const auto& d = drivers[i];
        cout << " [" << i << "] " << d->getFullName()
             << " (стаж: " << d->getExperienceYears() << " лет)\n";
    }

    cout << "Транспорт:\n";
    const auto& vehicles = system.getVehicles();
    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles[i];
        cout << " [" << i << "] " << v->getType()
             << " " << v->getRegNumber() << " (" << v->getModel() << ")";
        if (v->getDriver()) {
            cout << " | Водитель: " << v->getDriver()->getFullName();
        } else {
            cout << " | Водитель: не назначен";
        }
        cout << "\n";
    }

    cout << "Маршруты:\n";
    const auto& routes = system.getRoutes();
    for (size_t i = 0; i < routes.size(); ++i) {
        const auto& r = routes[i];
        cout << " [" << i << "] №" << r->getNumber()
             << " \"" << r->getName() << "\" | Закреплено ТС: " << r->getAssignedVehicles().size() << "\n";
    }

    cout << "--------------------------------\n";
}

void demoAddNewDriver(TransportSystem& system) {
    auto newDriver = make_shared<Driver>("Смирнов Алексей Владимирович", 20);
    system.addDriver(newDriver);
    cout << "Успешно добавлен водитель: " << newDriver->getFullName() << "\n";
}

void demoAddNewVehicle(TransportSystem& system) {
    auto newVehicle = make_shared<Bus>("Е777КХ", "ЛиАЗ-5292", 2021, 110, nullptr, "Газ", 150.0);
    system.addVehicle(newVehicle);
    cout << "Успешно добавлено ТС: " << newVehicle->getType() << " " << newVehicle->getRegNumber() << "\n";
}

void demoAddNewRoute(TransportSystem& system) {
    auto newRoute = make_shared<Route>(3, "Восточный", "Парк Челюскинцев", "Шабаны", 40);
    system += newRoute;
    cout << "Успешно добавлен маршрут №" << newRoute->getNumber() << " \"" << newRoute->getName() << "\"\n";
}

void demoLinkVehicleDriver(TransportSystem& system) {
    cout << "=== Связывание ТС с водителем ===\n";
    const auto& vehicles = system.getVehicles();
    const auto& drivers = system.getDrivers();

    if (vehicles.empty() || drivers.empty()) {
        cout << "В системе недостаточно ТС или водителей.\n";
        return;
    }

    cout << "Транспорт:\n";
    for (size_t i = 0; i < vehicles.size(); ++i) {
        cout << " [" << i << "] " << vehicles[i]->getRegNumber();
        if (vehicles[i]->getDriver()) cout << " (водитель: " << vehicles[i]->getDriver()->getFullName() << ")";
        cout << "\n";
    }
    int vIdx = inputNumber<int>("Выберите индекс ТС: ", 0, vehicles.size() - 1);

    cout << "Водители:\n";
    for (size_t i = 0; i < drivers.size(); ++i) {
        cout << " [" << i << "] " << drivers[i]->getFullName() << "\n";
    }
    int dIdx = inputNumber<int>("Выберите индекс водителя: ", 0, drivers.size() - 1);

    vehicles[vIdx]->setDriver(drivers[dIdx]);
    cout << "Водитель " << drivers[dIdx]->getFullName() 
         << " закреплен за ТС " << vehicles[vIdx]->getRegNumber() << ".\n";
}

void demoAssignVehicleToRoute(TransportSystem& system) {
    cout << "=== Закрепление ТС за маршрутом ===\n";
    const auto& routes = system.getRoutes();
    const auto& vehicles = system.getVehicles();

    if (routes.empty() || vehicles.empty()) {
        cout << "В системе недостаточно маршрутов или ТС.\n";
        return;
    }

    cout << "Маршруты:\n";
    for (size_t i = 0; i < routes.size(); ++i) {
        cout << " [" << i << "] №" << routes[i]->getNumber() << " \"" << routes[i]->getName() << "\"\n";
    }
    int rIdx = inputNumber<int>("Выберите индекс маршрута: ", 0, routes.size() - 1);
    routes[rIdx]->printRouteInformation();
    cout << "Доступный транспорт:\n";
    for (size_t i = 0; i < vehicles.size(); ++i) {
        cout << " [" << i << "] " << vehicles[i]->getType() << " " << vehicles[i]->getRegNumber() 
             << " (Вместимость: " << vehicles[i]->getCapacity() << ")\n";
    }
    int vIdx = inputNumber<int>("Выберите индекс ТС: ", 0, vehicles.size() - 1);

    try {
        *routes[rIdx] += vehicles[vIdx];
        cout << "ТС " << vehicles[vIdx]->getRegNumber() 
             << " успешно закреплено за маршрутом №" << routes[rIdx]->getNumber() << ".\n";
    } catch (const exception& e) {
        cout << "ОШИБКА: " << e.what() << "\n";
    }
}

void demoSaveState(const string& filename, const string& logFilename, const TransportSystem& system) {
    StorageManager::saveState(filename, system);
    StorageManager::logAction(logFilename, "Сохранение в " + filename);
}

void demoLogAction(const string& logFilename) {
    string action = inputString("Введите текст для журнала: ");
    if (action.empty()) {
        cout << "Пустая строка — запись отменена\n";
        return;
    }
    StorageManager::logAction(logFilename, action);
    cout << "Запись добавлена\n";
}

void demoGenerateReport(const string& reportFilename, const string& logFilename, const TransportSystem& system) {
    StorageManager::generateReport(reportFilename, system);
    StorageManager::logAction(logFilename, "Отчёт " + reportFilename);
}

void demoFindRouteByNumber(const TransportSystem& system) {
    cout << "=== Поиск маршрута по номеру (map) ===\n";
    map<int, shared_ptr<Route>> routesMap;
    for (const auto& r : system.getRoutes()) {
        routesMap[r->getNumber()] = r;
    }

    int number = inputNumber<int>("Введите номер маршрута: ", 0, 1000);

    if (auto it = routesMap.find(number); it != routesMap.end()) {
        cout << "Найден: " << it->second->getName() << '\n';
    } else {
        cout << "Маршрут не найден\n";
    }
}

void demoUniqueRegNumbers(const TransportSystem& system) {
    cout << "=== Уникальные госномера (set) ===\n";
    set<string, less<>> regNumbers;

    for (const auto& v : system.getVehicles()) {
        regNumbers.emplace(v->getRegNumber());
    }

    cout << "Уникальных госномеров: " << regNumbers.size() << '\n';
    for (const auto& r : regNumbers) {
        cout << "  - " << r << '\n';
    }
}

void demoGroupByType(const TransportSystem& system) {
    cout << "=== Группировка ТС по типу ===\n";
    map<string, vector<shared_ptr<Vehicle>>, less<>> groups;

    for_each(system.getVehicles().begin(), system.getVehicles().end(),
        [&groups](const shared_ptr<Vehicle>& v) {
            groups[v->getType()].push_back(v);
        });

    for (const auto& [type, list] : groups) {
        cout << type << ": " << list.size() << " шт.\n";
        for (const auto& v : list) {
            cout << "    - " << v->getRegNumber() << '\n';
        }
    }
}

void demoCountByCapacity(const TransportSystem& system) {
    cout << "=== Подсчёт ТС с вместимостью > 100 ===\n";
    size_t count = count_if(system.getVehicles().begin(),
                             system.getVehicles().end(),
        [](const shared_ptr<Vehicle>& v) {
            return v->getCapacity() > 100;
        });
    cout << "Найдено: " << count << '\n';
}

void demoMinMaxMetric(const TransportSystem& system) {
    cout << "=== Min/Max метрики ТС ===\n";
    const auto& vs = system.getVehicles();
    if (vs.empty()) { cout << "Список пуст\n"; return; }

    auto [minVehicle, maxVehicle] = TransportAnalytics::findMinMaxMetric(vs);

    if (minVehicle) {
        cout << "Min: " << minVehicle->getRegNumber()
             << " | Метрика: " << minVehicle->calculateMetric()
             << " (" << minVehicle->getMetricName() << ")\n";
    }
    if (maxVehicle) {
        cout << "Max: " << maxVehicle->getRegNumber()
             << " | Метрика: " << maxVehicle->calculateMetric()
             << " (" << maxVehicle->getMetricName() << ")\n";
    }
}

void demoFilterByYear(const TransportSystem& system) {
    cout << "=== Фильтрация ТС по критерию (год >= 2018) ===\n";
    vector<shared_ptr<Vehicle>> filtered;

    copy_if(system.getVehicles().begin(), system.getVehicles().end(),
            back_inserter(filtered),
        [](const shared_ptr<Vehicle>& v) {
            return v->getYear() >= 2018;
        });

    cout << "Найдено: " << filtered.size() << '\n';
    for (const auto& v : filtered) {
        cout << "  - " << v->getType() << " " << v->getRegNumber() << '\n';
    }
}

void demoSumCapacity(const TransportSystem& system) {
    cout << "=== Сумма вместимостей ТС ===\n";
    int totalCapacity = accumulate(system.getVehicles().begin(),
                                    system.getVehicles().end(), 0,
        [](int acc, const shared_ptr<Vehicle>& v) {
            return acc + v->getCapacity();
        });
    cout << "Общая вместимость: " << totalCapacity << " чел.\n";
}

void demoSortByTwo(const TransportSystem& system) {
    cout << "=== Сортировка по 2 критериям ===\n";
    auto sorted = system.getVehicles();

    sort(sorted.begin(), sorted.end(),
        [](const shared_ptr<Vehicle>& a, const shared_ptr<Vehicle>& b) {
            if (a->getCapacity() != b->getCapacity()) {
                return a->getCapacity() > b->getCapacity();
            }
            return a->getYear() > b->getYear();
        });

    for (const auto& v : sorted) {
        cout << v->getType() << " " << v->getRegNumber()
             << " | Мест: " << v->getCapacity()
             << " | Год: " << v->getYear() << '\n';
    }
}

void demoTopN(const TransportSystem& system) {
    cout << "=== Топ-2 ТС по метрике ===\n";
    const auto& vs = system.getVehicles();
    if (vs.empty()) { cout << "Список пуст\n"; return; }

    auto top = TransportAnalytics::findTopN(vs, 2);

    for (size_t i = 0; i < top.size(); ++i) {
        cout << "  " << (i + 1) << ". " << top[i]->getType()
             << " " << top[i]->getRegNumber()
             << " | Метрика: " << top[i]->calculateMetric()
             << " (" << top[i]->getMetricName() << ")\n";
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    string filename       = "transport.txt";
    string reportFilename = "report.txt";
    string logFilename    = "journal.log";

    TransportSystem system;

    int startMode = chooseStartMode();
    if (startMode == 0) return 0;
    
    if (startMode == 1) {
        createTestSystem(system);
    } else {
        try {
            StorageManager::loadState(filename, system);
            cout << "Система загружена из " << filename << "\n";
        } catch (const exception& e) {
            cout << "ОШИБКА загрузки: " << e.what() << "\nБудет создана тестовая система.\n";
            createTestSystem(system);
        }
    }

    int choice = -1;

    do {
        printMenu();
        try {
            choice = inputNumber<int>("Выберите пункт меню: ", 0, 18);

            switch (choice) {
                case 1:  printCurrentState(system); break;
                case 2:  demoAddNewDriver(system); break;
                case 3:  demoAddNewVehicle(system); break;
                case 4:  demoAddNewRoute(system); break;
                case 5:  demoLinkVehicleDriver(system); break;
                case 6:  demoAssignVehicleToRoute(system); break;
                
                case 7:  demoSaveState(filename, logFilename, system); break;
                case 8:  demoLogAction(logFilename); break;
                case 9:  demoGenerateReport(reportFilename, logFilename, system); break;
                
                case 10: demoFindRouteByNumber(system); break;
                case 11: demoUniqueRegNumbers(system); break;
                case 12: demoGroupByType(system); break;
                case 13: demoCountByCapacity(system); break;
                case 14: demoMinMaxMetric(system); break;
                case 15: demoFilterByYear(system); break;
                case 16: demoSumCapacity(system); break;
                case 17: demoSortByTwo(system); break;
                case 18: demoTopN(system); break;
                
                case 0:  cout << "Выход.\n"; break;
                default: cout << "Неверный пункт.\n"; break;
            }
        }
        catch (const InvalidDataException& e) { cout << "ОШИБКА! " << e.what() << '\n'; }
        catch (const LimitExceededException& e) { cout << "ОШИБКА! " << e.what() << '\n'; }
        catch (const DuplicateIdException& e) { cout << "ОШИБКА! " << e.what() << '\n'; }
        catch (const ObjectNotFoundException& e) { cout << "ОШИБКА! " << e.what() << '\n'; }
        catch (const OutOfRangeException& e) { cout << "ОШИБКА (границы): " << e.what() << '\n'; }
        catch (const InvalidOperationException& e) { cout << "ОШИБКА! " << e.what() << '\n'; }
        catch (const RelationException& e) { cout << "ОШИБКА! " << e.what() << '\n'; }
        catch (const TransportException& e) { cout << "ОШИБКА! " << e.what() << '\n'; }

    } while (choice != 0);

    return 0;
}