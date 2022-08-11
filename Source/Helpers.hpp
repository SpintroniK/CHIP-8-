#pragma once

#include "Defintions.hpp"

#include <bit>

// Helper functions
inline constexpr Address_t GetLowest12bitAddr(Address_t address)
{
    return address & Address_t{0x0FFF};
}

inline constexpr Byte_t GetLowestByte(Address_t address)
{
    return static_cast<Byte_t>(address & Address_t{0x00FF});
}

inline constexpr Byte_t GetLowestNibble(Address_t address)
{
    return address & Address_t{0x0F};
}

template <std::uint8_t N>
auto GetNibble(auto address)
{
    constexpr auto NbNibblesPerByte = 2U;
    constexpr auto Nmax = sizeof(decltype(address)) * NbNibblesPerByte;

    static_assert(N < Nmax);

    constexpr auto NbBitsPerNibble = 4U;

    return static_cast<Byte_t>(std::rotr(address, NbBitsPerNibble * (Nmax - N - 1)) & 0xF);
}