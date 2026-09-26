#pragma once

#include "vehicle.h"

class Trolleybus : public Vehicle {
public:
    Trolleybus(std::string_view regNumber,
               std::string_view model,
               int year,
               int capacity,
               int maxSpeed,
               std::shared_ptr<Driver> driver,
               int voltage,            
               bool hasBattery);       

    int getVoltage() const;
    bool hasBattery() const;

    void setVoltage(int newVoltage);
    void setBattery(bool newHasBattery);

    void printInfo() const override;
    std::string getType() const override;

private:
    int _voltage;       
    bool _hasBattery;   
};