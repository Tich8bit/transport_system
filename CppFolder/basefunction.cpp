#include "basefunction.h"

std::string inputString(std::string_view message) {
    std::string input;
    std::cout << message.data();
    std::getline(std::cin, input);
    return input;
}
int inputInt(std::string_view message, int min, int max) {
    std::string input;
    int number;
    char extra;

    while (true) {
        std::cout << message.data();
        std::getline(std::cin, input);

        std::stringstream ss(input);
        if (ss >> number && !(ss >> extra) && number >= min && number <= max) {
            return number;
        }

        std::cout << "Ошибка! Введите целое число от " << min
                  << " до " << max << ".\n";
    }
}

double inputDouble(std::string_view message, double min, double max) {
    std::string input;
    double number;
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