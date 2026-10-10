#include "files.h"
#include <sstream>
#include <algorithm>
#include <ctime>
#include "driver.h"
#include "vehicle.h"
#include "bus.h"
#include "trolleybus.h"
#include "tram.h"
#include "route.h"

std::string StorageManager::trim(const std::string& str) {
    const size_t first = str.find_first_not_of(" \t\n\r");
    if (std::string::npos == first) return "";
    const size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::vector<std::string>
StorageManager::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

std::string StorageManager::replaceSpaces(const std::string& str) {
    std::string result = str;
    std::replace(result.begin(), result.end(), ' ', '_');
    return result;
}

std::string StorageManager::restoreSpaces(const std::string& str) {
    std::string result = str;
    std::replace(result.begin(), result.end(), '_', ' ');
    return result;
}

void StorageManager::saveState(const std::string& filename,
                                const TransportSystem& system) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw InvalidOperationException(
            "Не удалось открыть файл: " + filename);
    }

    const auto& drivers  = system.getDrivers();
    const auto& vehicles = system.getVehicles();
    const auto& routes   = system.getRoutes();

    ofs << drivers.size() << "\n";
    for (size_t i = 0; i < drivers.size(); ++i) {
        const auto& d = drivers[i];
        ofs << PREFIX_DRIVER << i << " "
            << replaceSpaces(std::string(d->getFullName())) << " "
            << d->getExperienceYears() << "\n";
    }

    // ===== ТС =====
    ofs << vehicles.size() << "\n";
    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles[i];

        int driverIdx = -1;
        if (v->getDriver()) {
            for (size_t j = 0; j < drivers.size(); ++j) {
                if (drivers[j] == v->getDriver()) {
                    driverIdx = static_cast<int>(j);
                    break;
                }
            }
        }

        const std::string type = v->getType();
        std::string typeKey;
        if (type == "Автобус")          typeKey = TYPE_BUS;
        else if (type == "Троллейбус")  typeKey = TYPE_TROLLEYBUS;
        else if (type == "Трамвай")     typeKey = TYPE_TRAM;

        ofs << PREFIX_VEHICLE << i << " "
            << typeKey << " "
            << v->getRegNumber() << " "
            << replaceSpaces(std::string(v->getModel())) << " "
            << v->getYear() << " "
            << v->getCapacity() << " "
            << driverIdx;

        if (typeKey == TYPE_BUS) {
            auto b = std::dynamic_pointer_cast<Bus>(v);
            ofs << " " << replaceSpaces(std::string(b->getFuelType()))
                << " " << b->getFuelTankCapacity();
        } else if (typeKey == TYPE_TROLLEYBUS) {
            auto t = std::dynamic_pointer_cast<Trolleybus>(v);
            ofs << " " << t->getVoltage()
                << " " << t->getPowerConsumption();
        } else if (typeKey == TYPE_TRAM) {
            auto t = std::dynamic_pointer_cast<Tram>(v);
            ofs << " " << t->getGauge()
                << " " << t->getPowerConsumption();
        }
        ofs << "\n";
    }

    ofs << routes.size() << "\n";
    for (size_t i = 0; i < routes.size(); ++i) {
        const auto& r = routes[i];
        ofs << PREFIX_ROUTE << i << " "
            << r->getNumber() << " "
            << replaceSpaces(std::string(r->getName())) << " "
            << replaceSpaces(std::string(r->getStartStop())) << " "
            << replaceSpaces(std::string(r->getEndStop())) << " "
            << r->getMinCapacity();

        const auto& assigned = r->getAssignedVehicles();
        ofs << " " << assigned.size();
        for (const auto& v : assigned) {
            for (size_t j = 0; j < vehicles.size(); ++j) {
                if (vehicles[j] == v) { ofs << " " << j; break; }
            }
        }
        ofs << "\n";
    }

    ofs.close();
    std::cout << "Данные сохранены в " << filename << std::endl;
}

