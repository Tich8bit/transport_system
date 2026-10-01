#include "vehicle.h"
#include "driver.h"
#include "basefunction.h"
#include <iostream>
#include <compare>
#include <vector>
#include "exceptions.h"

Vehicle::Vehicle(std::string_view regNumber,
                 std::string_view model,
                 int year,
                 int capacity,
                 std::shared_ptr<Driver> driver)
    : _regNumber(regNumber),
      _model(model),
      _year(year),
      _capacity(capacity),
      _driver(driver)
{
    if (year < 1900 || year > 2100) {
        throw InvalidDataException(
            "год " + std::to_string(year) + " вне диапазона [1900, 2100]");
    }
    if (capacity <= 0 || capacity > 500) {
        throw InvalidDataException(
            "вместимость " + std::to_string(capacity) + " вне диапазона [1, 500]");
    }
}

std::string_view Vehicle::getRegNumber() const { return _regNumber; }
std::string_view Vehicle::getModel() const { return _model; }
int Vehicle::getYear() const { return _year; }
int Vehicle::getCapacity() const { return _capacity; }
std::shared_ptr<Driver> Vehicle::getDriver() const { return _driver; }

void Vehicle::setYear(int newYear) { _year = newYear; }
void Vehicle::setCapacity(int newCapacity) { _capacity = newCapacity; }
void Vehicle::setModel(std::string_view newModel) { _model = newModel; }
void Vehicle::setRegNumber(std::string_view newRegNumber) { _regNumber = newRegNumber; }
void Vehicle::setDriver(std::shared_ptr<Driver> newDriver) { _driver = newDriver; }

void Vehicle::printBaseInfo() const {
    std::cout << "ТС: " << _model
              << " | Госномер: " << _regNumber
              << " | Год: " << _year
              << " | Вместимость: " << _capacity << " чел.";
    if (_driver)
        std::cout << " | Водитель: " << _driver->getFullName();
    else
        std::cout << " | Водитель не назначен";
}
