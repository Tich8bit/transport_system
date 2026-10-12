#include "basefunction.h"

std::string inputString(std::string_view message) {
    std::string input;
    std::cout << message.data();
    std::getline(std::cin, input);
    return input;
}
