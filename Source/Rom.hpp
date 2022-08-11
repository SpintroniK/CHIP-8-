#pragma once

#include "Defintions.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

class Rom
{

public:
    Rom() = default;
    ~Rom() = default;

    void LoadFromFile(std::string_view fileLocation)
    {
        const std::filesystem::path path{fileLocation};

        std::ifstream file{path.string(), std::ios::binary};

        // const auto fileSize = std::filesystem::file_size(path);
        // TODO: Check size here

        // Read file into string
        const std::string str{(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
        
        // Copy string into data array
        std::copy(str.cbegin(), str.cend(), data.begin());
    }

    auto GetData() const { return data; }

private:

    std::array<Byte_t, ramSize> data{};

};