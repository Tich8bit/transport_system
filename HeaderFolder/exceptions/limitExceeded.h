#pragma once

#include "TransportException.h"

class LimitExceededException : public TransportException {
public:
    explicit LimitExceededException(const std::string& message)
        : TransportException("Превышено ограничение: " + message) {}
};