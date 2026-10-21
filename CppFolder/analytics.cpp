#include "analytics.h"
#include <algorithm>
#include <numeric>
#include <iterator>

std::map<std::string, std::vector<std::shared_ptr<Vehicle>>>
TransportAnalytics::groupByType(const Collection<Vehicle>& vehicles) {
    std::map<std::string, std::vector<std::shared_ptr<Vehicle>>> groups;

    std::for_each(vehicles.begin(), vehicles.end(),
        [&groups](const std::shared_ptr<Vehicle>& v) {
            groups[v->getType()].push_back(v);
        });

    return groups;
}

std::set<std::string>
TransportAnalytics::getUniqueRegNumbers(const Collection<Vehicle>& vehicles) {
    std::set<std::string> nums;

    std::transform(vehicles.begin(), vehicles.end(),
        std::inserter(nums, nums.begin()),
        [](const std::shared_ptr<Vehicle>& v) {
            return std::string(v->getRegNumber());
        });

    return nums;
}

std::vector<std::shared_ptr<Vehicle>>
TransportAnalytics::findTopN(const Collection<Vehicle>& vehicles, size_t n) {
    std::vector<std::shared_ptr<Vehicle>> sorted;

    std::copy(vehicles.begin(), vehicles.end(), std::back_inserter(sorted));

    std::sort(sorted.begin(), sorted.end(),
        [](const std::shared_ptr<Vehicle>& a, const std::shared_ptr<Vehicle>& b) {
            return a->calculateMetric() > b->calculateMetric();
        });

    if (sorted.size() > n) sorted.resize(n);
    return sorted;
}

std::pair<std::shared_ptr<Vehicle>, std::shared_ptr<Vehicle>>
TransportAnalytics::findMinMaxMetric(const Collection<Vehicle>& vehicles) {
    if (vehicles.empty()) return {nullptr, nullptr};

    auto [minIt, maxIt] = std::minmax_element(vehicles.begin(), vehicles.end(),
        [](const std::shared_ptr<Vehicle>& a, const std::shared_ptr<Vehicle>& b) {
            return a->calculateMetric() < b->calculateMetric();
        });

    return {*minIt, *maxIt};
}

size_t TransportAnalytics::countByCapacity(const Collection<Vehicle>& vehicles, int minCapacity) {
    return std::count_if(vehicles.begin(), vehicles.end(),
        [minCapacity](const std::shared_ptr<Vehicle>& v) {
            return v->getCapacity() > minCapacity;
        });
}

std::vector<std::shared_ptr<Vehicle>>
TransportAnalytics::filterByYear(const Collection<Vehicle>& vehicles, int minYear) {
    std::vector<std::shared_ptr<Vehicle>> result;

    std::copy_if(vehicles.begin(), vehicles.end(),
        std::back_inserter(result),
        [minYear](const std::shared_ptr<Vehicle>& v) {
            return v->getYear() >= minYear;
        });

    return result;
}

int TransportAnalytics::sumCapacity(const Collection<Vehicle>& vehicles) {
    return std::accumulate(vehicles.begin(), vehicles.end(), 0,
        [](int acc, const std::shared_ptr<Vehicle>& v) {
            return acc + v->getCapacity();
        });
}

std::vector<std::shared_ptr<Vehicle>>
TransportAnalytics::sortByTwoCriteria(const Collection<Vehicle>& vehicles) {
    std::vector<std::shared_ptr<Vehicle>> sorted;
    std::copy(vehicles.begin(), vehicles.end(), std::back_inserter(sorted));

    std::sort(sorted.begin(), sorted.end(),
        [](const std::shared_ptr<Vehicle>& a, const std::shared_ptr<Vehicle>& b) {
            if (a->getCapacity() != b->getCapacity()) {
                return a->getCapacity() > b->getCapacity();
            }
            return a->getYear() > b->getYear();
        });

    return sorted;
}