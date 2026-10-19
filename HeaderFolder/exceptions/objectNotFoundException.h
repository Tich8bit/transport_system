#pragma once

#include "transportException.h"

class ObjectNotFoundException : public TransportException {
public:
    explicit ObjectNotFoundException(const std::string& message)
        : TransportException("Объект не найден: " + message) {}
};