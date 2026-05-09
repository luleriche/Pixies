#include "Graphics.hpp"
#include <SFML/Graphics.hpp>

void lancerJeu(){
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({500, 500}), "Pixies");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.display();
    }
}