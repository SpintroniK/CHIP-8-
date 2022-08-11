#pragma once

#include "Defintions.hpp"

#include <algorithm>
#include <array>
#include <cstddef>

template <Address_t Size>
class Memory
{
    
public:
    Memory() = default;
    ~Memory() = default;

    template <typename T>
    void WriteChunk(const T& data, Address_t address)
    {
        std::copy(data.begin(), data.end(), mem.begin() + address);
    }

private:

    std::array<Byte_t, Size> mem;
};