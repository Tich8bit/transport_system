#pragma once

#include <string>
#include <string_view>
#include <memory>

class Driver;

class Vehicle {
public:
    Vehicle(std::string_view regNumber, std::string_view model, int year, int capacity, std::shared_ptr<Driver> driver);
    std::string_view getRegNumber() const;
    std::string_view getModel() const;
    int getYear() const;
    int getCapacity() const;
    std::shared_ptr<Driver> getDriver() const;
    void setYear(int newYear);
    void setCapacity(int newCapacity);
    void setRegNumber(std::string_view newRegNumber);
    void setDriver(std::shared_ptr<Driver> newDriver);
    void printVehicleInformation() const;
private:
    std::string _regNumber;
    std::string _model;
    int _year;
    int _capacity;
    std::shared_ptr<Driver> _driver;
};