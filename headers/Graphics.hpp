#pragma once

#include <SFML/Graphics.hpp>

#include "Partie.hpp"

struct RenduPartie{
    Partie partie;
    sf::RenderWindow * window;
    std::array<sf::Texture, 100> cartesTextures;
    std::array<std::optional<sf::Sprite>, 100> cartesSprites;
    unsigned int nbSprites;
};

void chargerCartesTextures(RenduPartie& renduPartie, const Partie& partie);

void lancerJeu();

void afficher(RenduPartie& renduPartie);

void creerSprites(RenduPartie& renduPartie);

void chargerCartesTextures(RenduPartie& renduPartie);

std::string recupNomFichier(const Carte& c);

sf::Color couleurCarte(Carte c);