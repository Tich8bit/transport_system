#pragma once

#include <iostream>
#include "collection.h"
#include <algorithm>
#include "exceptions.h"

template <typename T>
const std::vector<std::shared_ptr<T>>& Collection<T>::getItems() const {
    return _items;
}

template <typename T>
void Collection<T>::add(const std::shared_ptr<T>& item) {
    _items.push_back(item);
}

template <typename T>
void Collection<T>::removeAt(size_t index) {
    if (index >= _items.size()) {
        std::cout << "ОШИБКА: индекс вне диапазона\n";
        return;
    }
    _items.erase(_items.begin() + index);
}

template <typename T>
template <typename Predicate>
size_t Collection<T>::removeIf(Predicate pred) {
    size_t removed = 0;
    for (auto it = _items.begin(); it != _items.end(); ) {
        if (*it && pred(*it)) {
            it = _items.erase(it);
            ++removed;
        } else {
            ++it;
        }
    }
    return removed;
}

template <typename T>
std::shared_ptr<T> Collection<T>::get(size_t index) const {
    if (index >= _items.size()) {
        std::cout << "ОШИБКА: индекс вне диапазона\n";
        return nullptr;
    }
    return _items[index];
}

template <typename T>
template <typename Predicate>
std::shared_ptr<T> Collection<T>::find(Predicate pred) const {
    for (const auto& item : _items) {
        if (item && pred(item)) {
            return item;
        }
    }
    return nullptr;
}

template <typename T>
template <typename Predicate>
Collection<T> Collection<T>::findAll(Predicate pred) const {
    Collection<T> result;
    for (const auto& item : _items) {
        if (item && pred(item)) {
            result.add(item);
        }
    }
    return result;
}

template <typename T>
size_t Collection<T>::size() const {
    return _items.size();
}

template <typename T>
bool Collection<T>::empty() const {
    return _items.empty();
}

template <typename T>
void Collection<T>::clear() {
    _items.clear();
}

template <typename T>
void Collection<T>::print(std::ostream& os) const {
    if (_items.empty()) {
        os << " (коллекция пуста)\n";
        return;
    }
    for (size_t i = 0; i < _items.size(); ++i) {
        os << "[" << i << "] ";
        if (_items[i]) {
            os << *_items[i];
        } else {
            os << " (null)";
        }
        os << "\n";
    }
}

template <typename T>
auto Collection<T>::begin() const {
    return _items.begin();
}

template <typename T>
auto Collection<T>::end() const {
    return _items.end();
}