#include "tram.h"
#include <iostream>

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

void Tram::printInfo() const {
    Vehicle::printInfo();
    std::cout << "  Колея: " << _gauge << " мм" << std::endl;
    std::cout << "  Расход: " << _powerConsumption << " кВт·ч/100 км" << std::endl;
}

std::string Tram::getType() const { return "Трамвай"; }