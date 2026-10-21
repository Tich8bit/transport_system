#pragma once

#include <vector>
#include <memory>

class Route;
class Vehicle;
class Driver;

class TransportSystem {
public:
    void addDriver(std::shared_ptr<Driver> driver);
    bool removeDriver(std::shared_ptr<Driver> driver);
    const std::vector<std::shared_ptr<Driver>>& getDrivers() const;

    void addVehicle(std::shared_ptr<Vehicle> vehicle);
    bool removeVehicle(std::shared_ptr<Vehicle> vehicle);
    const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const;

    const std::vector<std::shared_ptr<Route>>& getRoutes() const;
    std::shared_ptr<Route> findRouteByNumber(int number) const;

    void clear();
    std::shared_ptr<Vehicle> findBestVehicle() const;

    TransportSystem& operator+=(std::shared_ptr<Route> route);
    TransportSystem& operator-=(std::shared_ptr<Route> route);

private:
    std::vector<std::shared_ptr<Route>>   _routes;
    std::vector<std::shared_ptr<Vehicle>> _vehicles;
    std::vector<std::shared_ptr<Driver>>  _drivers;
};