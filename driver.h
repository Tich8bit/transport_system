#pragma once

#include <string>
#include <string_view>

class Driver {
public:
    Driver(std::string_view fullName, int experienceYears);
    std::string_view getFullName() const;
    int getExperienceYears() const;
    void setFullName(std::string_view newFullName);
    void setExperienceYears(int newExperienceYears);
    void printDriverInformation() const;
private:
    std::string _fullName;
    int _experienceYears;
};