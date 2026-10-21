#pragma once

#include "collection.h"
#include "driver.h"
#include "vehicle.h"
#include "route.h"

class TransportSystem {
public:
    void addDriver(std::shared_ptr<Driver> driver);
    bool removeDriver(std::shared_ptr<Driver> driver);
    const Collection<Driver>& getDrivers() const;

    void addVehicle(std::shared_ptr<Vehicle> vehicle);
    bool removeVehicle(std::shared_ptr<Vehicle> vehicle);
    const Collection<Vehicle>& getVehicles() const;

    const Collection<Route>& getRoutes() const;
    std::shared_ptr<Route> findRouteByNumber(int number) const;

    void clear();
    std::shared_ptr<Vehicle> findBestVehicle() const;

    TransportSystem& operator+=(std::shared_ptr<Route> route);
    TransportSystem& operator-=(std::shared_ptr<Route> route);

private:
    Collection<Route>   _routes;
    Collection<Vehicle> _vehicles;
    Collection<Driver>  _drivers;
};