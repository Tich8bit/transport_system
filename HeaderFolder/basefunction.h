#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <sstream>
#include <string_view>

std::string inputString(std::string_view message);
int inputInt(std::string_view message, int max, int min);
double inputDouble(std::string_view message, double min, double max);