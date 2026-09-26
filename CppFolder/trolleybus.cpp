#include "trolleybus.h"
#include <iostream>

Trolleybus::Trolleybus(std::string_view regNumber,
                       std::string_view model,
                       int year,
                       int capacity,
                       int maxSpeed,
                       std::shared_ptr<Driver> driver,
                       int voltage,
                       bool hasBattery)
    : Vehicle(regNumber, model, year, capacity, maxSpeed, driver),   
      _voltage(voltage),
      _hasBattery(hasBattery) {}

int Trolleybus::getVoltage() const { return _voltage; }
bool Trolleybus::hasBattery() const { return _hasBattery; }

void Trolleybus::setVoltage(int newVoltage) { _voltage = newVoltage; }
void Trolleybus::setBattery(bool newHasBattery) { _hasBattery = newHasBattery; }

void Trolleybus::printInfo() const {
    Vehicle::printInfo();   
    std::cout << "  Напряжение сети: " << _voltage << " В" << std::endl;
    std::cout << "  Аккумулятор: " << (_hasBattery ? "есть" : "нет") << std::endl;
}

std::string Trolleybus::getType() const {
    return "Троллейбус";
}