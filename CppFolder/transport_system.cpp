#include "transport_system.h"
#include "route.h"
#include "vehicle.h"
#include "driver.h"
#include "exceptions/exception.h"
#include <iostream>
#include <string_view>

void TransportSystem::addDriver(std::shared_ptr<Driver> driver) {
    if (!driver) throw InvalidDataException("пустой указатель");
    
    for (const auto& d : _drivers) {
        if (d->getFullName() == driver->getFullName()) {
            throw DuplicateIdException("Водитель с именем " + std::string(driver->getFullName()) + " уже есть в системе");
        }
    }
    _drivers.push_back(driver);
}

bool TransportSystem::removeDriver(std::shared_ptr<Driver> driver) {
    if (!driver) {
        throw InvalidDataException("пустой указатель на водителя");
    }
    for (auto it = _drivers.begin(); it != _drivers.end(); ++it) {
        if (*it == driver) {
            _drivers.erase(it);
            return true;
        }
    }
    throw ObjectNotFoundException("Водитель не найден в системе");
}

const std::vector<std::shared_ptr<Driver>>&
TransportSystem::getDrivers() const {
    return _drivers;
}

void TransportSystem::addVehicle(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
        throw InvalidDataException("пустой указатель на ТС");
    }
    for (const auto& v : _vehicles) {
        if (*v == *vehicle) {
            throw DuplicateIdException(
                "ТС с госномером " + std::string(vehicle->getRegNumber())
                + " уже есть в системе");
        }
    }
    _vehicles.push_back(vehicle);
}

bool TransportSystem::removeVehicle(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
        throw InvalidDataException("пустой указатель на ТС");
    }
    for (auto it = _vehicles.begin(); it != _vehicles.end(); ++it) {
        if (*it == vehicle) {
            _vehicles.erase(it);
            return true;
        }
    }
    throw ObjectNotFoundException(
        "ТС с госномером " + std::string(vehicle->getRegNumber())
        + " не найдено в системе");
}

const std::vector<std::shared_ptr<Vehicle>>&
TransportSystem::getVehicles() const { return _vehicles; }

const std::vector<std::shared_ptr<Route>>&
TransportSystem::getRoutes() const { return _routes; }

std::shared_ptr<Route>
TransportSystem::findRouteByNumber(int number) const {
    for (const auto& r : _routes) {
        if (r->getNumber() == number) return r;
    }
    throw ObjectNotFoundException(
        "Маршрут №" + std::to_string(number) + " не найден");
}

TransportSystem& TransportSystem::operator+=(std::shared_ptr<Route> route) {
    if (!route) {
        throw InvalidDataException("пустой указатель на маршрут");
    }
    for (const auto& r : _routes) {
        if (r == route || r->getNumber() == route->getNumber()) {
            throw DuplicateIdException(
                "Маршрут №" + std::to_string(route->getNumber())
                + " уже есть в системе");
        }
    }
    _routes.push_back(route);
    return *this;
}

TransportSystem& TransportSystem::operator-=(std::shared_ptr<Route> route) {
    if (!route) {
        throw InvalidDataException("пустой указатель на маршрут");
    }
    for (auto it = _routes.begin(); it != _routes.end(); ++it) {
        if (*it == route) {
            _routes.erase(it);
            return *this;
        }
    }
    throw ObjectNotFoundException(
        "Маршрут №" + std::to_string(route->getNumber())
        + " не найден в системе");
}

void TransportSystem::clear() {
    _routes.clear();
    _vehicles.clear();
    _drivers.clear();
}

std::shared_ptr<Vehicle> TransportSystem::findBestVehicle() const {
    if (_vehicles.empty()) return nullptr;
    auto best = _vehicles[0];
    for (const auto& v : _vehicles) {
        if (v->calculateMetric() > best->calculateMetric()) best = v;
    }
    return best;
}