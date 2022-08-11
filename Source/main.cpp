#include "Cpu.hpp"
#include "KeyPad.hpp"
#include "Rom.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std::chrono;

int main(int argc, char** argv)
{
    constexpr auto width = 640U;
    constexpr auto height = 480U;

    // Create CPU
    Cpu cpu;

    // Create Rom
    Rom rom;
    rom.LoadFromFile("/home/jeremy/Desktop/CHIP-8-Emulator/roms/BRIX");

    cpu.LoadProgram(rom.GetData());

    std::vector<std::uint8_t> display(width * height);
    sf::RectangleShape pixel(sf::Vector2f(2, 2));
    pixel.setFillColor(sf::Color(0xf8, 0x67, 0x0e));



    sf::RenderWindow window{sf::VideoMode{width, height}, "CHIP-8-"};
    // window.setFramerateLimit(60);


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

        for(std::size_t i = 0; i < 64; i++) {
            for(std::size_t j = 0; j < 128; j++) {
                // if(display.at(j + i * 128) == 1) 
                {
                    std::uint8_t r = 255;
                    std::uint8_t g = 255;
                    std::uint8_t b = 255;
                    pixel.setFillColor(sf::Color(r, g, b));
                    pixel.setPosition(2 * j, 2 * i);
                    window.draw(pixel);
                }
            }
        }

        window.display();

        const auto delta = high_resolution_clock::now() - start;
        // std::cout << microseconds::period::den / duration_cast<microseconds>(delta).count() << " fps" << std::endl;
    }

    return EXIT_SUCCESS;
}