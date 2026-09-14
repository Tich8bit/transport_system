#pragma once

#include <vector>
#include <memory>

class Route;
class Vehicle;

class TransportSystem {
public:
    void addRoute(std::shared_ptr<Route> route);
    void addVehicle(std::shared_ptr<Vehicle> vehicle);
    std::shared_ptr<Route> findRouteByNumber(int number) const;
    std::shared_ptr<Vehicle> findVehicleByRegNumber(std::string_view regNumber) const;
    const std::vector<std::shared_ptr<Route>>& getRoutes() const;
    const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const;
    void printAllRoutes() const;
    void printAllVehicles() const;
private:
    std::vector<std::shared_ptr<Route>> _routes;
    std::vector<std::shared_ptr<Vehicle>> _vehicles;
};