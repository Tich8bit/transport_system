#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "collection.h"

template <typename T>
std::shared_ptr<T> maxByMetric(const Collection<T>& collection) {
    std::shared_ptr<T> best = nullptr;
    for (const auto& item : collection) {
        if (!item) continue;
        if (!best || item->calculateMetric() > best->calculateMetric()) {
            best = item;
        }
    }
    return best;
}

template <typename T>
double averageMetric(const Collection<T>& collection) {
    if (collection.size() == 0) return 0.0;

    double sum = 0.0;
    size_t count = 0;
    for (const auto& item : collection) {
        if (item) {
            sum += item->calculateMetric();
            ++count;
        }
    }
    return (count > 0) ? sum / count : 0.0;
}

template <typename T, typename Comparator>
std::vector<std::shared_ptr<T>> sortBy(const Collection<T>& collection, Comparator comp) {
    std::vector<std::shared_ptr<T>> result(collection.begin(), collection.end());
    std::sort(result.begin(), result.end(), comp);
    return result;
}