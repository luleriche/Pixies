#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode({1000, 800}), "SFML 3 Image");

    // Chemin relatif vers l'image
    sf::Texture texture;
    if (!texture.loadFromFile("../images_pixies/Joker_2.jpg")) {
        std::cerr << "Erreur : impossible de charger l'image !" << std::endl;
        return -1;
    }

    // Créer le sprite à partir de la texture
    sf::Sprite sprite(texture);

    sf::Vector2u textureSize = texture.getSize();  // largeur et hauteur de l'image originale
    float targetWidth = 400.f;
    float targetHeight = 600.f;

    // Calcul du facteur de redimensionnement pour garder les proportions
    float scaleX = targetWidth / textureSize.x;
    float scaleY = targetHeight / textureSize.y;
    float scale = std::min(scaleX, scaleY);  // prend le plus petit pour que l'image tienne dans le cadre
    sprite.setScale({scale, scale});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    return 0;
}