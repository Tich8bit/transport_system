#pragma once

#include <stdexcept>
#include <string>

class TransportException : public std::runtime_error {
public:
    explicit TransportException(const std::string& message)
        : std::runtime_error(message) {}
};

class InvalidDataException : public TransportException {
public:
    explicit InvalidDataException(const std::string& message)
        : TransportException("Некорректные данные: " + message) {}
};

class ObjectNotFoundException : public TransportException {
public:
    explicit ObjectNotFoundException(const std::string& message)
        : TransportException("Объект не найден: " + message) {}
};

class DuplicateIdException : public TransportException {
public:
    explicit DuplicateIdException(const std::string& message)
        : TransportException("Дубликат идентификатора: " + message) {}
};

class LimitExceededException : public TransportException {
public:
    explicit LimitExceededException(const std::string& message)
        : TransportException("Нарушение ограничения: " + message) {}
};

class InvalidOperationException : public TransportException {
public:
    explicit InvalidOperationException(const std::string& message)
        : TransportException("Недопустимая операция: " + message) {}
};

class OutOfRangeException : public TransportException {
public:
    explicit OutOfRangeException(const std::string& message)
        : TransportException("Выход за границы: " + message) {}
};

class RelationException : public TransportException {
public:
    explicit RelationException(const std::string& message)
        : TransportException("Нарушение связи: " + message) {}
};