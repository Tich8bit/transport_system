#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>

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
    void setEndStop(std::string_view newEndStop);
    void setMinCapacity(int newMinCapacity);
    bool addVehicle(std::shared_ptr<Vehicle> vehicle);
    const std::vector<std::shared_ptr<Vehicle>>& getAssignedVehicles() const;
    void printRouteInformation() const;
private:
    int _number;
    std::string _name;
    std::string _startStop;
    std::string _endStop;
    int _minCapacity;
    std::vector<std::shared_ptr<Vehicle>> _assignedVehicles;
};