#include <SFML/Graphics.hpp>

#include <iostream>

int main(int argc, char** argv)
{
    constexpr auto width = 640U;
    constexpr auto height = 480U;

    sf::RenderWindow window{sf::VideoMode{width, height}, "CHIP-8-"};

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }

    return EXIT_SUCCESS;
}