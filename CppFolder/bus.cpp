#include "bus.h"
#include <iostream>

Bus::Bus(std::string_view regNumber, std::string_view model,
         int year, int capacity, int maxSpeed, std::shared_ptr<Driver> driver,
         int doorCount, std::string_view salonType)
    : Vehicle(regNumber, model, year, capacity, maxSpeed, driver),   
      _doorCount(doorCount),
      _salonType(salonType) {}

int Bus::getDoorCount() const { return _doorCount; }
std::string_view Bus::getSalonType() const { return _salonType; }

void Bus::printInfo() const {
    Vehicle::printInfo();   
    std::cout << "  Дверей: " << _doorCount << std::endl;
    std::cout << "  Салон: " << _salonType << std::endl;
}

std::string Bus::getType() const {
    return "Автобус";
}