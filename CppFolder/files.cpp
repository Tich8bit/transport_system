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
        for (int j = 0; j < drivers.size(); ++j) {
            if (drivers.get(j) == target) return j;
        }
        return -1;
    }

    int findVehicleIndex(const Collection<Vehicle>& vehicles, const std::shared_ptr<Vehicle>& target) {
        for (int j = 0; j < vehicles.size(); ++j) {
            if (vehicles.get(j) == target) return j;
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
    ofs << system.getVehicles().size() << "\n";
    
    saveBuses(ofs, system);
    saveTrolleybuses(ofs, system);
    saveTrams(ofs, system);
}

void StorageManager::saveBuses(std::ofstream& ofs, const TransportSystem& system) {
    const auto& vehicles = system.getVehicles();
    const auto& drivers = system.getDrivers();

    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles.get(i);
        if (auto bus = std::dynamic_pointer_cast<Bus>(v)) {
            int driverIdx = -1;
            if (bus->getDriver()) {
                driverIdx = findDriverIndex(drivers, bus->getDriver());
            }

            auto regNumber = std::string(bus->getRegNumber());
            auto model = std::string(bus->getModel());
            auto fuelType = std::string(bus->getFuelType());

            ofs << PREFIX_VEHICLE << " " << TYPE_BUS << " "
                << std::quoted(regNumber) << " " << std::quoted(model) << " "
                << bus->getYear() << " " << bus->getCapacity() << " "
                << driverIdx << " " << std::quoted(fuelType) << " "
                << bus->getFuelTankCapacity() << "\n";
        }
    }
}

void StorageManager::saveTrolleybuses(std::ofstream& ofs, const TransportSystem& system) {
    const auto& vehicles = system.getVehicles();
    const auto& drivers = system.getDrivers();

    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles.get(i);
        if (auto tb = std::dynamic_pointer_cast<Trolleybus>(v)) {
            int driverIdx = -1;
            if (tb->getDriver()) {
                driverIdx = findDriverIndex(drivers, tb->getDriver());
            }

            auto regNumber = std::string(tb->getRegNumber());
            auto model = std::string(tb->getModel());

            ofs << PREFIX_VEHICLE << " " << TYPE_TROLLEYBUS << " "
                << std::quoted(regNumber) << " " << std::quoted(model) << " "
                << tb->getYear() << " " << tb->getCapacity() << " "
                << driverIdx << " " << tb->getVoltage() << " "
                << tb->getPowerConsumption() << "\n";
        }
    }
}

