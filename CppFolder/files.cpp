#include "files.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <array>
#include <algorithm>
#include "bus.h"
#include "trolleybus.h"
#include "tram.h"
#include "exceptions/exception.h"

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

void StorageManager::logAction(const std::string& logFilename, const std::string& action) {
    std::ofstream ofs(logFilename, std::ios::app);
    if (!ofs.is_open()) {
        throw InvalidOperationException("Не удалось открыть файл журнала: " + logFilename);
    }

    std::time_t now = std::time(nullptr);
    std::tm timeInfo;
    localtime_s(&timeInfo, &now);

    std::array<char, 100> buf{};
    std::strftime(buf.data(), buf.size(), "%Y-%m-%d %H:%M:%S", &timeInfo);

    ofs << "[" << buf.data() << "] " << action << "\n";
}

void StorageManager::saveState(const std::string& filename, const TransportSystem& system) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw InvalidOperationException("Не удалось открыть файл для сохранения: " + filename);
    }

    saveDrivers(ofs, system);
    saveVehicles(ofs, system);
    saveRoutes(ofs, system);
}

void StorageManager::saveDrivers(std::ofstream& ofs, const TransportSystem& system) {
    const auto& drivers = system.getDrivers();
    ofs << drivers.size() << "\n";

    for (size_t i = 0; i < drivers.size(); ++i) {
        const auto& d = drivers.get(i);
        ofs << "DRV" << i << " "
            << replaceSpaces(std::string(d->getFullName())) << " "
            << d->getExperienceYears() << "\n";
    }
}

void StorageManager::saveVehicles(std::ofstream& ofs, const TransportSystem& system) {
    const auto& vehicles = system.getVehicles();
    const auto& drivers = system.getDrivers();
    ofs << vehicles.size() << "\n";

    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles.get(i);
        
        int driverIdx = -1;
        if (v->getDriver()) {
            for (size_t j = 0; j < drivers.size(); ++j) {
                if (drivers.get(j) == v->getDriver()) {
                    driverIdx = static_cast<int>(j);
                    break;
                }
            }
        }

        std::string typeKey;
        if (std::dynamic_pointer_cast<Bus>(v)) typeKey = "Bus";
        else if (std::dynamic_pointer_cast<Trolleybus>(v)) typeKey = "Trolleybus";
        else if (std::dynamic_pointer_cast<Tram>(v)) typeKey = "Tram";

        ofs << "VEH" << i << " " << typeKey << " "
            << std::string(v->getRegNumber()) << " "
            << std::string(v->getModel()) << " "
            << v->getYear() << " " << v->getCapacity() << " "
            << driverIdx << " ";

        if (typeKey == "Bus") {
            auto bus = std::dynamic_pointer_cast<Bus>(v);
            ofs << replaceSpaces(std::string(bus->getFuelType())) << " " << bus->getFuelTankCapacity() << "\n";
        } else if (typeKey == "Trolleybus") {
            auto tb = std::dynamic_pointer_cast<Trolleybus>(v);
            ofs << tb->getVoltage() << " " << tb->getPowerConsumption() << "\n";
        } else if (typeKey == "Tram") {
            auto tram = std::dynamic_pointer_cast<Tram>(v);
            ofs << tram->getGauge() << " " << tram->getPowerConsumption() << "\n";
        }
    }
}

void StorageManager::saveRoutes(std::ofstream& ofs, const TransportSystem& system) {
    const auto& routes = system.getRoutes();
    const auto& vehicles = system.getVehicles();
    ofs << routes.size() << "\n";

    for (size_t i = 0; i < routes.size(); ++i) {
        const auto& r = routes.get(i);
        ofs << "RTE" << i << " " << r->getNumber() << " "
            << replaceSpaces(std::string(r->getName())) << " "
            << replaceSpaces(std::string(r->getStartStop())) << " "
            << replaceSpaces(std::string(r->getEndStop())) << " "
            << r->getMinCapacity() << " "
            << r->getAssignedVehicles().size();

        for (const auto& av : r->getAssignedVehicles()) {
            for (size_t j = 0; j < vehicles.size(); ++j) {
                if (vehicles.get(j) == av) {
                    ofs << " " << j;
                    break;
                }
            }
        }
        ofs << "\n";
    }
}

void StorageManager::loadState(const std::string& filename, TransportSystem& system) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw InvalidOperationException("Не удалось открыть файл для загрузки: " + filename);
    }

    system.clear();
    std::string line;

    if (!std::getline(ifs, line)) return;
    int driversCount = std::stoi(line);
    for (int i = 0; i < driversCount; ++i) {
        std::getline(ifs, line);
        loadDriverLine(line, system);
    }

    if (!std::getline(ifs, line)) return;
    int vehiclesCount = std::stoi(line);
    for (int i = 0; i < vehiclesCount; ++i) {
        std::getline(ifs, line);
        loadVehicleLine(line, system);
    }

    if (!std::getline(ifs, line)) return;
    int routesCount = std::stoi(line);
    for (int i = 0; i < routesCount; ++i) {
        std::getline(ifs, line);
        loadRouteLine(line, system);
    }
}

void StorageManager::loadDriverLine(const std::string& line, TransportSystem& system) {
    std::istringstream iss(line);
    std::string prefix;
    std::string rawName;
    int experience;

    if (!(iss >> prefix >> rawName >> experience)) {
        throw InvalidDataException("Ошибка чтения водителя: " + line);
    }

    auto driver = std::make_shared<Driver>(restoreSpaces(rawName), experience);
    system.addDriver(driver);
}

