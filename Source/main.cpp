#include "Chip8.hpp"
#include "KeyPad.hpp"
#include "Rom.hpp"
#include "Sound.hpp"


#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <array>
#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std::chrono;

inline constexpr auto scale = 16;
inline constexpr auto width = 64U * scale;
inline constexpr auto height = 32U * scale;

static Chip8 chip8;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static bool isWindowOpen = true;


void mainLoop()
{

    if(isWindowOpen)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            KeyArray currentKeys{};
            switch (event.type)
            {
            case SDL_QUIT: 
            {
                isWindowOpen = false; 
                #ifdef __EMSCRIPTEN__
                    emscripten_cancel_main_loop();
                #endif
                break;
            }
            case SDL_KEYDOWN:
            {
                // Handle KeyPad
                std::transform(KeyPad::keys.cbegin(), KeyPad::keys.cend(), currentKeys.begin(), [&] (const auto& k)
                {
                    return k == event.key.keysym.scancode;
                });

                chip8.SetKeys(currentKeys);
                break;
            }
            case SDL_KEYUP: 
            {
                const auto key = event.key.keysym.sym;
                switch(key)
                {
                    case SDL_KeyCode::SDLK_ESCAPE : isWindowOpen = false; break;
                }

                std::transform(KeyPad::keys.begin(), KeyPad::keys.end(), currentKeys.begin(), currentKeys.begin(), [&](const auto& k, const auto& ck)
                {
                    return ck & !(key == k);
                });

                chip8.SetKeys(currentKeys);
                break;
            }
            
            default:
                break;
            }
        }


        // Get screen data
        const auto pixels = chip8.GetScreen().GetPixels();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for(std::size_t i = 0; i < screenHeight; i++) 
        {
            for(std::size_t j = 0; j < screenWidth; j++) 
            {
                if(pixels[i][screenWidth - 1 - j])
                {
                    // pixel.setPosition(static_cast<float>(scale * j), static_cast<float>(scale * i));
                    // window.draw(pixel);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    const auto rect = SDL_Rect{static_cast<int>(scale * j), static_cast<int>(scale * i), scale, scale};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char** argv)
{

    const std::vector<std::string> args(argv, argv + argc);

    // if(args.size() < 2)
    // {
    //     return EXIT_SUCCESS;
    // }

    // Create Rom
    Rom rom;
    rom.LoadFromFile("roms/BRIX");

    chip8.LoadRom(rom);

    chip8.Start();

    // constexpr auto sampleRate = 44'100;
    // constexpr auto noteFreq = 440;
    // const auto beep = Sound::GenerateSound<std::int16_t>(noteFreq, sampleRate);

    // sf::SoundBuffer soundBuffer;
    // if(!soundBuffer.loadFromSamples(beep.data(), sampleRate, 1, sampleRate)) 
    // {
    //     return EXIT_FAILURE;
    // }

    // sf::Sound sound;
    // sound.setBuffer(soundBuffer);
    // sound.setLoop(true);

    // bool isSoundPlaying = false;

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Chip-8-", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(mainLoop, 0, true);
    #endif

    //     // const auto start = high_resolution_clock::now();


    //     if(!isSoundPlaying && chip8.GetPlaySound())
    //     {
    //         sound.play();
    //         isSoundPlaying = true;
    //     }

    //     if(isSoundPlaying && !chip8.GetPlaySound())
    //     {
    //         sound.stop();
    //         isSoundPlaying = false;
    //     }


        // const auto delta = high_resolution_clock::now() - start;
        // std::cout << microseconds::period::den / duration_cast<microseconds>(delta).count() << " fps" << std::endl;
    // }

    chip8.Stop();

    SDL_DestroyWindow(window);
     
    // close SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}