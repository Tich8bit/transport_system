#include "tram.h"
#include <iostream>
#include "exceptions/exception.h"

const double Tram::MAX_GAUGE = 1600;
const double Tram::GAUGE_METRIC_COEF = 0.1;

Tram::Tram(std::string_view regNumber,
           std::string_view model,
           int year,
           int capacity,
           std::shared_ptr<Driver> driver,
           int gauge,
           double powerConsumption)
    : Vehicle(regNumber, model, year, capacity, driver),
      _gauge(gauge),
      _powerConsumption(powerConsumption) {}

int Tram::getGauge() const { return _gauge; }
double Tram::getPowerConsumption() const { return _powerConsumption; }

void Tram::setGauge(int newGauge) { _gauge = newGauge; }
void Tram::setPowerConsumption(double newConsumption) { _powerConsumption = newConsumption; }

void Tram::inputVehicle() {
    setRegNumber(inputString("Введите госномер: "));
    setModel(inputString("Введите модель: "));
    setYear(inputInt("Введите год выпуска: ", 1900, 2100));
    setCapacity(inputInt("Введите вместимость: ", 1, 500));
    setGauge(inputInt("Введите колею (мм): ", 1000, 2000));
    setPowerConsumption(inputDouble("Введите расход (кВт·ч/100 км): ", 1.0, 200.0));
}

std::string Tram::getType() const { return "Трамвай"; }

double Tram::calculateMetric() const {
    return getCapacity() + _gauge * GAUGE_METRIC_COEF;
}

void Tram::applyEffect(int value) {
    if (value < 0) {
        throw InvalidOperationException(
            "значение не может быть отрицательным: " + std::to_string(value));
    }

    if (value > MAX_GAUGE) {
        throw LimitExceededException(
            "колея не может превысить " + std::to_string(MAX_GAUGE)
            + " мм: текущая " + std::to_string(_gauge)
            + " мм, изменение " + std::to_string(value)
            + " мм, итог " + std::to_string(_gauge + value) + " мм");
    }

    _gauge += value;
    std::cout << "Колея трамвая " << getRegNumber()
              << " увеличена на " << value << " мм. Теперь: "
              << _gauge << " мм\n";
}

bool Tram::equals(const Vehicle& other) const {
    auto* t = dynamic_cast<const Tram*>(&other);
    if (!t) return false;
    return getRegNumber() == t->getRegNumber();
}

std::string Tram::getMetricName() const {
    return "Провозная способность (колея)";
}

void Tram::printInfo(std::ostream& os) const {
    os << "\n  Колея: " << _gauge << " мм"
       << "\n  Расход: " << _powerConsumption << " кВт·ч/100 км";
}