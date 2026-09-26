#pragma once

#include "vehicle.h"

class Tram : public Vehicle {
public:
    Tram(std::string_view regNumber,
         std::string_view model,
         int year,
         int capacity,
         int maxSpeed,
         std::shared_ptr<Driver> driver,
         int trackNumber,           
         bool hasPantograph);       

    int getTrackNumber() const;
    bool hasPantograph() const;

    void setTrackNumber(int newTrackNumber);
    void setPantograph(bool newHasPantograph);

    void printInfo() const override;
    std::string getType() const override;

private:
    int _trackNumber;       
    bool _hasPantograph;    
};