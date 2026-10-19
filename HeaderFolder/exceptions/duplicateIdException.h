#pragma once

#include "transportException.h"

class DuplicateIdException : public TransportException {
public:
    explicit DuplicateIdException(const std::string& message)
        : TransportException("Дубликат: " + message) {}
};