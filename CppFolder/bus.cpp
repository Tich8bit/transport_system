#include "bus.h"
#include <iostream>
#include "exceptions.h"

const double Bus::MAX_FUELTANKCAPACITY = 100;
const double Bus::FUEL_METRIC_COEF = 0.5;

Bus::Bus(std::string_view regNumber,
         std::string_view model,
         int year,
         int capacity,
         std::shared_ptr<Driver> driver,
         std::string_view fuelType,
         double fuelTankCapacity)
    : Vehicle(regNumber, model, year, capacity, driver),
      _fuelType(fuelType),
      _fuelTankCapacity(fuelTankCapacity) {}

std::string_view Bus::getFuelType() const { return _fuelType; }
double Bus::getFuelTankCapacity() const { return _fuelTankCapacity; }

void Bus::setFuelType(std::string_view newFuelType) { _fuelType = newFuelType; }
void Bus::setFuelTankCapacity(double newCapacity) { _fuelTankCapacity = newCapacity; }

void Bus::inputVehicle() {
    setRegNumber(inputString("Введите госномер: "));
    setModel(inputString("Введите модель: "));
    setYear(inputInt("Введите год выпуска: ", 1900, 2100));
    setCapacity(inputInt("Введите вместимость: ", 1, 500));
    setFuelType(inputString("Введите тип топлива: "));
    setFuelTankCapacity(inputDouble("Введите объём бака (л): ", 1.0, 1000.0));
}

std::string Bus::getType() const {
    return "Автобус";
}

double Bus::calculateMetric() const {
    return getCapacity() + _fuelTankCapacity * FUEL_METRIC_COEF;
}

void Bus::applyEffect(int value) {
    if (value < 0) {
        throw InvalidOperationException(
            "значение не может быть отрицательным: " + std::to_string(value));
    }

    if (value > MAX_FUELTANKCAPACITY) {
        throw LimitExceededException(
            "бак не может превысить "
            + std::to_string(MAX_FUELTANKCAPACITY)
            + " л: текущий " + std::to_string(_fuelTankCapacity)
            + " л, изменение " + std::to_string(value)
            + " л, итог " + std::to_string(_fuelTankCapacity + value) + " л");
    }

    _fuelTankCapacity += value;
    std::cout << "Бак автобуса " << getRegNumber()
              << " увеличен на " << value << " л. Теперь: "
              << _fuelTankCapacity << " л\n";
}

bool Bus::equals(const Vehicle& other) const {
    auto* b = dynamic_cast<const Bus*>(&other);
    if (!b) return false;
    return getRegNumber() == b->getRegNumber();
}

std::string Bus::getMetricName() const {
    return "Провозная способность (топливо)";
}

void Bus::printInfo(std::ostream& os) const {
    os << "\n  Топливо: " << _fuelType
       << "\n  Объём бака: " << _fuelTankCapacity << " л\n";
}