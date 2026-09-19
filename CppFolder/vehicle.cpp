#include "vehicle.h"
#include "driver.h"
#include <iostream>

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
void Vehicle::setDriver(std::shared_ptr<Driver> newDriver) { _driver = newDriver; }
void Vehicle::setRegNumber(std::string_view newRegNumber){ _regNumber = newRegNumber; }
void Vehicle::setMaxSpeed(int newMaxSpeed) { _maxSpeed = newMaxSpeed; }

std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle) {
    os << "ТС: " << vehicle._model
       << " | Госномер: " << vehicle._regNumber
       << " | Год выпуска: " << vehicle._year
       << " | Вместимость: " << vehicle._capacity << " чел."
       << " | Скорость: " << vehicle._maxSpeed << " км/ч.";
    if (vehicle._driver) {
        os << " | Водитель: " << vehicle._driver->getFullName();
    } else {
        os << " | Водитель не назначен";
    }
    return os;
}

std::istream& operator>>(std::istream& is, Vehicle& vehicle) {
    std::cout << "Введите госномер: ";
    is >> vehicle._regNumber;
    std::cout << "Введите модель: ";
    is >> vehicle._model;
    std::cout << "Введите год выпуска: ";
    is >> vehicle._year;
    std::cout << "Введите вместимость: ";
    is >> vehicle._capacity;
    std::cout << "Введите скорость: ";
    is >> vehicle._maxSpeed;
    return is;
}

bool isNewer(const Vehicle& a, const Vehicle& b) {
    return a._year > b._year;
}

bool Vehicle::operator==(const Vehicle& other) const {
    return _regNumber == other._regNumber;
}

bool Vehicle::operator!=(const Vehicle& other) const {
    return _regNumber != other._regNumber;
}

bool Vehicle::operator<(const Vehicle& other) const {
    return _maxSpeed < other._maxSpeed;
}

bool Vehicle::operator<=(const Vehicle& other) const {
    return _maxSpeed <= other._maxSpeed;
}

bool Vehicle::operator>(const Vehicle& other) const {
    return _maxSpeed > other._maxSpeed;
}

bool Vehicle::operator>=(const Vehicle& other) const {
    return _maxSpeed >= other._maxSpeed;
}