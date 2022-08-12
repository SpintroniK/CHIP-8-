#include "Chip8.hpp"
#include "KeyPad.hpp"
#include "Rom.hpp"

#include <SFML/Graphics.hpp>

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

    constexpr auto scale = 8;
    constexpr auto width = 64U * scale;
    constexpr auto height = 32U * scale;

    // Create Rom
    Rom rom;
    rom.LoadFromFile(args[1]);

    Chip8 chip8;
    chip8.LoadRom(rom);

    chip8.Start();

    std::array<std::uint8_t, width * height> display;
    sf::RectangleShape pixel(sf::Vector2f(scale / 2, scale / 2));
    pixel.setFillColor(sf::Color(0xf8, 0x67, 0x0e));



    sf::RenderWindow window{sf::VideoMode{width, height}, "CHIP-8-"};
    window.setFramerateLimit(60);


    while(window.isOpen())
    {
        const auto start = high_resolution_clock::now();

        sf::Event event;

        while(window.pollEvent(event))
        {

            std::array<bool, KeyPad::keys.size()> currentKeys{};

            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window.close(); 
                    break;
                }

                case sf::Event::KeyPressed:
                {
                    if(event.key.code == sf::Keyboard::Escape) 
                    {
                        window.close();
                    }

                    // Handle KeyPad
                    std::transform(KeyPad::keys.cbegin(), KeyPad::keys.cend(), currentKeys.begin(), [&] (const auto& k)
                    {
                        return k == event.key.code;
                    });

                    const auto i = std::accumulate(currentKeys.begin(), currentKeys.end(), std::uint16_t{}, [](const auto& acc, const auto& k)
                    {
                        return std::rotl(acc, 1) | k;
                    });
                    
                    break;
                }

                case sf::Event::KeyReleased:
                {
                    std::transform(KeyPad::keys.begin(), KeyPad::keys.end(), currentKeys.begin(), currentKeys.begin(), [&](const auto& k, const auto& ck)
                    {
                        return ck & !(event.key.code == k);
                    });


                    break;
                }

                default: break;
            }

        }
    
        window.clear();

        // Get screen data
        const auto pixels = chip8.GetScreen().GetPixels();

        for(std::size_t i = 0; i < 32; i++) 
        {
            for(int j = 0; j < 64; j++) 
            {
                if(pixels[i] & std::rotr(static_cast<std::uint64_t>(0x8000000000000000), j))
                {
                    const std::uint8_t r = 255;
                    const std::uint8_t g = 255;
                    const std::uint8_t b = 255;
                    pixel.setFillColor(sf::Color(r, g, b));
                    pixel.setPosition(static_cast<float>(scale * j), static_cast<float>(scale * i));
                    window.draw(pixel);
                }
            }
        }

        window.display();

        const auto delta = high_resolution_clock::now() - start;
        // std::cout << microseconds::period::den / duration_cast<microseconds>(delta).count() << " fps" << std::endl;
    }

    chip8.Stop();

    return EXIT_SUCCESS;
}