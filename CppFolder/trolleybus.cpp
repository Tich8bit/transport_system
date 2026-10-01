#include "trolleybus.h"
#include <iostream>
#include "exceptions.h"

const double Trolleybus::MAX_VOLTAGE = 100;
const double Trolleybus::VOLTAGE_METRIC_COEF = 0.5;

Trolleybus::Trolleybus(std::string_view regNumber,
                       std::string_view model,
                       int year,
                       int capacity,
                       std::shared_ptr<Driver> driver,
                       int voltage,
                       double powerConsumption)
    : Vehicle(regNumber, model, year, capacity, driver),
      _voltage(voltage),
      _powerConsumption(powerConsumption) {}

int Trolleybus::getVoltage() const { return _voltage; }
double Trolleybus::getPowerConsumption() const { return _powerConsumption; }

void Trolleybus::setVoltage(int newVoltage) { _voltage = newVoltage; }
void Trolleybus::setPowerConsumption(double newConsumption) { _powerConsumption = newConsumption; }

void Trolleybus::inputVehicle() {
    setRegNumber(inputString("Введите госномер: "));
    setModel(inputString("Введите модель: "));
    setYear(inputInt("Введите год выпуска: ", 1900, 2100));
    setCapacity (inputInt("Введите вместимость: ", 1, 500));
    setVoltage(inputInt("Введите напряжение (В): ", 100, 1500));
    setPowerConsumption(inputDouble("Введите расход (кВт·ч/100 км): ", 1.0, 200.0));
}

std::string Trolleybus::getType() const { return "Троллейбус"; }

double Trolleybus::calculateMetric() const {
    return getCapacity() + _voltage * VOLTAGE_METRIC_COEF;
}

void Trolleybus::applyEffect(int value) {
    if (value < 0) {
        throw InvalidOperationException(
            "значение не может быть отрицательным: " + std::to_string(value));
    }

    if (value > MAX_VOLTAGE) {
        throw LimitExceededException(
            "напряжение не может превысить "
            + std::to_string(MAX_VOLTAGE)
            + " В: текущее " + std::to_string(_voltage)
            + " В, изменение " + std::to_string(value)
            + " В, итог " + std::to_string(_voltage + value) + " В");
    }

    _voltage += value;
    std::cout << "Напряжение троллейбуса " << getRegNumber()
              << " увеличено на " << value << " В. Теперь: "
              << _voltage << " В\n";
}

bool Trolleybus::equals(const Vehicle& other) const {
    auto* t = dynamic_cast<const Trolleybus*>(&other);
    if (!t) return false;
    return getRegNumber() == t->getRegNumber();
}

std::string Trolleybus::getMetricName() const {
    return "Провозная способность (напряжение)";
}

void Trolleybus::printInfo(std::ostream& os) const {
    os << "\n  Напряжение: " << _voltage << " В"
       << "\n  Расход: " << _powerConsumption << " кВт·ч/100 км";
}