#include "transport_system.h"
#include "exceptions/exception.h"

void TransportSystem::addDriver(std::shared_ptr<Driver> driver) {
    if (!driver) throw InvalidDataException("пустой указатель на водителя");
    auto existing = _drivers.find([&driver](const std::shared_ptr<Driver>& d) {
        return d->getFullName() == driver->getFullName();
    });
    if (existing) throw DuplicateIdException("Водитель с именем " + std::string(driver->getFullName()) + " уже есть в системе");
    _drivers.add(driver);
}

bool TransportSystem::removeDriver(std::shared_ptr<Driver> driver) {
    if (!driver) throw InvalidDataException("пустой указатель на водителя");
    size_t removed = _drivers.removeIf([&driver](const std::shared_ptr<Driver>& d) { return d == driver; });
    if (removed == 0) throw ObjectNotFoundException("Водитель не найден в системе");
    return true;
}

const Collection<Driver>& TransportSystem::getDrivers() const { return _drivers; }

void TransportSystem::addVehicle(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) throw InvalidDataException("пустой указатель на ТС");
    auto existing = _vehicles.find([&vehicle](const std::shared_ptr<Vehicle>& v) {
        return *v == *vehicle;
    });
    if (existing) throw DuplicateIdException("ТС с госномером " + std::string(vehicle->getRegNumber()) + " уже есть в системе");
    _vehicles.add(vehicle);
}

bool TransportSystem::removeVehicle(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) throw InvalidDataException("пустой указатель на ТС");
    size_t removed = _vehicles.removeIf([&vehicle](const std::shared_ptr<Vehicle>& v) { return v == vehicle; });
    if (removed == 0) throw ObjectNotFoundException("ТС с госномером " + std::string(vehicle->getRegNumber()) + " не найдено");
    return true;
}

const Collection<Vehicle>& TransportSystem::getVehicles() const { return _vehicles; }
const Collection<Route>& TransportSystem::getRoutes() const { return _routes; }

std::shared_ptr<Route> TransportSystem::findRouteByNumber(int number) const {
    auto route = _routes.find([number](const std::shared_ptr<Route>& r) { return r->getNumber() == number; });
    if (!route) throw ObjectNotFoundException("Маршрут №" + std::to_string(number) + " не найден");
    return route;
}

TransportSystem& TransportSystem::operator+=(std::shared_ptr<Route> route) {
    if (!route) throw InvalidDataException("пустой указатель на маршрут");
    auto existing = _routes.find([&route](const std::shared_ptr<Route>& r) { return r->getNumber() == route->getNumber(); });
    if (existing) throw DuplicateIdException("Маршрут №" + std::to_string(route->getNumber()) + " уже есть в системе");
    _routes.add(route);
    return *this;
}

TransportSystem& TransportSystem::operator-=(std::shared_ptr<Route> route) {
    if (!route) throw InvalidDataException("пустой указатель на маршрут");
    size_t removed = _routes.removeIf([&route](const std::shared_ptr<Route>& r) { return r == route; });
    if (removed == 0) throw ObjectNotFoundException("Маршрут №" + std::to_string(route->getNumber()) + " не найден");
    return *this;
}

void TransportSystem::clear() {
    _routes.clear();
    _vehicles.clear();
    _drivers.clear();
}

std::shared_ptr<Vehicle> TransportSystem::findBestVehicle() const {
    if (_vehicles.empty()) return nullptr;
    auto best = _vehicles.get(0);
    for (const auto& v : _vehicles) {
        if (v->calculateMetric() > best->calculateMetric()) best = v;
    }
    return best;
}