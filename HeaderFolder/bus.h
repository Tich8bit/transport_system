#pragma once

#include "vehicle.h"

class Bus : public Vehicle {
public:
    Bus(std::string_view regNumber,
        std::string_view model,
        int year,
        int capacity,
        int maxSpeed,
        std::shared_ptr<Driver> driver,
        std::string_view fuelType,
        double fuelTankCapacity);            

    std::string_view getFuelType() const;
    double getFuelTankCapacity() const;

    void setFuelType(std::string_view newFuelType);
    void setFuelTankCapacity(double newCapacity);

    void printInfo() const override;
    std::string getType() const override;

private:
    std::string _fuelType;           
    double _fuelTankCapacity;        
};