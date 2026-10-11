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

void printMenu() {
    cout << "_________________________________________\n";
    cout << "|             ЛР №5 — Шаблоны           |\n";
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
    cout << "|             ЛР №6 — Исключения        |\n";
    cout << "|_______________________________________|\n";
    cout << "|9.  InvalidDataException               |\n";
    cout << "|10. LimitExceededException             |\n";
    cout << "|11. DuplicateIdException               |\n";
    cout << "|12. ObjectNotFoundException            |\n";
    cout << "|13. OutOfRangeException                |\n";
    cout << "|14. InvalidOperationException          |\n";
    cout << "|15. RelationException                  |\n";
    cout << "|_______________________________________|\n";
    cout << "|             ЛР №7 — Файлы             |\n";
    cout << "|_______________________________________|\n";
    cout << "|16. Показать текущее состояние         |\n";
    cout << "|17. Сохранить в файл                   |\n";
    cout << "|18. Загрузить из файла                 |\n";
    cout << "|19. Добавить запись в журнал           |\n";
    cout << "|20. Сформировать отчёт                 |\n";
    cout << "|_______________________________________|\n";
    cout << "|             ЛР №8 — STL               |\n";
    cout << "|_______________________________________|\n";
    cout << "|21. Поиск маршрута по номеру (map)     |\n";
    cout << "|22. Уникальные госномера (set)         |\n";
    cout << "|23. Группировка ТС по типу             |\n";
    cout << "|24. Подсчёт ТС с вместимостью > 100    |\n";
    cout << "|25. Min/Max метрики ТС                 |\n";
    cout << "|26. Фильтрация ТС по критерию          |\n";
    cout << "|27. Сумма вместимостей ТС              |\n";
    cout << "|28. Сортировка по 2 критериям          |\n";
    cout << "|_______________________________________|\n";
    cout << "|0. Выход                               |\n";
    cout << "|_______________________________________|\n";
}

Collection<Driver> buildDriverCollection(
    const vector<shared_ptr<Driver>>& drivers) {
    Collection<Driver> coll;
    for (const auto& d : drivers) coll.add(d);
    return coll;
}

Collection<Vehicle> buildVehicleCollection(
    const vector<shared_ptr<Vehicle>>& vehicles) {
    Collection<Vehicle> coll;
    for (const auto& v : vehicles) coll.add(v);
    return coll;
}

void refreshCollections(const TransportSystem& system,
                         Collection<Vehicle>& vehicleColl,
                         Collection<Driver>& driverColl) {
    vehicleColl.clear();
    driverColl.clear();
    for (const auto& v : system.getVehicles()) vehicleColl.add(v);
    for (const auto& d : system.getDrivers())  driverColl.add(d);
}

