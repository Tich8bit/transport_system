#pragma once

#include <string>
#include <string_view>
#include <iostream>

class Driver {
public:
    Driver(std::string_view fullName, int experienceYears);
    std::string_view getFullName() const;
    int getExperienceYears() const;
    void setFullName(std::string_view newFullName);
    void setExperienceYears(int newExperienceYears);
    void printDriverInformation() const;

    double calculateMetric() const;    

    friend std::ostream& operator<<(std::ostream& os, const Driver& d) {
        os << "Водитель: " << d._fullName
           << " | Стаж: " << d._experienceYears << " лет";
        return os;
    }
private:
static const int EXPERIENCE_METRIC_COEF;
    std::string _fullName;
    int _experienceYears;
};