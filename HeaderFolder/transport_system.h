#pragma once

#include <vector>
#include <memory>

class Route;
class Vehicle;

class TransportSystem {
public:
    void addVehicle(std::shared_ptr<Vehicle> vehicle);
    bool removeVehicle(std::shared_ptr<Vehicle> vehicle);
    const std::vector<std::shared_ptr<Route>>& getRoutes() const;
    const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const;
    void printAllRoutes() const;
    void printAllVehicles() const;
    TransportSystem& operator+=(std::shared_ptr<Route> route);
    TransportSystem& operator-=(std::shared_ptr<Route> route);
    std::shared_ptr<Vehicle> findBestVehicle() const;
private:
    std::vector<std::shared_ptr<Route>> _routes;
    std::vector<std::shared_ptr<Vehicle>> _vehicles;
};