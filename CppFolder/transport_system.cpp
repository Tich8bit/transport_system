#include "transport_system.h"
#include "route.h"
#include "vehicle.h"
#include <iostream>
#include <string_view>

bool TransportSystem::addRoute(std::shared_ptr<Route> route) {
    if (!route) {
        std::cout << "\nОШИБКА! Пустой указатель на маршрут!\n";
        return false;
    }
    for (const auto& r : _routes) {
        if (r == route) {
            std::cout << "\nОШИБКА! Маршрут №" << route->getNumber()
                      << " уже есть в системе!\n";
            return false;
        }
    }
    _routes.push_back(route);
    std::cout << "\nУСПЕХ! Маршрут №" << route->getNumber()
              << " \"" << route->getName() << "\" добавлен в систему.\n";
    return true;
}

bool TransportSystem::removeRoute(std::shared_ptr<Route> route) {
    if (!route) {
        std::cout << "\nОШИБКА! Пустой указатель на маршрут!\n";
        return false;
    }

    for (auto it = _routes.begin(); it != _routes.end(); ++it) {
        if (*it == route) {
            _routes.erase(it);
            std::cout << "\nУСПЕХ! Маршрут №" << route->getNumber()
                      << " \"" << route->getName() << "\" удалён из системы.\n";
            return true;
        }
    }

    std::cout << "\nОШИБКА! Маршрут №" << route->getNumber()
              << " не найден в системе!\n";
    return false;
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
        std::cout << *v;
        std::cout << "\n-----------------------------------------------" << std::endl;
    }
}

TransportSystem& TransportSystem::operator+=(std::shared_ptr<Route> route) {
    addRoute(route);   
    return *this;
}

TransportSystem& TransportSystem::operator-=(std::shared_ptr<Route> route) {
    removeRoute(route);   
    return *this;
}