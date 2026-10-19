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
    cout << "|             ЛР №6 — Исключения        |\n";
    cout << "|_______________________________________|\n";
    cout << "|1.  InvalidDataException               |\n";
    cout << "|2.  LimitExceededException             |\n";
    cout << "|3.  DuplicateIdException               |\n";
    cout << "|4.  ObjectNotFoundException            |\n";
    cout << "|5.  OutOfRangeException                |\n";
    cout << "|6.  InvalidOperationException          |\n";
    cout << "|7.  RelationException                  |\n";
    cout << "|_______________________________________|\n";
    cout << "|             ЛР №7 — Файлы             |\n";
    cout << "|_______________________________________|\n";
    cout << "|8.  Показать текущее состояние         |\n";
    cout << "|9.  Сохранить в файл                   |\n";
    cout << "|10. Добавить запись в журнал           |\n";
    cout << "|11. Сформировать отчёт                 |\n";
    cout << "|_______________________________________|\n";
    cout << "|             ЛР №8 — STL               |\n";
    cout << "|_______________________________________|\n";
    cout << "|12. Поиск маршрута по номеру (map)     |\n";
    cout << "|13. Уникальные госномера (set)         |\n";
    cout << "|14. Группировка ТС по типу             |\n";
    cout << "|15. Подсчёт ТС с вместимостью > 100    |\n";
    cout << "|16. Min/Max метрики ТС                 |\n";
    cout << "|17. Фильтрация ТС по критерию          |\n";
    cout << "|18. Сумма вместимостей ТС              |\n";
    cout << "|19. Сортировка по 2 критериям          |\n";
    cout << "|20. Топ-2 ТС по метрике                |\n";
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

    auto bus = make_shared<Bus>("А123ВС", "ПАЗ-3205", 2015, 30,
                                 driver1, "Дизель", 100.0);
    auto trolleybus = make_shared<Trolleybus>("В456ЕК", "АКСМ-321", 2018, 90,
                                               driver2, 550, 45.0);
    auto tram = make_shared<Tram>("С789МН", "БКМ-843", 2020, 150,
                                   driver3, 1524, 60.0);

    system.addVehicle(bus);
    system.addVehicle(trolleybus);
    system.addVehicle(tram);

    auto route1 = make_shared<Route>(1, "Центральный", "Вокзал",
                                      "Площадь Победы", 30);
    auto route2 = make_shared<Route>(2, "Северный", "Университет",
                                      "ТЦ Экспобел", 50);

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
             << " " << v->getRegNumber() << " (" << v->getModel() << ")\n";
    }

    cout << "Маршруты:\n";
    const auto& routes = system.getRoutes();
    for (size_t i = 0; i < routes.size(); ++i) {
        const auto& r = routes[i];
        cout << " [" << i << "] №" << r->getNumber()
             << " \"" << r->getName() << "\"\n";
    }

    cout << "--------------------------------\n";
}

void demoInvalidData() {
    auto badBus = make_shared<Bus>("А000ХХ", "Test", -100, 30,
                                    nullptr, "Test", 100.0);
}

void demoLimitExceeded(const TransportSystem& system) {
    if (const auto& vs = system.getVehicles(); !vs.empty()) {
        vs[0]->applyEffect(100000);
    }
}

void demoDuplicateId(TransportSystem& system) {
    if (const auto& vs = system.getVehicles(); !vs.empty()) {
        system.addVehicle(vs[0]);
    }
}

void demoNotFound(TransportSystem& system) {
    auto ghost = make_shared<Bus>("ZZZZZZ", "Ghost", 2020, 30,
                                   nullptr, "Test", 100.0);
    system.removeVehicle(ghost);
}

void demoOutOfRange(const TransportSystem& system) {
    Collection<Vehicle> tmp;
    for (const auto& v : system.getVehicles()) tmp.add(v);
    tmp.get(100);
}

void demoInvalidOperation(const TransportSystem& system) {
    if (const auto& vs = system.getVehicles(); vs.size() > 2) {
        vs[2]->applyEffect(-5);
    }
}

void demoRelation(const TransportSystem& system) {
    auto tinyBus = make_shared<Bus>("МАЛ777", "Малый", 2020, 5,
                                     nullptr, "Газ", 50.0);
    if (const auto& rs = system.getRoutes(); !rs.empty()) {
        *rs[0] += tinyBus;
    }
}

