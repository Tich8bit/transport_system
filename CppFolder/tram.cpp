#include "tram.h"
#include <iostream>

Tram::Tram(std::string_view regNumber,
           std::string_view model,
           int year,
           int capacity,
           int maxSpeed,
           std::shared_ptr<Driver> driver,
           int trackNumber,
           bool hasPantograph)
    : Vehicle(regNumber, model, year, capacity, maxSpeed, driver),   
      _trackNumber(trackNumber),
      _hasPantograph(hasPantograph) {}

int Tram::getTrackNumber() const { return _trackNumber; }
bool Tram::hasPantograph() const { return _hasPantograph; }

void Tram::setTrackNumber(int newTrackNumber) { _trackNumber = newTrackNumber; }
void Tram::setPantograph(bool newHasPantograph) { _hasPantograph = newHasPantograph; }

void Tram::printInfo() const {
    Vehicle::printInfo();   
    std::cout << "  Номер пути: " << _trackNumber << std::endl;
    std::cout << "  Пантограф: " << (_hasPantograph ? "есть" : "нет") << std::endl;
}

std::string Tram::getType() const {
    return "Трамвай";
}