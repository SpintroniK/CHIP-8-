#include "Chip8.hpp"
#include "KeyPad.hpp"
#include "Rom.hpp"
#include "Sound.hpp"


#include <SDL2/SDL.h>

#if defined( __EMSCRIPTEN__)
#include <emscripten.h>
#include <emscripten/threading.h>
#endif

#include <array>
#include <chrono>
#include <iostream>
#include <numbers>
#include <numeric>
#include <string>
#include <vector>

using namespace std::chrono;

inline constexpr auto scale = 16;
inline constexpr auto width = 64U * scale;
inline constexpr auto height = 32U * scale;


constexpr auto sampleRate = 44'100;
constexpr auto noteFreq = 110;

static Chip8 chip8;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static bool isWindowOpen = true;
static double gain = 0.;
static double phase = 0.;
static bool isSoundPlaying{false};

void audiomixer(void* userdata, Uint8* stream, int len)
{
    using namespace std::numbers;
    int samples = len / 4;
    short* buf = reinterpret_cast<short*>(stream);

    const auto radiansPerSample = noteFreq * pi / static_cast<double>(sampleRate);
    
    for(std::size_t i = 0; i < samples * 2; ++i)
    {
        phase += radiansPerSample;
        buf[i] = static_cast<short>(std::sin(phase) * gain);
    }
}

void initAudio()
{
    SDL_AudioSpec as;
    as.freq = sampleRate;
    as.format = AUDIO_S16;
    as.channels = 2;
    as.samples = 2048;
    as.callback = audiomixer;

    SDL_AudioSpec as2;
    if(SDL_OpenAudio(&as, &as2) < 0)
    {
        return;
    }

    SDL_PauseAudio(0);

    return;
}   

void pauseAudio()
{
    gain = 0.;
}

void playAudio()
{
    gain = static_cast<double>(std::numeric_limits<std::int16_t>::max()) / 8;
}


void mainLoop()
{

    if(isWindowOpen)
    {
        if(!isSoundPlaying && chip8.GetPlaySound())
        {
            playAudio();
            isSoundPlaying = true;
        }

        if(isSoundPlaying && !chip8.GetPlaySound())
        {
            pauseAudio();
            isSoundPlaying = false;
        }

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
                const auto key = event.key.keysym.scancode;
                switch(key)
                {
                    case SDL_Scancode::SDL_SCANCODE_ESCAPE : isWindowOpen = false; break;
                    default: break;
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

    if(args.size() < 2)
    {
        return EXIT_SUCCESS;
    }

    // Create Rom
    Rom rom;

    #if defined(__EMSCRIPTEN__)
        rom.LoadFromData(args[1]);
    #else
        rom.LoadFromFile(args[1]);
    #endif

    chip8.LoadRom(rom);

    chip8.Start();

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Chip-8-", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    #if defined(__EMSCRIPTEN__)
        emscripten_async_run_in_main_runtime_thread(EM_FUNC_SIG_V, initAudio);
        emscripten_set_main_loop(mainLoop, 0, true);
    #else
        initAudio();
        while(isWindowOpen) mainLoop();
    #endif

    chip8.Stop();

    SDL_DestroyWindow(window);
     
    // close SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}