#pragma once

#include "Defintions.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

static std::string base64_decode(const std::string &in) {

    std::string out;

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val=0, valb=-8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}

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
        
        std::cout << "Rom size = " << str.length() << std::endl;
        
        // Copy string into data array
        std::copy(str.cbegin(), str.cend(), data.begin());
    }

    void LoadFromData(const std::string& input)
    {
        const auto str = base64_decode(input);

        std::copy(str.cbegin(), str.cend(), data.begin());
    }

    auto GetData() const { return data; }

private:

    std::array<Byte_t, ramSize - programLoadAddress> data{};

};