void StorageManager::loadState(const std::string& filename,
                                TransportSystem& system) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw InvalidOperationException(
            "Не удалось открыть файл: " + filename);
    }

    system.clear();

    std::string line;
    size_t count;

    // ===== ВОДИТЕЛИ =====
    if (!(ifs >> count)) {
        throw InvalidDataException("Не удалось прочитать количество водителей");
    }
    ifs.ignore();

    std::vector<std::shared_ptr<Driver>> tempDrivers;

    for (size_t i = 0; i < count; ++i) {
        if (!std::getline(ifs, line)) {
            throw InvalidDataException("Не удалось прочитать водителя #" + std::to_string(i));
        }
        auto t = split(line, ' ');
        if (t.size() < 3) {
            throw InvalidDataException("Строка водителя неполная: " + line);
        }

        std::string name = restoreSpaces(t[1]);
        int experience = std::stoi(t[2]);

        auto d = std::make_shared<Driver>(name, experience);
        tempDrivers.push_back(d);
        system.addDriver(d);
    }

    if (!(ifs >> count)) {
        throw InvalidDataException("Не удалось прочитать количество ТС");
    }
    ifs.ignore();

    for (size_t i = 0; i < count; ++i) {
        if (!std::getline(ifs, line)) {
            throw InvalidDataException("Не удалось прочитать ТС #" + std::to_string(i));
        }
        auto t = split(line, ' ');
        if (t.size() < 7) {
            throw InvalidDataException("Строка ТС неполная: " + line);
        }

        std::string typeKey   = t[1];
        std::string regNumber = t[2];
        std::string model     = restoreSpaces(t[3]);
        int year              = std::stoi(t[4]);
        int capacity          = std::stoi(t[5]);
        int driverIdx         = std::stoi(t[6]);

        std::shared_ptr<Driver> driver = nullptr;
        if (driverIdx != -1) {
            if (driverIdx < 0 || driverIdx >= (int)tempDrivers.size()) {
                throw RelationException(
                    "ТС ссылается на несуществующего водителя ID="
                    + std::to_string(driverIdx));
            }
            driver = tempDrivers[driverIdx];
        }

        std::shared_ptr<Vehicle> v;

        if (typeKey == TYPE_BUS) {
            std::string fuelType = restoreSpaces(t[7]);
            double tank = std::stod(t[8]);
            v = std::make_shared<Bus>(regNumber, model, year, capacity,
                                       driver, fuelType, tank);
        } else if (typeKey == TYPE_TROLLEYBUS) {
            int voltage = std::stoi(t[7]);
            double power = std::stod(t[8]);
            v = std::make_shared<Trolleybus>(regNumber, model, year, capacity,
                                              driver, voltage, power);
        } else if (typeKey == TYPE_TRAM) {
            int gauge = std::stoi(t[7]);
            double power = std::stod(t[8]);
            v = std::make_shared<Tram>(regNumber, model, year, capacity,
                                        driver, gauge, power);
        } else {
            throw InvalidDataException("Неизвестный тип ТС: " + typeKey);
        }

        system.addVehicle(v);
    }

    if (!(ifs >> count)) {
        throw InvalidDataException("Не удалось прочитать количество маршрутов");
    }
    ifs.ignore();

    const auto& vehicles = system.getVehicles();

    for (size_t i = 0; i < count; ++i) {
        if (!std::getline(ifs, line)) {
            throw InvalidDataException("Не удалось прочитать маршрут #" + std::to_string(i));
        }
        auto t = split(line, ' ');
        if (t.size() < 7) {
            throw InvalidDataException("Строка маршрута неполная: " + line);
        }

        int number         = std::stoi(t[1]);
        std::string name   = restoreSpaces(t[2]);
        std::string start  = restoreSpaces(t[3]);
        std::string end    = restoreSpaces(t[4]);
        int minCapacity    = std::stoi(t[5]);
        int vehiclesCount  = std::stoi(t[6]);

        auto route = std::make_shared<Route>(number, name, start, end,
                                              minCapacity);

        for (int k = 0; k < vehiclesCount; ++k) {
            int vIdx = std::stoi(t[7 + k]);
            if (vIdx < 0 || vIdx >= (int)vehicles.size()) {
                throw RelationException(
                    "Маршрут ссылается на несуществующее ТС ID="
                    + std::to_string(vIdx));
            }
            *route += vehicles[vIdx];
        }

        system += route;
    }

    ifs.close();
    std::cout << "Данные загружены из " << filename << std::endl;
}

void StorageManager::logAction(const std::string& logFilename,
                                const std::string& action) {
    std::ofstream ofs(logFilename, std::ios::app);
    if (!ofs.is_open()) {
        throw InvalidOperationException(
            "Не удалось открыть файл журнала: " + logFilename);
    }

    std::time_t now = std::time(nullptr);
    std::tm timeInfo;
    localtime_s(&timeInfo, &now);

    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeInfo);

    ofs << "[" << buf << "] " << action << "\n";
}

void StorageManager::generateReport(const std::string& reportFilename,
                                     const TransportSystem& system) {
    std::ofstream ofs(reportFilename);
    if (!ofs.is_open()) {
        throw InvalidOperationException(
            "Не удалось создать файл отчёта: " + reportFilename);
    }

    const auto& drivers  = system.getDrivers();
    const auto& vehicles = system.getVehicles();
    const auto& routes   = system.getRoutes();

    ofs << "=== ОТЧЁТ О СОСТОЯНИИ СИСТЕМЫ ТРАНСПОРТА ===\n\n";
    ofs << "Всего водителей: " << drivers.size() << "\n";
    ofs << "Всего ТС: " << vehicles.size() << "\n";
    ofs << "Всего маршрутов: " << routes.size() << "\n\n";

    ofs << "--- ВОДИТЕЛИ ---\n";
    for (const auto& d : drivers) {
        ofs << "ФИО: " << d->getFullName()
            << " | Стаж: " << d->getExperienceYears() << " лет\n";
    }
    ofs << "\n";

    ofs << "--- ТРАНСПОРТ ---\n";
    for (const auto& v : vehicles) {
        ofs << "Тип: " << v->getType() << "\n";
        ofs << "  Госномер: " << v->getRegNumber() << "\n";
        ofs << "  Модель: " << v->getModel() << "\n";
        ofs << "  Год: " << v->getYear() << "\n";
        ofs << "  Вместимость: " << v->getCapacity() << " чел.\n";
        ofs << "  Метрика (" << v->getMetricName() << "): "
            << v->calculateMetric() << "\n";
        if (v->getDriver()) {
            ofs << "  Водитель: " << v->getDriver()->getFullName() << "\n";
        } else {
            ofs << "  Водитель: не назначен\n";
        }
        ofs << "\n";
    }

    ofs << "--- МАРШРУТЫ ---\n";
    for (const auto& r : routes) {
        ofs << "Маршрут №" << r->getNumber()
            << " \"" << r->getName() << "\"\n";
        ofs << "  От: " << r->getStartStop() << "\n";
        ofs << "  До: " << r->getEndStop() << "\n";
        ofs << "  Мин. вместимость: " << r->getMinCapacity() << "\n";

        const auto& assigned = r->getAssignedVehicles();
        ofs << "  ТС на маршруте (" << assigned.size() << "):\n";
        for (const auto& v : assigned) {
            ofs << "    - " << v->getType()
                << " " << v->getRegNumber() << "\n";
        }
        ofs << "\n";
    }

    ofs.close();
    std::cout << "Отчёт сохранён в " << reportFilename << std::endl;
}