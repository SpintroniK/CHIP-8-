#pragma once

#include "Defintions.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <iostream>
#include <limits>
#include <numeric>

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

    auto ReadChunk(Address_t address, Byte_t length) -> std::vector<Byte_t>
    {
        std::vector<Byte_t> d(length);
        std::copy(mem.begin() + address, mem.begin() + address + length, d.begin());
        return d;
    }

    void WriteByte(Byte_t byte, Address_t address)
    {
        mem[address] = byte;
    }

    template <typename T>
    T Read(Address_t address)
    {
        return std::accumulate(mem.begin() + address, mem.begin() + address + sizeof(T), T{}, [](const auto& acc, const auto& x)
        {
            return std::rotl(acc, std::numeric_limits<Byte_t>::digits) | x;
        });
    }

private:

    std::array<Byte_t, Size> mem;
};