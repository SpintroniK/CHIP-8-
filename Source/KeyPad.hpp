#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <array>

struct KeyPad
{

    static constexpr auto nbKeys = 16U;

    static constexpr std::array<sf::Keyboard::Key, nbKeys> keys
    {
        sf::Keyboard::Num1,
        sf::Keyboard::Num2,
        sf::Keyboard::Num3,
        sf::Keyboard::Num4,
        sf::Keyboard::A,
        sf::Keyboard::Z,
        sf::Keyboard::E,
        sf::Keyboard::R,
        sf::Keyboard::Q,
        sf::Keyboard::S,
        sf::Keyboard::D,
        sf::Keyboard::F,
        sf::Keyboard::W,
        sf::Keyboard::X,
        sf::Keyboard::C,
        sf::Keyboard::V,
    };
    
};

using KeyArray = std::array<bool, KeyPad::keys.size()>;