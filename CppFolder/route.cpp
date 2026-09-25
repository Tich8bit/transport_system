#include "route.h"
#include "vehicle.h"
#include <iostream>

const double Route::SPEED_NORMAL_THRESHOLD = 0.7;
const double Route::SPEED_COMPENSATION_THRESHOLD = 1.2;

Route::Route(int number,
             std::string_view name,
             std::string_view startStop,
             std::string_view endStop,
             int minCapacity,
             int minSpeed)
    : _number(number),
      _name(name),
      _startStop(startStop),
      _endStop(endStop),
      _minCapacity(minCapacity),
      _minSpeed(minSpeed) {}

int Route::getNumber() const { return _number; }
std::string_view Route::getName() const { return _name; }
std::string_view Route::getStartStop() const { return _startStop; }
std::string_view Route::getEndStop() const { return _endStop; }
int Route::getMinCapacity() const { return _minCapacity; }
int Route::getMinSpeed() const { return _minSpeed; }

void Route::setName(std::string_view newName) { _name = newName; }
void Route::setStartStop(std::string_view newStartStop) { _startStop = newStartStop; }
void Route::setEndStop(std::string_view newEndStop) { _endStop = newEndStop; }
void Route::setMinCapacity(int newMinCapacity) { _minCapacity = newMinCapacity; }
void Route::setMinSpeed(int newSpeed) { _minSpeed = newSpeed; }
const std::vector<std::shared_ptr<Vehicle>>&
Route::getAssignedVehicles() const {
    return _assignedVehicles;
}

void Route::printRouteInformation() const {
    std::cout << "=== Маршрут №" << _number << " ===" << std::endl;
    std::cout << "Название: " << _name << std::endl;
    std::cout << "Начальная остановка: " << _startStop << std::endl;
    std::cout << "Конечная остановка: " << _endStop << std::endl;
    std::cout << "Минимальная вместимость ТС: " << _minCapacity << " чел." << std::endl;
    std::cout << "Транспорт на маршруте (" << _assignedVehicles.size() << "):" << std::endl;

    for (const auto& v : _assignedVehicles) {
        std::cout << "  - " << v->getModel() << " (" << v->getRegNumber() << ")" << std::endl;
    }
}

Route& Route::operator+=(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
        std::cout << "\nОШИБКА! Пустой указатель на транспорт!" << std::endl;
        return *this;
    }
    for (const auto& v : _assignedVehicles) {
        if (v == vehicle) {
            std::cout << "\nОШИБКА! ТС \"" << vehicle->getModel()
                      << "\" (" << vehicle->getRegNumber()
                      << ") уже закреплено за маршрутом №" << _number << "!" << std::endl;
            return *this;
        }
    }
    if ((vehicle->getCapacity() >= _minCapacity && vehicle->getMaxSpeed() >= _minSpeed * SPEED_NORMAL_THRESHOLD) ||
        (vehicle->getCapacity() < _minCapacity && vehicle->getMaxSpeed() >= _minSpeed * SPEED_COMPENSATION_THRESHOLD)) {
        _assignedVehicles.push_back(vehicle);
        return *this;
    }
    else {
        std::cout << "\nОШИБКА! ТС \"" << vehicle->getModel() << "\"" << std::endl;
        return *this;
    }
}

Route& Route::operator-=(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
        std::cout << "\nОШИБКА! Пустой указатель на транспорт!\n";
        return *this;
    }
    for (auto it = _assignedVehicles.begin(); it != _assignedVehicles.end(); ++it) {
        if (*it == vehicle) {
            _assignedVehicles.erase(it);
            return *this;
        }
    }
    std::cout << "\nОШИБКА! ТС \"" << vehicle->getModel()
              << "\" не закреплено за маршрутом №" << _number << "!" << std::endl;
    return *this;
}