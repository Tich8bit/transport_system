#pragma once

#include <vector>
#include <memory>

class Route;
class Vehicle;

class TransportSystem {
public:
    bool addRoute(std::shared_ptr<Route> route);   
    bool removeRoute(std::shared_ptr<Route> route);
    void addVehicle(std::shared_ptr<Vehicle> vehicle);
    std::shared_ptr<Route> findRouteByNumber(int number) const;
    const std::vector<std::shared_ptr<Route>>& getRoutes() const;
    const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const;
    void printAllRoutes() const;
    void printAllVehicles() const;
    TransportSystem& operator+=(std::shared_ptr<Route> route);
    TransportSystem& operator-=(std::shared_ptr<Route> route);
private:
    std::vector<std::shared_ptr<Route>> _routes;
    std::vector<std::shared_ptr<Vehicle>> _vehicles;
};