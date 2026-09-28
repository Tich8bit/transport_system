#pragma once

#include <string>
#include <string_view>
#include <memory>
#include "basefunction.h"
#include "driver.h"

class Driver;

class Vehicle {
public:
    Vehicle(std::string_view regNumber,
            std::string_view model,
            int year,
            int capacity,
            std::shared_ptr<Driver> driver);

    virtual ~Vehicle() = default;

    std::string_view getRegNumber() const;
    std::string_view getModel() const;
    int getYear() const;
    int getCapacity() const;
    std::shared_ptr<Driver> getDriver() const;

    void setYear(int newYear);
    void setCapacity(int newCapacity);
    void setRegNumber(std::string_view newRegNumber);
    void setDriver(std::shared_ptr<Driver> newDriver);

    void printBaseInfo() const;  
    void readBaseFrom();
    
    virtual void printInfo(std::ostream& os) const = 0;
    virtual std::string getType() const = 0;         
    virtual double calculateMetric() const = 0;
    virtual void applyEffect(int value) = 0;
    virtual bool equals(const Vehicle& other) const = 0;
    virtual std::string getMetricName() const = 0;
    virtual void readFrom() = 0;

    friend std::strong_ordering operator<=>(const Vehicle& a, const Vehicle& b) {
        double metricA = a.calculateMetric();
        double metricB = b.calculateMetric();
        if (metricA < metricB) return std::strong_ordering::less;
        if (metricA > metricB) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
    friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle) {
        vehicle.printInfo(os);
        return os;
    }
    friend bool operator==(const Vehicle& a, const Vehicle& b) {
        return a.equals(b);
    }
protected: // NOSONAR                         
    std::string _regNumber; // NOSONAR
    std::string _model; // NOSONAR
    int _year; // NOSONAR
    int _capacity; // NOSONAR
    int _maxSpeed; // NOSONAR
    std::shared_ptr<Driver> _driver; // NOSONAR
};