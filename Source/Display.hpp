#pragma once

#include "Defintions.hpp"

#include <array>
#include <bitset>
#include <iostream>
#include <limits>
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
        // pixels.resize(screenHeight); 
        std::fill(pixels.begin(), pixels.end(), std::bitset<screenWidth>{});
    };

    Screen(const Screen& other)
    {
        pixels = other.pixels;
    }


    ~Screen() = default;


    void Clear()
    {
        std::fill(pixels.begin(), pixels.end(), std::bitset<screenWidth>{});
    }

    bool DrawSprite(const Sprite& sp)
    {
        const Byte_t y = sp.GetY() % screenHeight;
        const Byte_t x = sp.GetX();
        const auto data = sp.GetData();
        const auto spriteHeight = data.size();

        // std::cout << "Draw sprite (" << +x << ", " << +y << ")" << std::endl;

        // Same as std::rotl, but for std::bitset
        const auto rotl = []<std::size_t N>(const std::bitset<N>& x_, std::size_t s) { const auto r = s % N; return (x_ << r) | (x_ >> (N - r)); };

        bool collision = false;

        for(auto i = y; i < y + spriteHeight; ++i)
        {
            auto& screenline = pixels[i % screenHeight];

            const auto spriteLine = std::bitset<screenWidth>{data[i % screenHeight - y]};
            const auto prevScreenLine = screenline;

            screenline ^= rotl(spriteLine, screenWidth - 1 - spriteWidth - x);

            collision |= (prevScreenLine & ~screenline).any();
        }

        return collision;
    }

    auto GetPixels() const
    {
        return pixels;
    }

private:

    std::array<std::bitset<screenWidth>, screenHeight> pixels; // pixels[y][x]
};
