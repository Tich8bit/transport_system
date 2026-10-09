#pragma once

#include "transportException.h"

class RelationException : public TransportException {
public:
    explicit RelationException(const std::string& message)
        : TransportException("Нарушение связи: " + message) {}
};