void demoSaveState(const string& filename,
                    const string& logFilename,
                    const TransportSystem& system) {
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

void demoGenerateReport(const string& reportFilename,
                         const string& logFilename,
                         const TransportSystem& system) {
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
    
    if (vs.empty()) {
        cout << "Список ТС пуст\n";
        return;
    }

    auto [minVehicle, maxVehicle] = TransportAnalytics::findMinMaxMetric(vs);

    if (minVehicle) {
        cout << "Min: " << minVehicle->getRegNumber()
             << " | Метрика: " << minVehicle->calculateMetric()
             << " (" << minVehicle->getMetricName() << ")\n";
    } else {
        cout << "Min: не найдено\n";
    }

    if (maxVehicle) {
        cout << "Max: " << maxVehicle->getRegNumber()
             << " | Метрика: " << maxVehicle->calculateMetric()
             << " (" << maxVehicle->getMetricName() << ")\n";
    } else {
        cout << "Max: не найдено\n";
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

    if (startMode == 0) {
        cout << "Выход из программы. До свидания!\n";
        return 0;
    }

    if (startMode == 1) {
        createTestSystem(system);
    } else if (startMode == 2) {
        try {
            StorageManager::loadState(filename, system);
            StorageManager::logAction(logFilename, "Загрузка из " + filename + " при запуске");
            cout << "Система успешно загружена из файла " << filename << "\n";
            cout << "  Водителей: " << system.getDrivers().size() << '\n';
            cout << "  ТС: " << system.getVehicles().size() << '\n';
            cout << "  Маршрутов: " << system.getRoutes().size() << '\n';
        } catch (const exception& e) {
            cout << "ОШИБКА загрузки: " << e.what() << "\n";
            cout << "Будет создана тестовая система.\n";
            createTestSystem(system);
        }
    }

    int choice = -1;

    do {
        printMenu();

        try {
            choice = inputNumber<int>("Выберите пункт меню: ", 0, 20);

            switch (choice) {
                case 1:  demoInvalidData(); break;
                case 2:  demoLimitExceeded(system); break;
                case 3:  demoDuplicateId(system); break;
                case 4:  demoNotFound(system); break;
                case 5:  demoOutOfRange(system); break;
                case 6:  demoInvalidOperation(system); break;
                case 7:  demoRelation(system); break;
                case 8:  printCurrentState(system); break;
                case 9:  demoSaveState(filename, logFilename, system); break;
                case 10: demoLogAction(logFilename); break;
                case 11: demoGenerateReport(reportFilename, logFilename, system); break;
                case 12: demoFindRouteByNumber(system); break;
                case 13: demoUniqueRegNumbers(system); break;
                case 14: demoGroupByType(system); break;
                case 15: demoCountByCapacity(system); break;
                case 16: demoMinMaxMetric(system); break;
                case 17: demoFilterByYear(system); break;
                case 18: demoSumCapacity(system); break;
                case 19: demoSortByTwo(system); break;
                case 20: demoTopN(system); break;
                case 0:  cout << "Выход из программы. До свидания!\n"; break;
                default: cout << "Неверный пункт меню.\n"; break;
            }
        }
        catch (const InvalidDataException& e) {
            cout << "ОШИБКА (данные): " << e.what() << '\n';
        }
        catch (const LimitExceededException& e) {
            cout << "ОШИБКА (лимит): " << e.what() << '\n';
        }
        catch (const DuplicateIdException& e) {
            cout << "ОШИБКА (дубликат): " << e.what() << '\n';
        }
        catch (const ObjectNotFoundException& e) {
            cout << "ОШИБКА (не найдено): " << e.what() << '\n';
        }
        catch (const OutOfRangeException& e) {
            cout << "ОШИБКА (границы): " << e.what() << '\n';
        }
        catch (const InvalidOperationException& e) {
            cout << "ОШИБКА (операция): " << e.what() << '\n';
        }
        catch (const RelationException& e) {
            cout << "ОШИБКА (связь): " << e.what() << '\n';
        }
        catch (const TransportException& e) {
            cout << "ОШИБКА: " << e.what() << '\n';
        }

    } while (choice != 0);

    return 0;
}