#include "analytics.h"
#include <algorithm>
#include <numeric>

std::shared_ptr<Route> TransportAnalytics::findRouteByNumber(
    const std::map<int, std::shared_ptr<Route>>& routes, int number) {
    auto it = routes.find(number);
    return (it != routes.end()) ? it->second : nullptr;
}

bool TransportAnalytics::isRegNumberUnique(
    const std::set<std::string, std::less<>>& existing,
    std::string_view regNumber) {
    return existing.find(regNumber) == existing.end();
}

std::vector<std::shared_ptr<Vehicle>> TransportAnalytics::sortByMetric(
    const std::vector<std::shared_ptr<Vehicle>>& vehicles) {
    std::vector<std::shared_ptr<Vehicle>> sorted = vehicles;

    std::sort(sorted.begin(), sorted.end(),
        [](const std::shared_ptr<Vehicle>& a,
           const std::shared_ptr<Vehicle>& b) {
            return a->calculateMetric() > b->calculateMetric();
        });

    return sorted;
}

std::pair<std::shared_ptr<Vehicle>, std::shared_ptr<Vehicle>>
TransportAnalytics::findMinMaxMetric(
    const std::vector<std::shared_ptr<Vehicle>>& vehicles) {
    if (vehicles.empty()) return {nullptr, nullptr};

    auto minIt = std::min_element(vehicles.begin(), vehicles.end(),
        [](const std::shared_ptr<Vehicle>& a,
           const std::shared_ptr<Vehicle>& b) {
            return a->calculateMetric() < b->calculateMetric();
        });

    auto maxIt = std::max_element(vehicles.begin(), vehicles.end(),
        [](const std::shared_ptr<Vehicle>& a,
           const std::shared_ptr<Vehicle>& b) {
            return a->calculateMetric() < b->calculateMetric();
        });

    return {*minIt, *maxIt};
}

size_t TransportAnalytics::countByPredicate(
    const std::vector<std::shared_ptr<Vehicle>>& vehicles,
    const std::function<bool(const std::shared_ptr<Vehicle>&)>& pred) {
    return std::count_if(vehicles.begin(), vehicles.end(), pred);
}

std::shared_ptr<Vehicle> TransportAnalytics::findVehicle(
    const std::vector<std::shared_ptr<Vehicle>>& vehicles,
    const std::function<bool(const std::shared_ptr<Vehicle>&)>& pred) {
    auto it = std::find_if(vehicles.begin(), vehicles.end(), pred);
    return (it != vehicles.end()) ? *it : nullptr;
}

double TransportAnalytics::averageMetric(
    const std::vector<std::shared_ptr<Vehicle>>& vehicles) {
    if (vehicles.empty()) return 0.0;

    double sum = std::accumulate(vehicles.begin(), vehicles.end(), 0.0,
        [](double acc, const std::shared_ptr<Vehicle>& v) {
            return acc + v->calculateMetric();
        });

    return sum / vehicles.size();
}

std::vector<std::shared_ptr<Vehicle>> TransportAnalytics::findTopN(
    const std::vector<std::shared_ptr<Vehicle>>& vehicles, size_t n) {
    auto sorted = sortByMetric(vehicles);
    if (sorted.size() > n) sorted.resize(n);
    return sorted;
}

std::vector<std::shared_ptr<Vehicle>> TransportAnalytics::filterByPredicate(
    const std::vector<std::shared_ptr<Vehicle>>& vehicles,
    const std::function<bool(const std::shared_ptr<Vehicle>&)>& pred) {
    std::vector<std::shared_ptr<Vehicle>> result;
    std::copy_if(vehicles.begin(), vehicles.end(),
                  std::back_inserter(result), pred);
    return result;
}