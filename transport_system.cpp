#include "transport_system.h"
#include "route.h"
#include "vehicle.h"
#include <iostream>
#include <string_view>

void TransportSystem::addRoute(std::shared_ptr<Route> route) {
    if (route) {
        _routes.push_back(route);
    }
}

void TransportSystem::addVehicle(std::shared_ptr<Vehicle> vehicle) {
    if (vehicle) {
        _vehicles.push_back(vehicle);
    }
}

std::shared_ptr<Route>
TransportSystem::findRouteByNumber(int number) const {
    for (const auto& r : _routes) {
        if (r->getNumber() == number) return r;
    }
    return nullptr;
}

std::shared_ptr<Vehicle>
TransportSystem::findVehicleByRegNumber(std::string_view regNumber) const {
    for (const auto& v : _vehicles) {
        if (v->getRegNumber() == regNumber) return v;
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Route>>&
TransportSystem::getRoutes() const { return _routes; }

const std::vector<std::shared_ptr<Vehicle>>&
TransportSystem::getVehicles() const { return _vehicles; }

void TransportSystem::printAllRoutes() const {
    if (_routes.empty()) {
        std::cout << "Список маршрутов пуст." << std::endl;
        return;
    }
    for (const auto& r : _routes) 
        r->printRouteInformation();
    
}

void TransportSystem::printAllVehicles() const {
    if (_vehicles.empty()) {
        std::cout << "Список транспорта пуст." << std::endl;
        return;
    }
    std::cout << "\n=== СПИСОК ТРАНСПОРТА ===" << std::endl;
    for (const auto& v : _vehicles) {
        v->printVehicleInformation();
        std::cout << "-------------------" << std::endl;
    }
}