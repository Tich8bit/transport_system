#pragma once

#include "transportException.h"

class OutOfRangeException : public TransportException {
public:
    explicit OutOfRangeException(const std::string& message)
        : TransportException("Выход за границы: " + message) {}
};
