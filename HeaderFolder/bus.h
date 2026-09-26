#pragma once
#include "vehicle.h"

class Bus : public Vehicle {
public:
    Bus(std::string_view regNumber, std::string_view model,
        int year, int capacity, int maxSpeed, std::shared_ptr<Driver> driver,
        int doorCount, std::string_view salonType);   

    int getDoorCount() const;
    std::string_view getSalonType() const;

    void printInfo() const override;
    std::string getType() const override;  

private:
    int _doorCount;
    std::string _salonType;
};