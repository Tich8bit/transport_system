#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <locale>
#include <algorithm>
#include <numeric>
#include <Windows.h>

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
    cout << "|7.  Очистить систему                   |\n";
    cout << "|_______________________________________|\n";
    cout << "|             ЛР №7 — Файлы             |\n";
    cout << "|_______________________________________|\n";
    cout << "|8.  Сохранить в файл                   |\n";
    cout << "|9.  Добавить запись в журнал           |\n";
    cout << "|10. Сформировать отчёт                 |\n";
    cout << "|_______________________________________|\n";
    cout << "|             ЛР №8 — STL               |\n";
    cout << "|_______________________________________|\n";
    cout << "|11. Поиск маршрута по номеру (map)     |\n";
    cout << "|12. Уникальные госномера (set)         |\n";
    cout << "|13. Группировка ТС по типу             |\n";
    cout << "|14. Подсчёт ТС с вместимостью > 100    |\n";
    cout << "|15. Min/Max метрики ТС                 |\n";
    cout << "|16. Фильтрация ТС по критерию          |\n";
    cout << "|17. Сумма вместимостей ТС              |\n";
    cout << "|18. Сортировка по 2 критериям          |\n";
    cout << "|19. Топ-2 ТС по метрике                |\n";
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
        const auto& d = drivers.get(i);
        cout << " [" << i << "] " << d->getFullName()
             << " (стаж: " << d->getExperienceYears() << " лет)\n";
    }

    cout << "Транспорт:\n";
    const auto& vehicles = system.getVehicles();
    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles.get(i);
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
        const auto& r = routes.get(i);
        cout << " [" << i << "] №" << r->getNumber()
             << " \"" << r->getName() << "\" | Закреплено ТС: " << r->getAssignedVehicles().size() << "\n";
    }

    cout << "--------------------------------\n";
}

void demoAddNewDriver(TransportSystem& system) {
    cout << "=== Добавление нового водителя ===\n";
    string name = inputString("Введите ФИО водителя: ");
    int experience = inputNumber<int>("Введите стаж (лет): ", 0, 60);

    auto newDriver = make_shared<Driver>(name, experience);
    system.addDriver(newDriver);
    cout << "Успешно добавлен водитель: " << name << "\n";
}

void demoAddNewVehicle(TransportSystem& system) {
    cout << "=== Добавление нового ТС ===\n";
    cout << "1. Автобус\n2. Троллейбус\n3. Трамвай\n";
    int type = inputNumber<int>("Выберите тип ТС: ", 1, 3);

    shared_ptr<Vehicle> newVehicle = nullptr;
    if (type == 1) newVehicle = make_shared<Bus>("A000AA", "Default", 2020, 50, nullptr, "Diesel", 100.0);
    else if (type == 2) newVehicle = make_shared<Trolleybus>("A000AA", "Default", 2020, 50, nullptr, 550, 50.0);
    else if (type == 3) newVehicle = make_shared<Tram>("A000AA", "Default", 2020, 50, nullptr, 1524, 60.0);

    newVehicle->inputVehicle();
    system.addVehicle(newVehicle);
    cout << "Успешно добавлено ТС: " << newVehicle->getType() << " " << newVehicle->getRegNumber() << "\n";
}

void demoAddNewRoute(TransportSystem& system) {
    cout << "=== Добавление нового маршрута ===\n";
    int number = inputNumber<int>("Введите номер маршрута: ", 1, 9999);
    string name = inputString("Введите название маршрута: ");
    string startStop = inputString("Введите начальную остановку: ");
    string endStop = inputString("Введите конечную остановку: ");
    int minCapacity = inputNumber<int>("Введите минимальную вместимость: ", 1, 500);

    auto newRoute = make_shared<Route>(number, name, startStop, endStop, minCapacity);
    system += newRoute;
    cout << "Успешно добавлен маршрут №" << number << " \"" << name << "\"\n";
}

void demoLinkVehicleDriver(const TransportSystem& system) {
    cout << "=== Связывание ТС с водителем ===\n";
    const auto& vehicles = system.getVehicles();
    const auto& drivers = system.getDrivers();

    if (vehicles.empty() || drivers.empty()) {
        cout << "В системе недостаточно ТС или водителей.\n";
        return;
    }

    cout << "Транспорт:\n";
    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles.get(i);
        cout << " [" << i << "] " << v->getRegNumber();
        if (v->getDriver()) cout << " (водитель: " << v->getDriver()->getFullName() << ")";
        cout << "\n";
    }
    int vIdx = inputNumber<int>("Выберите индекс ТС: ", 0, vehicles.size() - 1);

    cout << "Водители:\n";
    for (size_t i = 0; i < drivers.size(); ++i) {
        const auto& d = drivers.get(i);
        cout << " [" << i << "] " << d->getFullName() << "\n";
    }
    int dIdx = inputNumber<int>("Выберите индекс водителя: ", 0, drivers.size() - 1);

    vehicles.get(vIdx)->setDriver(drivers.get(dIdx));
    cout << "Водитель " << drivers.get(dIdx)->getFullName() 
         << " закреплен за ТС " << vehicles.get(vIdx)->getRegNumber() << ".\n";
}

