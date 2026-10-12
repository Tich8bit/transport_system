#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <sstream>
#include <string_view>

std::string inputString(std::string_view message);

template <typename T>
T inputNumber(std::string_view message, T min, T max) {
    std::string input;
    T number;
    char extra;

    while (true) {
        std::cout << message.data();
        std::getline(std::cin, input);

        if (std::stringstream ss(input); ss >> number && !(ss >> extra)
                                         && number >= min && number <= max) {
            return number;
        }

        std::cout << "Ошибка! Введите число от " << min
                  << " до " << max << ".\n";
    }
}