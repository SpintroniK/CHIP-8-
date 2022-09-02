#pragma once

// #include <SFML/Window/Keyboard.hpp>

#include <SDL2/SDL.h>

#include <array>

struct KeyPad
{

    static constexpr auto nbKeys = 16U;

    static constexpr std::array<SDL_Scancode, nbKeys> keys
    {
        SDL_Scancode::SDL_SCANCODE_1,
        SDL_Scancode::SDL_SCANCODE_2,
        SDL_Scancode::SDL_SCANCODE_3,
        SDL_Scancode::SDL_SCANCODE_4,

        SDL_Scancode::SDL_SCANCODE_Q,
        SDL_Scancode::SDL_SCANCODE_W,
        SDL_Scancode::SDL_SCANCODE_E,
        SDL_Scancode::SDL_SCANCODE_R,

        SDL_Scancode::SDL_SCANCODE_A,
        SDL_Scancode::SDL_SCANCODE_S,
        SDL_Scancode::SDL_SCANCODE_D,
        SDL_Scancode::SDL_SCANCODE_F,

        SDL_Scancode::SDL_SCANCODE_Z,
        SDL_Scancode::SDL_SCANCODE_X,
        SDL_Scancode::SDL_SCANCODE_C,
        SDL_Scancode::SDL_SCANCODE_V,
    };
    
};

using KeyArray = std::array<bool, KeyPad::keys.size()>;