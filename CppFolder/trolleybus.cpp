#include "trolleybus.h"
#include <iostream>

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

void Trolleybus::printInfo() const {
    Vehicle::printInfo();
    std::cout << "  Напряжение: " << _voltage << " В" << std::endl;
    std::cout << "  Расход: " << _powerConsumption << " кВт·ч/100 км" << std::endl;
}

std::string Trolleybus::getType() const { return "Троллейбус"; }