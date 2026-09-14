#include "vehicle.h"
#include "driver.h"
#include <iostream>

Vehicle::Vehicle(std::string_view regNumber,
                 std::string_view model,
                 int year,
                 int capacity,
                 std::shared_ptr<Driver> driver)
    : _regNumber(regNumber),
      _model(model),
      _year(year),
      _capacity(capacity),
      _driver(driver) {}

std::string_view Vehicle::getRegNumber() const { return _regNumber; }
std::string_view Vehicle::getModel() const { return _model; }
int Vehicle::getYear() const { return _year; }
int Vehicle::getCapacity() const { return _capacity; }
std::shared_ptr<Driver> Vehicle::getDriver() const { return _driver; }

void Vehicle::setYear(int newYear) { _year = newYear; }
void Vehicle::setCapacity(int newCapacity) { _capacity = newCapacity; }
void Vehicle::setDriver(std::shared_ptr<Driver> newDriver) { _driver = newDriver; }
void Vehicle::printVehicleInformation() const {
    std::cout << "ТС: " << _model
              << " | Госномер: " << _regNumber
              << " | Год выпуска: " << _year
              << " | Вместимость: " << _capacity << " чел." << std::endl;
    if (_driver) {
        std::cout << "  ";
        _driver->printDriverInformation();
    } else {
        std::cout << "  Водитель не назначен." << std::endl;
    }
}