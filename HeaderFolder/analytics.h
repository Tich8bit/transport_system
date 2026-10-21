#pragma once

#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "collection.h"
#include "vehicle.h"

class TransportAnalytics {
public:
    static std::map<std::string, std::vector<std::shared_ptr<Vehicle>>>
    groupByType(const Collection<Vehicle>& vehicles);

    static std::set<std::string>
    getUniqueRegNumbers(const Collection<Vehicle>& vehicles);

    static std::vector<std::shared_ptr<Vehicle>>
    findTopN(const Collection<Vehicle>& vehicles, size_t n);

    static std::pair<std::shared_ptr<Vehicle>, std::shared_ptr<Vehicle>>
    findMinMaxMetric(const Collection<Vehicle>& vehicles);

    static size_t
    countByCapacity(const Collection<Vehicle>& vehicles, int minCapacity);

    static std::vector<std::shared_ptr<Vehicle>>
    filterByYear(const Collection<Vehicle>& vehicles, int minYear);

    static int
    sumCapacity(const Collection<Vehicle>& vehicles);

    static std::vector<std::shared_ptr<Vehicle>>
    sortByTwoCriteria(const Collection<Vehicle>& vehicles);
};