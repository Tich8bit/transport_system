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
               double powerConsumption);       

    int getVoltage() const;
    double getPowerConsumption() const;

    void setVoltage(int newVoltage);
    void setPowerConsumption(double newConsumption);

    void printInfo() const override;
    std::string getType() const override;

private:
    int _voltage;                    
    double _powerConsumption;
};