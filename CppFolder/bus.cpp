#include "bus.h"
#include <iostream>

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

void Bus::printInfo() const {
    Vehicle::printInfo();
    std::cout << "  Топливо: " << _fuelType << std::endl;
    std::cout << "  Объём бака: " << _fuelTankCapacity << " л" << std::endl;
}

std::string Bus::getType() const {
    return "Автобус";
}