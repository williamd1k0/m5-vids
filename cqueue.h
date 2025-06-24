#pragma once
#include <deque>

template<typename T>
class CircularQueue {
private:
    std::deque<T> items;
    size_t current_index;

public:
    CircularQueue() : current_index(0) {}

    void push(const T& value) {
        items.push_back(value);
    }

    T next() {
        if (items.empty()) throw std::out_of_range("Queue is empty");
        T value = items[current_index];
        current_index = (current_index + 1) % items.size();
        return value;
    }

    void rollback() {
        if (items.empty()) throw std::out_of_range("Queue is empty");
        current_index = (current_index < 2) ? items.size() - (2 - current_index) : current_index - 2;
    }
};
