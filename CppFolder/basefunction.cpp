#include "basefunction.h"

std::string inputString(std::string_view message) {
    std::string input;
    std::cout << message.data();
    std::getline(std::cin, input);
    return input;
}

int inputInt(std::string_view message) {
    std::string input;
    int number;
    char extra;
    while (true) {
        std::cout << message.data();
        std::getline(std::cin, input);
        if (std::stringstream ss(input); ss >> number && !(ss >> extra)) {
            return number;
        }
        std::cout << "Ошибка! Введите целое число.\n";
    }
}
