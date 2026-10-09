#pragma once

#include "transportException.h"

class InvalidOperationException : public TransportException {
public:
    explicit InvalidOperationException(const std::string& message)
        : TransportException("Недопустимая операция: " + message) {}
};