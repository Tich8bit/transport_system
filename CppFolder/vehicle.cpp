#include "vehicle.h"
#include "driver.h"
#include "basefunction.h"
#include <iostream>
#include <compare>

Vehicle::Vehicle(std::string_view regNumber,
                 std::string_view model,
                 int year,
                 int capacity,
                 int maxSpeed,
                 std::shared_ptr<Driver> driver)
    : _regNumber(regNumber),
      _model(model),
      _year(year),
      _capacity(capacity),
      _maxSpeed(maxSpeed),
      _driver(driver) {}

std::string_view Vehicle::getRegNumber() const { return _regNumber; }
std::string_view Vehicle::getModel() const { return _model; }
int Vehicle::getYear() const { return _year; }
int Vehicle::getCapacity() const { return _capacity; }
int Vehicle::getMaxSpeed() const { return _maxSpeed; }
std::shared_ptr<Driver> Vehicle::getDriver() const { return _driver; }

void Vehicle::setYear(int newYear) { _year = newYear; }
void Vehicle::setCapacity(int newCapacity) { _capacity = newCapacity; }
void Vehicle::setRegNumber(std::string_view newRegNumber) { _regNumber = newRegNumber; }
void Vehicle::setMaxSpeed(int newMaxSpeed) { _maxSpeed = newMaxSpeed; }
void Vehicle::setDriver(std::shared_ptr<Driver> newDriver) { _driver = newDriver; }

void Vehicle::printBaseInfo() const {
    std::cout << "ТС: " << _model
              << " | Госномер: " << _regNumber
              << " | Год: " << _year
              << " | Вместимость: " << _capacity << " чел."
              << " | Скорость: " << _maxSpeed << " км/ч.";
    if (_driver)
        std::cout << " | Водитель: " << _driver->getFullName();
    else
        std::cout << " | Водитель не назначен";
}

void Vehicle::readBaseFrom() {
    std::cout << "Введите госномер: ";
    std::getline(std::cin >> std::ws, _regNumber);
    std::cout << "Введите модель: ";
    std::getline(std::cin, _model);
    std::cout << "Введите год выпуска: ";
    std::cin >> _year;
    std::cout << "Введите вместимость: ";
    std::cin >> _capacity;
    std::cout << "Введите максимальную скорость: ";
    std::cin >> _maxSpeed;
}