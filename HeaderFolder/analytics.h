#pragma once

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include "vehicle.h"
#include "route.h"

class TransportAnalytics {
public:
    static std::shared_ptr<Route> findRouteByNumber(
        const std::map<int, std::shared_ptr<Route>>& routes,
        int number);

    static bool isRegNumberUnique(
        const std::set<std::string>& existing,
        const std::string& regNumber);

    static std::vector<std::shared_ptr<Vehicle>> sortByMetric(
        const std::vector<std::shared_ptr<Vehicle>>& vehicles);

    static std::pair<std::shared_ptr<Vehicle>, std::shared_ptr<Vehicle>>
    findMinMaxMetric(
        const std::vector<std::shared_ptr<Vehicle>>& vehicles);

    static size_t countByPredicate(
        const std::vector<std::shared_ptr<Vehicle>>& vehicles,
        std::function<bool(const std::shared_ptr<Vehicle>&)> pred);

    static std::shared_ptr<Vehicle> findVehicle(
        const std::vector<std::shared_ptr<Vehicle>>& vehicles,
        std::function<bool(const std::shared_ptr<Vehicle>&)> pred);

    static double averageMetric(
        const std::vector<std::shared_ptr<Vehicle>>& vehicles);

    static std::vector<std::shared_ptr<Vehicle>> findTopN(
        const std::vector<std::shared_ptr<Vehicle>>& vehicles,
        size_t n);

    static std::vector<std::shared_ptr<Vehicle>> filterByPredicate(
        const std::vector<std::shared_ptr<Vehicle>>& vehicles,
        std::function<bool(const std::shared_ptr<Vehicle>&)> pred);
};