void StorageManager::loadVehicleLine(const std::string& line, TransportSystem& system) {
    std::istringstream iss(line);
    std::string prefix;
    std::string typeKey;
    std::string regNumber;
    std::string rawModel;
    int year;
    int capacity;
    int driverIdx;

    if (!(iss >> prefix >> typeKey >> regNumber >> rawModel >> year >> capacity >> driverIdx)) {
        throw InvalidDataException("Ошибка чтения базовых параметров ТС: " + line);
    }

    std::string model = restoreSpaces(rawModel);
    std::shared_ptr<Driver> driver = nullptr;
    
    if (driverIdx != -1) {
        const auto& drivers = system.getDrivers();
        if (driverIdx < 0 || driverIdx >= static_cast<int>(drivers.size())) {
            throw RelationException("ТС ссылается на несуществующего водителя ID=" + std::to_string(driverIdx));
        }
        driver = drivers.get(driverIdx);
    }

    std::shared_ptr<Vehicle> vehicle = nullptr;

    if (typeKey == "Bus") {
        std::string rawFuelType;
        double fuelCapacity;
        if (!(iss >> rawFuelType >> fuelCapacity)) {
            throw InvalidDataException("Ошибка чтения параметров автобуса: " + line);
        }
        vehicle = std::make_shared<Bus>(regNumber, model, year, capacity, driver, restoreSpaces(rawFuelType), fuelCapacity);
    } else if (typeKey == "Trolleybus") {
        int voltage;
        double consumption;
        if (!(iss >> voltage >> consumption)) {
            throw InvalidDataException("Ошибка чтения параметров троллейбуса: " + line);
        }
        vehicle = std::make_shared<Trolleybus>(regNumber, model, year, capacity, driver, voltage, consumption);
    } else if (typeKey == "Tram") {
        int gauge;
        double consumption;
        if (!(iss >> gauge >> consumption)) {
            throw InvalidDataException("Ошибка чтения параметров трамвая: " + line);
        }
        vehicle = std::make_shared<Tram>(regNumber, model, year, capacity, driver, gauge, consumption);
    } else {
        throw InvalidDataException("Неизвестный тип ТС: " + typeKey);
    }

    system.addVehicle(vehicle);
}

void StorageManager::loadRouteLine(const std::string& line, TransportSystem& system) {
    std::istringstream iss(line);
    std::string prefix;
    std::string rawName;
    std::string rawStart;
    std::string rawEnd;
    int number;

    if (!(iss >> prefix >> number)) {
        throw InvalidDataException("Ошибка чтения заголовка маршрута: " + line);
    }

    iss >> rawName >> rawStart >> rawEnd;
    std::string name  = restoreSpaces(rawName);
    std::string start = restoreSpaces(rawStart);
    std::string end   = restoreSpaces(rawEnd);

    int minCapacity;
    int vehiclesCount;
    
    if (!(iss >> minCapacity >> vehiclesCount)) {
        throw InvalidDataException("Ошибка чтения параметров маршрута: " + line);
    }

    auto route = std::make_shared<Route>(number, name, start, end, minCapacity);
    const auto& vehicles = system.getVehicles();

    for (int k = 0; k < vehiclesCount; ++k) {
        int vIdx;
        if (!(iss >> vIdx)) {
            throw InvalidDataException("В файле указано ТС, но индекс отсутствует или поврежден");
        }

        if (vIdx < 0 || vIdx >= static_cast<int>(vehicles.size())) {
            throw RelationException("Маршрут ссылается на несуществующее ТС ID=" + std::to_string(vIdx));
        }
        *route += vehicles.get(vIdx);
    }

    system += route;
}

void StorageManager::generateReport(const std::string& reportFilename, const TransportSystem& system) {
    std::ofstream ofs(reportFilename);
    if (!ofs.is_open()) {
        throw InvalidOperationException("Не удалось открыть файл для отчёта: " + reportFilename);
    }

    ofs << "ОТЧЁТ О СОСТОЯНИИ ТРАНСПОРТНОЙ СИСТЕМЫ\n";
    ofs << "=====================================\n\n";

    reportDrivers(ofs, system.getDrivers());
    reportVehicles(ofs, system.getVehicles());
    reportRoutes(ofs, system.getRoutes());
}

void StorageManager::reportDrivers(std::ofstream& ofs, const Collection<Driver>& drivers) {
    ofs << "ВОДИТЕЛИ (" << drivers.size() << "):\n";
    for (const auto& d : drivers) {
        ofs << "  - " << d->getFullName() << " (стаж: " << d->getExperienceYears() << " лет)\n";
    }
    ofs << "\n";
}

void StorageManager::reportVehicles(std::ofstream& ofs, const Collection<Vehicle>& vehicles) {
    ofs << "ТРАНСПОРТ (" << vehicles.size() << "):\n";
    for (const auto& v : vehicles) {
        ofs << "  - " << v->getType() << " " << v->getRegNumber() << " (" << v->getModel() << ")";
        if (v->getDriver()) {
            ofs << " | Водитель: " << v->getDriver()->getFullName();
        } else {
            ofs << " | Водитель: не назначен";
        }
        ofs << "\n";
    }
    ofs << "\n";
}

void StorageManager::reportRoutes(std::ofstream& ofs, const Collection<Route>& routes) {
    ofs << "МАРШРУТЫ (" << routes.size() << "):\n";
    for (const auto& r : routes) {
        ofs << "  - №" << r->getNumber() << " \"" << r->getName() << "\"";
        ofs << " | Закреплено ТС: " << r->getAssignedVehicles().size() << "\n";
    }
    ofs << "\n";
}