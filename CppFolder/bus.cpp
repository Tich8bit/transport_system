#include "bus.h"
#include <iostream>

const double Bus::MAX_FUELTANKCAPACITY = 100;

Bus::Bus(std::string_view regNumber,
         std::string_view model,
         int year,
         int capacity,
         int maxSpeed,
         std::shared_ptr<Driver> driver,
         std::string_view fuelType,
         double fuelTankCapacity)
    : Vehicle(regNumber, model, year, capacity, maxSpeed, driver),
      _fuelType(fuelType),
      _fuelTankCapacity(fuelTankCapacity) {}

std::string_view Bus::getFuelType() const { return _fuelType; }
double Bus::getFuelTankCapacity() const { return _fuelTankCapacity; }

void Bus::setFuelType(std::string_view newFuelType) { _fuelType = newFuelType; }
void Bus::setFuelTankCapacity(double newCapacity) { _fuelTankCapacity = newCapacity; }

void Bus::printInfo(std::ostream& os) const {
    os << "\n  Топливо: " << _fuelType
       << "\n  Объём бака: " << _fuelTankCapacity << " л";
}

std::string Bus::getType() const {
    return "Автобус";
}

double Bus::calculateMetric() const {
    return _capacity * 1.0 + _fuelTankCapacity * 0.5;
}

void Bus::applyEffect(int value) {
    if (value < 0 || value > MAX_FUELTANKCAPACITY) {
        std::cout << "Недопустимое изменение бака!\n";
        return;
    }
    _fuelTankCapacity += value;   
    std::cout << "Бак автобуса " << _regNumber << " увеличен на " << value << " л. Теперь: "  << _fuelTankCapacity << " л\n";
}

bool Bus::equals(const Vehicle& other) const {
    auto* b = dynamic_cast<const Bus*>(&other);
    if (!b) return false;
    return _regNumber == b->_regNumber;
}

std::string Bus::getMetricName() const {
    return "Провозная способность (топливо)";
}

void Bus::readFrom(std::istream& is) {
    std::cout << "Введите тип топлива: ";
    std::getline(is >> std::ws, _fuelType);
    std::cout << "Введите объём бака (л): ";
    is >> _fuelTankCapacity;
}