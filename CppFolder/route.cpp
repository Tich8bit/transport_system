#include "route.h"
#include "vehicle.h"
#include <iostream>

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
    }
    for (const auto& v : _assignedVehicles) {
        if (v == vehicle) {
            std::cout << "\nОШИБКА! ТС \"" << vehicle->getModel()
                      << "\" (" << vehicle->getRegNumber()
                      << ") уже закреплено за маршрутом №" << _number << "!" << std::endl;
        }
    }
    if ((vehicle->getCapacity() >= _minCapacity && vehicle->getMaxSpeed() >= _minSpeed * 0.7) ||
        (vehicle->getCapacity() < _minCapacity && vehicle->getMaxSpeed() >= _minSpeed * 1.2)) {
        _assignedVehicles.push_back(vehicle);
        std::cout << "\nУСПЕХ! ТС \"" << vehicle->getModel() << "\"" << std::endl;
        std::cout << "Госномер:  " << vehicle->getRegNumber() << std::endl;
        std::cout << "Маршрут №" << _number << " \"" << _name << "\"" << std::endl;
        std::cout << "Вместимость ТС: " << vehicle->getCapacity() << " чел." << std::endl;
        std::cout << "Скорость ТС: " << vehicle->getMaxSpeed() << " км/ч." << std::endl;
    }
    else {
        std::cout << "\nОШИБКА! ТС \"" << vehicle->getModel() << "\"" << std::endl;
        std::cout << "Вместимость: " << vehicle->getCapacity() << " чел." << std::endl;
        std::cout << "Скорость: " << vehicle->getMaxSpeed() << " км/ч." << std::endl;
        std::cout << "Маршрут №" << _number << " \"" << _name << "\"" << std::endl;
        std::cout << "Требуется вместимость: " << _minCapacity << " чел." << std::endl;
        std::cout << "Требуется скорость: " << _minSpeed << " км/ч." << std::endl;
    }
    return *this;
}

Route& Route::operator-=(std::shared_ptr<Vehicle> vehicle) {
        if (!vehicle) {
        std::cout << "\nОШИБКА! Пустой указатель на транспорт!\n";
    }
    for (auto it = _assignedVehicles.begin(); it != _assignedVehicles.end(); ++it) {
        if (*it == vehicle) {
            _assignedVehicles.erase(it);
            std::cout << "УСПЕХ! ТС \"" << vehicle->getModel()
                      << "\" (" << vehicle->getRegNumber() << ")" << std::endl;
            std::cout << "  Откреплено от маршрута №" << _number
                      << " \"" << _name << "\"\n";
        }
    } 
    std::cout << "\nОШИБКА! ТС \"" << vehicle->getModel()
              << "\" не закреплено за маршрутом №" << _number << "!" << std::endl;
    return *this;
}
