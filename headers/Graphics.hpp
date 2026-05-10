#pragma once

#include <SFML/Graphics.hpp>

#include "Partie.hpp"

struct RenduPartie{
    Partie* partie;
    sf::RenderWindow * window;
    sf::Texture fondTexture;
    std::array<sf::Texture, 100> cartesTextures;
    std::array<std::optional<sf::Sprite>, 100> cartesSprites;
    unsigned int nbSprites;

    std::array<sf::FloatRect, 5> zonesPioches;
};

void chargerCartesTextures(RenduPartie& renduPartie, const Partie& partie);

void lancerJeu();

void afficher(RenduPartie& renduPartie);

void afficherRenduCarte(RenduPartie& rendu, Carte* c, sf::Vector2f centre);

void afficherRenduGrille(RenduPartie& rendu, unsigned int joueur, sf::Vector2f centre);

void creerSprites(RenduPartie& renduPartie);

void chargerCartesTextures(RenduPartie& renduPartie);

std::string recupNomFichier(const Carte& c);

sf::Color couleurCarte(Carte c);