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

namespace {
    int findDriverIndex(const Collection<Driver>& drivers, const std::shared_ptr<Driver>& target) {
        for (size_t j = 0; j < drivers.size(); ++j) {
            if (drivers.get(j) == target) return static_cast<int>(j);
        }
        return -1;
    }

    int findVehicleIndex(const Collection<Vehicle>& vehicles, const std::shared_ptr<Vehicle>& target) {
        for (size_t j = 0; j < vehicles.size(); ++j) {
            if (vehicles.get(j) == target) return static_cast<int>(j);
        }
        return -1;
    }
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
        auto fullName = std::string(d->getFullName());
        ofs << PREFIX_DRIVER << " " << std::quoted(fullName) << " " << d->getExperienceYears() << "\n";
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
            driverIdx = findDriverIndex(drivers, v->getDriver());
        }

        std::string typeKey;
        if (std::dynamic_pointer_cast<Bus>(v)) typeKey = TYPE_BUS;
        else if (std::dynamic_pointer_cast<Trolleybus>(v)) typeKey = TYPE_TROLLEYBUS;
        else if (std::dynamic_pointer_cast<Tram>(v)) typeKey = TYPE_TRAM;

        auto regNumber = std::string(v->getRegNumber());
        auto model = std::string(v->getModel());

        ofs << PREFIX_VEHICLE << " " << typeKey << " "
            << std::quoted(regNumber) << " "
            << std::quoted(model) << " "
            << v->getYear() << " " << v->getCapacity() << " "
            << driverIdx << " ";

        if (typeKey == TYPE_BUS) {
            auto bus = std::dynamic_pointer_cast<Bus>(v);
            auto fuelType = std::string(bus->getFuelType());
            ofs << std::quoted(fuelType) << " " << bus->getFuelTankCapacity() << "\n";
        } else if (typeKey == TYPE_TROLLEYBUS) {
            auto tb = std::dynamic_pointer_cast<Trolleybus>(v);
            ofs << tb->getVoltage() << " " << tb->getPowerConsumption() << "\n";
        } else if (typeKey == TYPE_TRAM) {
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
        auto routeName = std::string(r->getName());
        auto startStop = std::string(r->getStartStop());
        auto endStop = std::string(r->getEndStop());

        ofs << PREFIX_ROUTE << " " << r->getNumber() << " "
            << std::quoted(routeName) << " "
            << std::quoted(startStop) << " "
            << std::quoted(endStop) << " "
            << r->getMinCapacity() << " "
            << r->getAssignedVehicles().size();

        for (const auto& av : r->getAssignedVehicles()) {
            int j = findVehicleIndex(vehicles, av);
            if (j != -1) ofs << " " << j;
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

    loadDrivers(ifs, system);
    loadVehicles(ifs, system);
    loadRoutes(ifs, system);
}

void StorageManager::loadDrivers(std::ifstream& ifs, TransportSystem& system) {
    std::string line;
    if (!std::getline(ifs, line)) return;
    int count = std::stoi(line);
    for (int i = 0; i < count; ++i) {
        std::getline(ifs, line);
        loadDriverLine(line, system);
    }
}

void StorageManager::loadVehicles(std::ifstream& ifs, TransportSystem& system) {
    std::string line;
    if (!std::getline(ifs, line)) return;
    int count = std::stoi(line);
    for (int i = 0; i < count; ++i) {
        std::getline(ifs, line);
        loadVehicleLine(line, system);
    }
}

void StorageManager::loadRoutes(std::ifstream& ifs, TransportSystem& system) {
    std::string line;
    if (!std::getline(ifs, line)) return;
    int count = std::stoi(line);
    for (int i = 0; i < count; ++i) {
        std::getline(ifs, line);
        loadRouteLine(line, system);
    }
}

void StorageManager::loadDriverLine(const std::string& line, TransportSystem& system) {
    std::istringstream iss(line);
    std::string prefix;
    std::string name;
    int experience;

    if (!(iss >> prefix >> std::quoted(name) >> experience)) {
        throw InvalidDataException("Ошибка чтения водителя: " + line);
    }
    if (prefix != PREFIX_DRIVER) throw InvalidDataException("Неверный префикс водителя: " + prefix);

    auto driver = std::make_shared<Driver>(name, experience);
    system.addDriver(driver);
}

void StorageManager::loadVehicleLine(const std::string& line, TransportSystem& system) {
    std::istringstream iss(line);
    std::string prefix;
    std::string typeKey;
    std::string regNumber;
    std::string model;
    int year;
    int capacity;
    int driverIdx;

    if (!(iss >> prefix >> typeKey >> std::quoted(regNumber) >> std::quoted(model) >> year >> capacity >> driverIdx)) {
        throw InvalidDataException("Ошибка чтения базовых параметров ТС: " + line);
    }
    if (prefix != PREFIX_VEHICLE) throw InvalidDataException("Неверный префикс ТС: " + prefix);

    std::shared_ptr<Driver> driver = nullptr;
    if (driverIdx != -1) {
        const auto& drivers = system.getDrivers();
        if (driverIdx < 0 || driverIdx >= static_cast<int>(drivers.size())) {
            throw RelationException("ТС ссылается на несуществующего водителя ID=" + std::to_string(driverIdx));
        }
        driver = drivers.get(driverIdx);
    }

    std::shared_ptr<Vehicle> vehicle = nullptr;

    if (typeKey == TYPE_BUS) {
        std::string fuelType;
        double fuelCapacity;
        if (!(iss >> std::quoted(fuelType) >> fuelCapacity)) {
            throw InvalidDataException("Ошибка чтения параметров автобуса: " + line);
        }
        vehicle = std::make_shared<Bus>(regNumber, model, year, capacity, driver, fuelType, fuelCapacity);
    } else if (typeKey == TYPE_TROLLEYBUS) {
        int voltage;
        double consumption;
        if (!(iss >> voltage >> consumption)) {
            throw InvalidDataException("Ошибка чтения параметров троллейбуса: " + line);
        }
        vehicle = std::make_shared<Trolleybus>(regNumber, model, year, capacity, driver, voltage, consumption);
    } else if (typeKey == TYPE_TRAM) {
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
    std::string name;
    std::string start;
    std::string end;
    int number;

    if (!(iss >> prefix >> number >> std::quoted(name) >> std::quoted(start) >> std::quoted(end))) {
        throw InvalidDataException("Ошибка чтения заголовка маршрута: " + line);
    }
    if (prefix != PREFIX_ROUTE) throw InvalidDataException("Неверный префикс маршрута: " + prefix);

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