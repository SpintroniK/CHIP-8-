#include "Chip8.hpp"
#include "KeyPad.hpp"
#include "Rom.hpp"
#include "Sound.hpp"


#include <SDL2/SDL.h>

#include <array>
#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std::chrono;

int main(int argc, char** argv)
{

    const std::vector<std::string> args(argv, argv + argc);

    if(args.size() < 2)
    {
        return EXIT_SUCCESS;
    }

    constexpr auto scale = 16;
    constexpr auto width = 64U * scale;
    constexpr auto height = 32U * scale;

    // Create Rom
    Rom rom;
    rom.LoadFromFile(args[1]);

    Chip8 chip8;
    chip8.LoadRom(rom);

    chip8.Start();

    // sf::RectangleShape pixel(sf::Vector2f(scale, scale));
    // pixel.setFillColor(sf::Color(0xff, 0xff, 0xff));


    // sf::RenderWindow window{sf::VideoMode{width, height}, {}};
    // window.setFramerateLimit(60);

    constexpr auto sampleRate = 44'100;
    constexpr auto noteFreq = 440;
    const auto beep = Sound::GenerateSound<std::int16_t>(noteFreq, sampleRate);

    // sf::SoundBuffer soundBuffer;
    // if(!soundBuffer.loadFromSamples(beep.data(), sampleRate, 1, sampleRate)) 
    // {
    //     return EXIT_FAILURE;
    // }

    // sf::Sound sound;
    // sound.setBuffer(soundBuffer);
    // sound.setLoop(true);

    bool isSoundPlaying = false;

    

    SDL_Init(SDL_INIT_EVERYTHING);
    auto window = SDL_CreateWindow("Chip-8-", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    bool isWindowOpen = true;
    while(isWindowOpen)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT: isWindowOpen = false; break;
            case SDL_KEYUP: 
            {
                const auto key = event.key.keysym.sym;
                switch(key)
                {
                    case SDL_KeyCode::SDLK_ESCAPE : isWindowOpen = false; break;
                }
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
                    const auto rect = SDL_Rect{scale * j, scale * i, scale, scale};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    //     // const auto start = high_resolution_clock::now();

    //     sf::Event event;

    //     while(window.pollEvent(event))
    //     {

    //         KeyArray currentKeys{};

    //         switch(event.type)
    //         {
    //             case sf::Event::Closed:
    //             {
    //                 window.close(); 
    //                 break;
    //             }

    //             case sf::Event::KeyPressed:
    //             {
    //                 if(event.key.code == sf::Keyboard::Escape) 
    //                 {
    //                     window.close();
    //                 }

    //                 // Handle KeyPad
    //                 std::transform(KeyPad::keys.cbegin(), KeyPad::keys.cend(), currentKeys.begin(), [&] (const auto& k)
    //                 {
    //                     return k == event.key.code;
    //                 });

    //                 chip8.SetKeys(currentKeys);

    //                 break;
    //             }

    //             case sf::Event::KeyReleased:
    //             {
    //                 std::transform(KeyPad::keys.begin(), KeyPad::keys.end(), currentKeys.begin(), currentKeys.begin(), [&](const auto& k, const auto& ck)
    //                 {
    //                     return ck & !(event.key.code == k);
    //                 });

    //                 chip8.SetKeys(currentKeys);

    //                 break;
    //             }

    //             default: break;
    //         }

    //     }

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
    
    //     window.clear();


        // window.display();

        // const auto delta = high_resolution_clock::now() - start;
        // std::cout << microseconds::period::den / duration_cast<microseconds>(delta).count() << " fps" << std::endl;
    // }

    chip8.Stop();

    SDL_DestroyWindow(window);
     
    // close SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}