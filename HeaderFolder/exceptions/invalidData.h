#pragma once

#include "transportException.h"

class InvalidDataException : public TransportException {
public:
    explicit InvalidDataException(const std::string& message)
        : TransportException("Некорректные данные: " + message) {}   
};