#include "transport_system.h"
#include "route.h"
#include "vehicle.h"
#include <iostream>
#include <string_view>
#include "exceptions.h"

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

const std::vector<std::shared_ptr<Route>>&
TransportSystem::getRoutes() const { return _routes; }

const std::vector<std::shared_ptr<Vehicle>>&
TransportSystem::getVehicles() const { return _vehicles; }

void TransportSystem::printAllRoutes() const {
    if (_routes.empty()) {
        std::cout << "\nСписок маршрутов пуст." << std::endl;
        return;
    }
    std::cout << "_______________________________________________\n";
    std::cout << "|            СПИСОК МАРШРУТОВ                 |\n";
    std::cout << "|_____________________________________________|\n";
    for (const auto& r : _routes) 
        r->printRouteInformation();
}

void TransportSystem::printAllVehicles() const {
    if (_vehicles.empty()) {
        std::cout << "\nСписок транспорта пуст." << std::endl;
        return;
    }
    std::cout << "_______________________________________________\n";
    std::cout << "|            СПИСОК ТРАНСПОРТА                |\n";
    std::cout << "|_____________________________________________|\n";
    for (const auto& v : _vehicles) {
        v->printBaseInfo();
        std::cout << *v;
        std::cout << "\n-----------------------------------------------" << std::endl;
    }
}

TransportSystem& TransportSystem::operator+=(std::shared_ptr<Route> route) {
    if (!route) {
        throw InvalidDataException("пустой указатель на маршрут");
    }
    for (const auto& r : _routes) {
        if (r == route) {
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
        throw InvalidDataException("пустой указатель на ТС");
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

std::shared_ptr<Vehicle> TransportSystem::findBestVehicle() const {
    if (_vehicles.empty()) return nullptr;

    auto best = _vehicles[0];
    for (const auto& v : _vehicles) {
        if (v->calculateMetric() > best->calculateMetric()) {
            best = v;
        }
    }
    return best;
}