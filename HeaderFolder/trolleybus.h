#pragma once

#include "vehicle.h"

class Trolleybus : public Vehicle {
public:
    Trolleybus(std::string_view regNumber,
               std::string_view model,
               int year,
               int capacity,
               std::shared_ptr<Driver> driver,
               int voltage,                    
               double powerConsumption);       

    int getVoltage() const;
    double getPowerConsumption() const;

    void setVoltage(int newVoltage);
    void setPowerConsumption(double newConsumption);

    void printInfo(std::ostream& os) const override;
    std::string getType() const override;
    bool equals(const Vehicle& other) const override;
    double calculateMetric() const override;
    void applyEffect(int value) override;
    std::string getMetricName() const override;
    void inputVehicle() override;
private:
    static const double MAX_VOLTAGE;
    static const double VOLTAGE_METRIC_COEF;
    int _voltage;                    
    double _powerConsumption;
};