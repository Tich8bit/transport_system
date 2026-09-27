#include "trolleybus.h"
#include <iostream>

const double Trolleybus::MAX_VOLTAGE = 100;

Trolleybus::Trolleybus(std::string_view regNumber,
                       std::string_view model,
                       int year,
                       int capacity,
                       int maxSpeed,
                       std::shared_ptr<Driver> driver,
                       int voltage,
                       double powerConsumption)
    : Vehicle(regNumber, model, year, capacity, maxSpeed, driver),
      _voltage(voltage),
      _powerConsumption(powerConsumption) {}

int Trolleybus::getVoltage() const { return _voltage; }
double Trolleybus::getPowerConsumption() const { return _powerConsumption; }

void Trolleybus::setVoltage(int newVoltage) { _voltage = newVoltage; }
void Trolleybus::setPowerConsumption(double newConsumption) { _powerConsumption = newConsumption; }

void Trolleybus::printInfo(std::ostream& os) const {
   os << "\n  Напряжение: " << _voltage << " В"
       << "\n  Расход: " << _powerConsumption << " кВт·ч/100 км";
}

std::string Trolleybus::getType() const { return "Троллейбус"; }

double Trolleybus::calculateMetric() const { return _voltage / _powerConsumption; }

void Trolleybus::applyEffect(int value) {
    if (value < 0 || value > MAX_VOLTAGE) {
        std::cout << "Недопустимое изменение напряжения!\n";
        return;
    }
    _voltage += value;
    std::cout << "Напряжение троллейбуса " << _regNumber 
         << " увеличено на " << value << " В. Теперь: " 
         << _voltage << " В\n";
}

bool Trolleybus::equals(const Vehicle& other) const {
    auto* t = dynamic_cast<const Trolleybus*>(&other);
    if (!t) return false;
    return _regNumber == t->_regNumber;
}

std::string Trolleybus::getMetricName() const {
    return "Провозная способность (напряжение)";
}

void Trolleybus::readFrom(std::istream& is) {
    std::cout << "Введите напряжение (В): ";
    is >> _voltage;
    std::cout << "Введите расход (кВт·ч/100 км): ";
    is >> _powerConsumption;
}