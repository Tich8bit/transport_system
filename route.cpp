#include "route.h"
#include "vehicle.h"
#include <iostream>

Route::Route(int number, std::string_view name, std::string_view startStop, std::string_view endStop, int minCapacity) 
: _number(number), _name(name), _startStop(startStop),
      _endStop(endStop),
      _minCapacity(minCapacity) {}

int Route::getNumber() const { return _number; }
std::string_view Route::getName() const { return _name; }
std::string_view Route::getStartStop() const { return _startStop; }
std::string_view Route::getEndStop() const { return _endStop; }
int Route::getMinCapacity() const { return _minCapacity; }

void Route::setName(std::string_view newName) { _name = newName; }
void Route::setEndStop(std::string_view newEndStop) { _endStop = newEndStop; }
void Route::setMinCapacity(int newMinCapacity) { _minCapacity = newMinCapacity; }

bool Route::addVehicle(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
        std::cout << "[ОШИБКА] Пустой указатель на транспорт!" << std::endl;
        return false;
    }

    if (vehicle->getCapacity() < _minCapacity) {
        std::cout << "[ОШИБКА] ТС \"" << vehicle->getModel()
                  << "\" (вместимость " << vehicle->getCapacity()
                  << ") не подходит для маршрута №" << _number
                  << " (требуется минимум " << _minCapacity << ")!" << std::endl;
        return false;
    }

    _assignedVehicles.push_back(vehicle);
    std::cout << "[УСПЕХ] ТС \"" << vehicle->getModel()
              << "\" закреплено за маршрутом №" << _number
              << " \"" << _name << "\"." << std::endl;
    return true;
}

const std::vector<std::shared_ptr<Vehicle>>&
Route::getAssignedVehicles() const {
    return _assignedVehicles;
}

void Route::printRouteInformation() const {
    std::cout << "\n=== Маршрут №" << _number << " ===" << std::endl;
    std::cout << "Название: " << _name << std::endl;
    std::cout << "Начальная остановка: " << _startStop << std::endl;
    std::cout << "Конечная остановка: " << _endStop << std::endl;
    std::cout << "Минимальная вместимость ТС: " << _minCapacity << " чел." << std::endl;
    std::cout << "Транспорт на маршруте (" << _assignedVehicles.size() << "):" << std::endl;
    for (const auto& v : _assignedVehicles) {
        std::cout << "  - " << v->getModel() << " (" << v->getRegNumber() << ")" << std::endl;
    }
}