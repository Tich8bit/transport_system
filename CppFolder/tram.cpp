#include "tram.h"
#include <iostream>

const double Tram::MAX_GAUGE = 1600;

Tram::Tram(std::string_view regNumber,
           std::string_view model,
           int year,
           int capacity,
           int maxSpeed,
           std::shared_ptr<Driver> driver,
           int gauge,
           double powerConsumption)
    : Vehicle(regNumber, model, year, capacity, maxSpeed, driver),
      _gauge(gauge),
      _powerConsumption(powerConsumption) {}

int Tram::getGauge() const { return _gauge; }
double Tram::getPowerConsumption() const { return _powerConsumption; }

void Tram::setGauge(int newGauge) { _gauge = newGauge; }
void Tram::setPowerConsumption(double newConsumption) { _powerConsumption = newConsumption; }

void Tram::printInfo(std::ostream& os) const {
    os << "\n  Колея: " << _gauge << " мм"
       << "\n  Расход: " << _powerConsumption << " кВт·ч/100 км";
}

std::string Tram::getType() const { return "Трамвай"; }

double Tram::calculateMetric() const {
    return _capacity * 1.5 - _powerConsumption * 0.4;
}

void Tram::applyEffect(int value) {
    if (value < 0) {
        std::cout << "Нельзя уменьшать колею!\n";
        return;
    }
    if (_gauge + value > MAX_GAUGE) {
        std::cout << "Колея слишком большая!\n";
        return;
    }
    _gauge += value;
    std::cout << "Колея трамвая " << _regNumber 
         << " увеличена на " << value << " мм. Теперь: " 
         << _gauge << " мм\n";
}

bool Tram::equals(const Vehicle& other) const {
    auto* t = dynamic_cast<const Tram*>(&other);
    if (!t) return false;
    return _regNumber == t->_regNumber;
}

std::string Tram::getMetricName() const {
    return "Провозная способность (колея)";
}

void Tram::readFrom(std::istream& is) {
    std::cout << "Введите колею (мм): ";
    is >> _gauge;
    std::cout << "Введите расход (кВт·ч/100 км): ";
    is >> _powerConsumption;
}