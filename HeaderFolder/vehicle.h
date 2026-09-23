#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <compare>
#include <iosfwd>
#include "basefunction.h"
#include "driver.h"
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
    void setYear(int newYear);
    void setCapacity(int newCapacity);
    void setRegNumber(std::string_view newRegNumber);
    void setMaxSpeed(int newMaxSpeed);
    void setDriver(std::shared_ptr<Driver> newDriver);
    friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle) {
        os << "ТС: " << vehicle._model
        << " | Госномер: " << vehicle._regNumber
        << " | Год выпуска: " << vehicle._year
        << " | Вместимость: " << vehicle._capacity << " чел."
        << " | Скорость: " << vehicle._maxSpeed << " км/ч.";
        if (vehicle._driver) {
            os << " | Водитель: " << vehicle._driver->getFullName();
        } else {
            os << " | Водитель не назначен\n";
        }
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Vehicle& vehicle) {
        vehicle._regNumber = inputString("Введите госномер: ");
        vehicle._model     = inputString("Введите модель: ");
        vehicle._year      = inputInt("Введите год выпуска: ");
        vehicle._capacity  = inputInt("Введите вместимость: ");
        vehicle._maxSpeed  = inputInt("Введите скорость: ");
        return is;
    }
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