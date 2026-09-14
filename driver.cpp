#include "driver.h"
#include <iostream>

Driver::Driver(std::string_view fullName, int experienceYears)
    : _fullName(fullName), _experienceYears(experienceYears) {}

std::string_view Driver::getFullName() const { return _fullName; }
int Driver::getExperienceYears() const { return _experienceYears; }

void Driver::setFullName(std::string_view newFullName) {
    _fullName = newFullName;
}

void Driver::setExperienceYears(int newExperienceYears) {
    _experienceYears = newExperienceYears;
}

void Driver::printDriverInformation() const {
    std::cout << "Водитель: " << _fullName
              << " | Стаж: " << _experienceYears << " лет" << std::endl;
}