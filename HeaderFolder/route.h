#pragma once

#include <string>
#include <string_view>
#include <memory>
#include "collection.h"

class Vehicle;

class Route {
public:
    Route(int number, std::string_view name, std::string_view startStop, std::string_view endStop, int minCapacity);
    
    int getNumber() const;
    std::string_view getName() const;
    std::string_view getStartStop() const;
    std::string_view getEndStop() const;
    int getMinCapacity() const;
    
    void setName(std::string_view newName);
    void setStartStop(std::string_view newStartStop);
    void setEndStop(std::string_view newEndStop);
    void setMinCapacity(int newMinCapacity);
    
    const Collection<Vehicle>& getAssignedVehicles() const;
    void printRouteInformation() const;
    
    Route& operator+=(std::shared_ptr<Vehicle> vehicle);
    Route& operator-=(std::shared_ptr<Vehicle> vehicle);

private:
    int _number;
    std::string _name;
    std::string _startStop;
    std::string _endStop;
    int _minCapacity;
    Collection<Vehicle> _assignedVehicles;
};