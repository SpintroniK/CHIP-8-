#pragma once

#include "Defintions.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <climits>
#include <iostream>
#include <vector>

class Sprite
{
    
public:
    Sprite() = default;

    Sprite(Byte_t x, Byte_t y, const std::vector<Byte_t>& dat)
    : x_{x}, y_{y}, data{dat}
    {
    }

    ~Sprite() = default;

    auto GetX() const { return x_; }
    auto GetY() const { return y_; }
    auto GetData() const { return data; }

private:

    Byte_t x_{};
    Byte_t y_{};

    std::vector<Byte_t> data;

};

class Screen
{
    
public:
    Screen()
    { 
        pixels.resize(screenHeight); 
        std::fill(pixels.begin(), pixels.end(), Byte_t{});
    };

    Screen(const Screen& other)
    {
        pixels = other.pixels;
    }


    ~Screen() = default;


    void Clear()
    {
        //std::fill(pixels.begin(), pixels.end(), std::bitset<screenWidth>{});
    }

    bool DrawSprite(const Sprite& s)
    {
        const Byte_t y = s.GetY();
        const Byte_t x = s.GetX();
        const auto data = s.GetData();

        std::cout << "Draw sprite (" << +x << ", " << +y << ")" << std::endl;

        for(auto lineNb = y;lineNb < y + data.size(); ++lineNb)
        {
            pixels[lineNb] ^= std::rotl(static_cast<std::uint64_t>(data[lineNb - y]), 55 - x);
        }

        return false;
    }

    auto GetPixels() const
    {
        return pixels;
    }

private:

    std::vector<std::uint64_t> pixels; // pixels[y][x]
};
