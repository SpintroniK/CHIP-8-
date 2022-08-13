#pragma once

#include "Defintions.hpp"

#include <atomic>

template <typename T>
class Timer
{

public:
    Timer() = default;
    ~Timer() = default;

    auto Count() const noexcept
    {
        return value.load(std::memory_order_acquire);
    }

    void Set(const T& v)
    {
        value.store(v, std::memory_order_release);
    }

    void Dec()
    {
        if(value.load(std::memory_order_relaxed) == 0)
        {
            return;
        }

        value.fetch_sub(1, std::memory_order_relaxed);
    }

private:

    std::atomic<T> value{};

};