#pragma once

#include "vehicle.h"

class Tram : public Vehicle {
public:
    Tram(std::string_view regNumber,
         std::string_view model,
         int year,
         int capacity,
         int maxSpeed,
         std::shared_ptr<Driver> driver,
         int gauge,                       
         double powerConsumption);        

    int getGauge() const;
    double getPowerConsumption() const;

    void setGauge(int newGauge);
    void setPowerConsumption(double newConsumption);

    void printInfo() const override;
    std::string getType() const override;

private:
    int _gauge;                       
    double _powerConsumption;         
};