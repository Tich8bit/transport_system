#pragma once

#include <string>
#include <string_view>
#include <memory>

class Driver;

class Vehicle {
public:
    Vehicle(std::string_view regNumber, std::string_view model, int year, int capacity, int maxSpeed, std::shared_ptr<Driver> driver);
    std::string_view getRegNumber() const;
    std::string_view getModel() const;
    int getYear() const;
    int getCapacity() const;
    int getMaxSpeed() const;
    std::shared_ptr<Driver> getDriver() const;
    std::string inputString(std::string_view message);
    int inputInt(std::string_view message);
    void setYear(int newYear);
    void setCapacity(int newCapacity);
    void setRegNumber(std::string_view newRegNumber);
    void setMaxSpeed(int newMaxSpeed);
    void setDriver(std::shared_ptr<Driver> newDriver);
    friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle);
    friend std::istream& operator>>(std::istream& is, Vehicle& vehicle);
    friend bool isNewer(const Vehicle& a, const Vehicle& b);
    bool operator==(const Vehicle& other) const;
    bool operator<(const Vehicle& other) const;
    bool operator<=(const Vehicle& other) const;
    bool operator>(const Vehicle& other) const;
    bool operator>=(const Vehicle& other) const;
private:
    std::string _regNumber;
    std::string _model;
    int _year;
    int _capacity;
    int _maxSpeed;
    std::shared_ptr<Driver> _driver;
};