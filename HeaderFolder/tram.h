#pragma once

#include "vehicle.h"

class Tram : public Vehicle {
public:
    Tram(std::string_view regNumber,
         std::string_view model,
         int year,
         int capacity,
         std::shared_ptr<Driver> driver,
         int gauge,                       
         double powerConsumption);        

    int getGauge() const;
    double getPowerConsumption() const;

    void setGauge(int newGauge);
    void setPowerConsumption(double newConsumption);

    void printInfo(std::ostream& os) const override;
    std::string getType() const override;
    double calculateMetric() const override;
    void applyEffect(int value) override;
    bool equals(const Vehicle& other) const override;
    std::string getMetricName() const override;
    void inputVehicle() override;
private:
    static const double MAX_GAUGE;
    static const double GAUGE_METRIC_COEF;
    int _gauge;                       
    double _powerConsumption;         
};