void demoAssignVehicleToRoute(const TransportSystem& system) {
    cout << "=== Закрепление ТС за маршрутом ===\n";
    const auto& routes = system.getRoutes();
    const auto& vehicles = system.getVehicles();

    if (routes.empty() || vehicles.empty()) {
        cout << "В системе недостаточно маршрутов или ТС.\n";
        return;
    }

    cout << "Маршруты:\n";
    for (size_t i = 0; i < routes.size(); ++i) {
        const auto& r = routes.get(i);
        cout << " [" << i << "] №" << r->getNumber() << " \"" << r->getName() << "\"\n";
    }
    int rIdx = inputNumber<int>("Выберите индекс маршрута: ", 0, routes.size() - 1);

    cout << "Доступный транспорт:\n";
    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles.get(i);
        cout << " [" << i << "] " << v->getType() << " " << v->getRegNumber() 
             << " (Вместимость: " << v->getCapacity() << ")\n";
    }
    int vIdx = inputNumber<int>("Выберите индекс ТС: ", 0, vehicles.size() - 1);

    *routes.get(rIdx) += vehicles.get(vIdx);
    cout << "ТС " << vehicles.get(vIdx)->getRegNumber() 
         << " успешно закреплено за маршрутом №" << routes.get(rIdx)->getNumber() << ".\n";
}

void demoClearSystem(TransportSystem& system) {
    system.clear();
    cout << "Система успешно очищена.\n";
}

void demoSaveState(const string& filename, const string& logFilename, const TransportSystem& system) {
    StorageManager::saveState(filename, system);
    StorageManager::logAction(logFilename, "Сохранение в " + filename);
    cout << "Система сохранена в файл " << filename << "\n";
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
    cout << "Отчёт сформирован в файле " << reportFilename << "\n";
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
    auto filtered = TransportAnalytics::filterByYear(system.getVehicles(), 2018);
    cout << "Найдено: " << filtered.size() << '\n';
    for (const auto& v : filtered) {
        cout << "  - " << v->getType() << " " << v->getRegNumber() << '\n';
    }
}

void demoSumCapacity(const TransportSystem& system) {
    cout << "=== Сумма вместимостей ТС ===\n";
    int total = TransportAnalytics::sumCapacity(system.getVehicles());
    cout << "Общая вместимость: " << total << " чел.\n";
}

void demoSortByTwo(const TransportSystem& system) {
    cout << "=== Сортировка по 2 критериям ===\n";
    auto sorted = TransportAnalytics::sortByTwoCriteria(system.getVehicles());
    for (const auto& v : sorted) {
        cout << v->getType() << " " << v->getRegNumber()
             << " | Мест: " << v->getCapacity()
             << " | Год: " << v->getYear() << '\n';
    }
}

void demoTopN(const TransportSystem& system) {
    cout << "=== Топ-2 ТС по метрике ===\n";
    auto top = TransportAnalytics::findTopN(system.getVehicles(), 2);
    for (size_t i = 0; i < top.size(); ++i) {
        cout << "  " << (i + 1) << ". " << top[i]->getType()
             << " " << top[i]->getRegNumber()
             << " | Метрика: " << top[i]->calculateMetric()
             << " (" << top[i]->getMetricName() << ")\n";
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
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
        } catch (const TransportException& e) {
            cout << "ОШИБКА загрузки: " << e.what() << "\nБудет создана тестовая система.\n";
            createTestSystem(system);
        }
    }

    int choice = -1;

    do {
        printMenu();
        try {
            choice = inputNumber<int>("Выберите пункт меню: ", 0, 19);

            switch (choice) {
                case 1:  printCurrentState(system); break;
                case 2:  demoAddNewDriver(system); break;
                case 3:  demoAddNewVehicle(system); break;
                case 4:  demoAddNewRoute(system); break;
                case 5:  demoLinkVehicleDriver(system); break;
                case 6:  demoAssignVehicleToRoute(system); break;
                case 7:  demoClearSystem(system); break;
                case 8:  demoSaveState(filename, logFilename, system); break;
                case 9:  demoLogAction(logFilename); break;
                case 10: demoGenerateReport(reportFilename, logFilename, system); break;
                
                case 11: demoFindRouteByNumber(system); break;
                case 12: demoUniqueRegNumbers(system); break;
                case 13: demoGroupByType(system); break;
                case 14: demoCountByCapacity(system); break;
                case 15: demoMinMaxMetric(system); break;
                case 16: demoFilterByYear(system); break;
                case 17: demoSumCapacity(system); break;
                case 18: demoSortByTwo(system); break;
                case 19: demoTopN(system); break;
                
                case 0:  cout << "Выход.\n"; break;
                default: cout << "Неверный пункт.\n"; break;
            }
        }
        catch (const InvalidDataException& e) { cout << "ОШИБКА (данные): " << e.what() << '\n'; }
        catch (const LimitExceededException& e) { cout << "ОШИБКА (лимит): " << e.what() << '\n'; }
        catch (const DuplicateIdException& e) { cout << "ОШИБКА (дубликат): " << e.what() << '\n'; }
        catch (const ObjectNotFoundException& e) { cout << "ОШИБКА (не найдено): " << e.what() << '\n'; }
        catch (const OutOfRangeException& e) { cout << "ОШИБКА (границы): " << e.what() << '\n'; }
        catch (const InvalidOperationException& e) { cout << "ОШИБКА (операция): " << e.what() << '\n'; }
        catch (const RelationException& e) { cout << "ОШИБКА (связь): " << e.what() << '\n'; }
        catch (const TransportException& e) { cout << "ОШИБКА: " << e.what() << '\n'; }

    } while (choice != 0);

    return 0;
}