void createTestSystem(TransportSystem& system,
                       Collection<Vehicle>& vehicleColl,
                       Collection<Driver>& driverColl) {
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

    refreshCollections(system, vehicleColl, driverColl);

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

void demoSize(const Collection<Vehicle>& vehicleColl,
              const Collection<Driver>& driverColl) {
    cout << "Vehicle:  " << vehicleColl.size() << '\n';
    cout << "Driver:   " << driverColl.size() << '\n';
}

void demoGetByIndex(const Collection<Vehicle>& vehicleColl,
                    const Collection<Driver>& driverColl,
                    int vIndex, int dIndex) {
    cout << "Vehicle[" << vIndex << "]:\n";
    if (auto v = vehicleColl.get(vIndex); v)
        cout << *v << '\n';
    else
        cout << "Не найдено\n";

    cout << "Driver[" << dIndex << "]:\n";
    if (auto d = driverColl.get(dIndex); d)
        cout << *d << '\n';
    else
        cout << "Не найдено\n";
}

void demoRemoveAt(Collection<Vehicle>& vehicleColl,
                  Collection<Driver>& driverColl,
                  int vIndex, int dIndex) {
    cout << "Vehicle: удаляем [" << vIndex << "]\n";
    vehicleColl.removeAt(vIndex);
    vehicleColl.print();

    cout << "\nDriver: удаляем [" << dIndex << "]\n";
    driverColl.removeAt(dIndex);
    driverColl.print();
}

void demoRemoveIf(Collection<Vehicle>& vehicleColl,
                   Collection<Driver>& driverColl) {
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

    cout << "\nПоиск водителя 'Петров Пётр Петрович':\n";
    if (auto driverPtr = driverColl.find(
            [](const shared_ptr<Driver>& d) {
                return d->getFullName() == "Петров Пётр Петрович";
            }); driverPtr) {
        cout << *driverPtr << '\n';
    } else {
        cout << "Не найдено\n";
    }
}

void demoAverage(const Collection<Vehicle>& vehicleColl,
                  const Collection<Driver>& driverColl) {
    cout << "Средняя метрика Vehicle:   "
         << averageMetric(vehicleColl) << '\n';
    cout << "Средняя метрика водителей: "
         << averageMetric(driverColl) << '\n';
}

void demoSort(const Collection<Vehicle>& vehicleColl,
              const Collection<Driver>& driverColl) {
    auto sortedVehicles = sortBy(vehicleColl,
        [](const shared_ptr<Vehicle>& a, const shared_ptr<Vehicle>& b) {
            return a->calculateMetric() > b->calculateMetric();
        });
    for (const auto& v : sortedVehicles) {
        cout << v->getType() << " (" << v->getRegNumber() << ") --- "
             << v->calculateMetric() << '\n';
    }

    cout << "\n";
    auto sortedDrivers = sortBy(driverColl,
        [](const shared_ptr<Driver>& a, const shared_ptr<Driver>& b) {
            return a->getExperienceYears() > b->getExperienceYears();
        });
    for (const auto& d : sortedDrivers) {
        cout << d->getFullName() << " --- стаж: "
             << d->getExperienceYears() << " лет\n";
    }
}

void demoPrint(const Collection<Vehicle>& vehicleColl,
               const Collection<Driver>& driverColl) {
    cout << "--- Collection<Vehicle> ---\n";
    vehicleColl.print(cout);
    cout << "\n--- Collection<Driver> ---\n";
    driverColl.print(cout);
}

void demoInvalidData() {
    cout << "=== Некорректные данные (год = -100) ===\n";
    auto badBus = make_shared<Bus>("А000ХХ", "Test", -100, 30,
                                    nullptr, "Test", 100.0);
}

void demoLimitExceeded(const TransportSystem& system) {
    cout << "=== Превышение лимита бака ===\n";
    if (const auto& vs = system.getVehicles(); !vs.empty()) {
        vs[0]->applyEffect(100000);
    }
}

void demoDuplicateId(TransportSystem& system) {
    cout << "=== Дубликат ТС ===\n";
    if (const auto& vs = system.getVehicles(); !vs.empty()) {
        system.addVehicle(vs[0]);
    }
}

void demoNotFound(TransportSystem& system) {
    cout << "=== Удаление несуществующего ТС ===\n";
    auto ghost = make_shared<Bus>("ZZZZZZ", "Ghost", 2020, 30,
                                   nullptr, "Test", 100.0);
    system.removeVehicle(ghost);
}

void demoOutOfRange(const TransportSystem& system) {
    cout << "=== Выход за границы (индекс 100) ===\n";
    Collection<Vehicle> tmp;
    for (const auto& v : system.getVehicles()) tmp.add(v);
    tmp.get(100);
}

void demoInvalidOperation(const TransportSystem& system) {
    cout << "=== Недопустимая операция (applyEffect -5) ===\n";
    if (const auto& vs = system.getVehicles(); vs.size() > 2) {
        vs[2]->applyEffect(-5);
    }
}

void demoRelation(const TransportSystem& system) {
    cout << "=== Нарушение связи ===\n";
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

void demoLoadState(const string& filename,
                    const string& logFilename,
                    TransportSystem& system,
                    Collection<Vehicle>& vehicleColl,
                    Collection<Driver>& driverColl) {
    StorageManager::loadState(filename, system);
    refreshCollections(system, vehicleColl, driverColl);
    StorageManager::logAction(logFilename, "Загрузка из " + filename);
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

    int number = inputInt("Введите номер маршрута: ", 0, 1000);

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
        regNumbers.insert(string(v->getRegNumber()));
    }

    cout << "Уникальных госномеров: " << regNumbers.size() << '\n';
    for (const auto& r : regNumbers) {
        cout << "  - " << r << '\n';
    }
}

void demoGroupByType(const TransportSystem& system) {
    cout << "=== Группировка ТС по типу ===\n";
    map<string, vector<shared_ptr<Vehicle>>> groups;

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

    auto minIt = min_element(vs.begin(), vs.end(),
        [](const shared_ptr<Vehicle>& a, const shared_ptr<Vehicle>& b) {
            return a->calculateMetric() < b->calculateMetric();
        });
    auto maxIt = max_element(vs.begin(), vs.end(),
        [](const shared_ptr<Vehicle>& a, const shared_ptr<Vehicle>& b) {
            return a->calculateMetric() < b->calculateMetric();
        });

    cout << "Min: " << (*minIt)->getRegNumber()
         << " (" << (*minIt)->calculateMetric() << ")\n";
    cout << "Max: " << (*maxIt)->getRegNumber()
         << " (" << (*maxIt)->calculateMetric() << ")\n";
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

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    string filename       = "transport.txt";
    string reportFilename = "report.txt";
    string logFilename    = "journal.log";

    TransportSystem system;
    Collection<Vehicle> vehicleColl;
    Collection<Driver>  driverColl;

    createTestSystem(system, vehicleColl, driverColl);

    int choice = -1;

    do {
        printMenu();

        try {
            choice = inputInt("Выберите пункт меню: ", 0, 28);

            switch (choice) {
                case 1:  demoSize(vehicleColl, driverColl); break;
                case 2:  demoGetByIndex(vehicleColl, driverColl, 1, 2); break;
                case 3:  demoRemoveAt(vehicleColl, driverColl, 1, 0); break;
                case 4:  demoRemoveIf(vehicleColl, driverColl); break;
                case 5:  demoFind(vehicleColl, driverColl); break;
                case 6:  demoAverage(vehicleColl, driverColl); break;
                case 7:  demoSort(vehicleColl, driverColl); break;
                case 8:  demoPrint(vehicleColl, driverColl); break;
                case 9:  demoInvalidData(); break;
                case 10: demoLimitExceeded(system); break;
                case 11: demoDuplicateId(system); break;
                case 12: demoNotFound(system); break;
                case 13: demoOutOfRange(system); break;
                case 14: demoInvalidOperation(system); break;
                case 15: demoRelation(system); break;
                case 16: printCurrentState(system); break;
                case 17: demoSaveState(filename, logFilename, system); break;
                case 18: demoLoadState(filename, logFilename, system,
                                        vehicleColl, driverColl); break;
                case 19: demoLogAction(logFilename); break;
                case 20: demoGenerateReport(reportFilename, logFilename, system); break;
                case 21: demoFindRouteByNumber(system); break;
                case 22: demoUniqueRegNumbers(system); break;
                case 23: demoGroupByType(system); break;
                case 24: demoCountByCapacity(system); break;
                case 25: demoMinMaxMetric(system); break;
                case 26: demoFilterByYear(system); break;
                case 27: demoSumCapacity(system); break;
                case 28: demoSortByTwo(system); break;
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