void StorageManager::saveTrams(std::ofstream& ofs, const TransportSystem& system) {
    const auto& vehicles = system.getVehicles();
    const auto& drivers = system.getDrivers();

    for (size_t i = 0; i < vehicles.size(); ++i) {
        const auto& v = vehicles.get(i);
        if (auto tram = std::dynamic_pointer_cast<Tram>(v)) {
            int driverIdx = -1;
            if (tram->getDriver()) {
                driverIdx = findDriverIndex(drivers, tram->getDriver());
            }

            auto regNumber = std::string(tram->getRegNumber());
            auto model = std::string(tram->getModel());

            ofs << PREFIX_VEHICLE << " " << TYPE_TRAM << " "
                << std::quoted(regNumber) << " " << std::quoted(model) << " "
                << tram->getYear() << " " << tram->getCapacity() << " "
                << driverIdx << " " << tram->getGauge() << " "
                << tram->getPowerConsumption() << "\n";
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

    int driversCount = 0;
    ifs >> driversCount;
    for (int i = 0; i < driversCount; ++i) {
        loadDriver(ifs, system);
    }

    int vehiclesCount = 0;
    ifs >> vehiclesCount;
    for (int i = 0; i < vehiclesCount; ++i) {
        loadVehicle(ifs, system);
    }

    int routesCount = 0;
    ifs >> routesCount;
    for (int i = 0; i < routesCount; ++i) {
        loadRoute(ifs, system);
    }
}

void StorageManager::loadDriver(std::ifstream& ifs, TransportSystem& system) {
    std::string prefix;
    std::string name;
    int experience;

    if (!(ifs >> prefix >> std::quoted(name) >> experience)) {
        throw InvalidDataException("Ошибка чтения водителя");
    }
    if (prefix != PREFIX_DRIVER) throw InvalidDataException("Неверный префикс водителя: " + prefix);

    auto driver = std::make_shared<Driver>(name, experience);
    system.addDriver(driver);
}

void StorageManager::loadVehicle(std::ifstream& ifs, TransportSystem& system) {
    std::string prefix;
    std::string typeKey;

    if (!(ifs >> prefix >> typeKey)) {
        throw InvalidDataException("Ошибка чтения заголовка ТС");
    }
    if (prefix != PREFIX_VEHICLE) throw InvalidDataException("Неверный префикс ТС: " + prefix);

    if (typeKey == TYPE_BUS) {
        loadBus(ifs, system);
    } else if (typeKey == TYPE_TROLLEYBUS) {
        loadTrolleybus(ifs, system);
    } else if (typeKey == TYPE_TRAM) {
        loadTram(ifs, system);
    } else {
        throw InvalidDataException("Неизвестный тип ТС: " + typeKey);
    }
}

void StorageManager::loadBus(std::ifstream& ifs, TransportSystem& system) {
    std::string regNumber;
    std::string model;
    int year;
    int capacity;
    int driverIdx;
    std::string fuelType;
    double fuelCapacity;

    if (!(ifs >> std::quoted(regNumber) >> std::quoted(model) >> year >> capacity >> driverIdx >> std::quoted(fuelType) >> fuelCapacity)) {
        throw InvalidDataException("Ошибка чтения параметров автобуса");
    }

    std::shared_ptr<Driver> driver = nullptr;
    if (driverIdx != -1) {
        const auto& drivers = system.getDrivers();
        if (driverIdx < 0 || driverIdx >= static_cast<int>(drivers.size())) {
            throw RelationException("ТС ссылается на несуществующего водителя ID=" + std::to_string(driverIdx));
        }
        driver = drivers.get(driverIdx);
    }

    auto vehicle = std::make_shared<Bus>(regNumber, model, year, capacity, driver, fuelType, fuelCapacity);
    system.addVehicle(vehicle);
}

void StorageManager::loadTrolleybus(std::ifstream& ifs, TransportSystem& system) {
    std::string regNumber;
    std::string model;
    int year;
    int capacity;
    int driverIdx;
    int voltage;
    double consumption;

    if (!(ifs >> std::quoted(regNumber) >> std::quoted(model) >> year >> capacity >> driverIdx >> voltage >> consumption)) {
        throw InvalidDataException("Ошибка чтения параметров троллейбуса");
    }

    std::shared_ptr<Driver> driver = nullptr;
    if (driverIdx != -1) {
        const auto& drivers = system.getDrivers();
        if (driverIdx < 0 || driverIdx >= static_cast<int>(drivers.size())) {
            throw RelationException("ТС ссылается на несуществующего водителя ID=" + std::to_string(driverIdx));
        }
        driver = drivers.get(driverIdx);
    }

    auto vehicle = std::make_shared<Trolleybus>(regNumber, model, year, capacity, driver, voltage, consumption);
    system.addVehicle(vehicle);
}

void StorageManager::loadTram(std::ifstream& ifs, TransportSystem& system) {
    std::string regNumber;
    std::string model;
    int year;
    int capacity;
    int driverIdx;
    int gauge;
    double consumption;

    if (!(ifs >> std::quoted(regNumber) >> std::quoted(model) >> year >> capacity >> driverIdx >> gauge >> consumption)) {
        throw InvalidDataException("Ошибка чтения параметров трамвая");
    }

    std::shared_ptr<Driver> driver = nullptr;
    if (driverIdx != -1) {
        const auto& drivers = system.getDrivers();
        if (driverIdx < 0 || driverIdx >= static_cast<int>(drivers.size())) {
            throw RelationException("ТС ссылается на несуществующего водителя ID=" + std::to_string(driverIdx));
        }
        driver = drivers.get(driverIdx);
    }

    auto vehicle = std::make_shared<Tram>(regNumber, model, year, capacity, driver, gauge, consumption);
    system.addVehicle(vehicle);
}

void StorageManager::loadRoute(std::ifstream& ifs, TransportSystem& system) {
    std::string prefix;
    std::string name;
    std::string start;
    std::string end;
    int number;

    if (!(ifs >> prefix >> number >> std::quoted(name) >> std::quoted(start) >> std::quoted(end))) {
        throw InvalidDataException("Ошибка чтения заголовка маршрута");
    }
    if (prefix != PREFIX_ROUTE) throw InvalidDataException("Неверный префикс маршрута: " + prefix);

    int minCapacity;
    int vehiclesCount;
    if (!(ifs >> minCapacity >> vehiclesCount)) {
        throw InvalidDataException("Ошибка чтения параметров маршрута");
    }

    auto route = std::make_shared<Route>(number, name, start, end, minCapacity);
    const auto& vehicles = system.getVehicles();

    for (int k = 0; k < vehiclesCount; ++k) {
        int vIdx;
        if (!(ifs >> vIdx)) {
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