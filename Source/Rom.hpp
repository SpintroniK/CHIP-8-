#pragma once

#include "Defintions.hpp"

#include <array>
#include <fstream>

class Rom
{

public:
    Rom() = default;
    ~Rom() = default;

    auto GetData() const { return data; }

private:

    std::array<Byte_t, ramSize> data;

};