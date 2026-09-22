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

TransportSystem& TransportSystem::operator+=(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
        std::cout << "\nОШИБКА! Пустой указатель!\n";
        return *this;
    }
    for (const auto& v : _vehicles) {
        if (v == vehicle) {
            std::cout << "\nОШИБКА! ТС \"" << vehicle->getModel()
                      << "\" (" << vehicle->getRegNumber()
                      << ") уже есть в системе!\n";
            return *this;
        }
    }
    _vehicles.push_back(vehicle);
    std::cout << "\nУСПЕХ! ТС \"" << vehicle->getModel()
              << "\" добавлено в систему.\n";
    return *this;
}

TransportSystem& TransportSystem::operator-=(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
        std::cout << "\nОШИБКА! Пустой указатель!\n";
        return *this;
    }
    bool found = false;
    for (auto it = _vehicles.begin(); it != _vehicles.end(); ++it) {
        if (*it == vehicle) {
            _vehicles.erase(it);
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "\nОШИБКА! ТС \"" << vehicle->getModel()
                  << "\" не найдено в системе!\n";
        return *this;
    }
    for (auto& route : _routes) route->removeVehicle(vehicle);
    std::cout << "\nУСПЕХ! ТС \"" << vehicle->getModel()
              << "\" удалено из системы и откреплено от всех маршрутов.\n";
    return *this;
}