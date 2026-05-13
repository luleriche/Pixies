#pragma once

#include <SFML/Graphics.hpp>

#include "Partie.hpp"


struct MoteurGraphique{
    sf::RenderWindow* window;
    Partie* partie;
    // La police
    sf::Font font;
    // La texture de fond
    sf::Texture textureFond;
    // Les textures et les emplacements pour les sprites des cartes
    std::array<sf::Texture, 100> texturesCartes;
    std::array<std::optional<sf::Sprite>, 100> spritesCartes;
    unsigned int nbSpritesCartes;
    // Rectangle de selection
    sf::RectangleShape selecteur;
    // Endroit ou navigue le selecteur
    std::string espaceSelecteur;
    // L'indice de l'emplacement de l'espace ou se trouve le selecteur
    unsigned int indiceSelecteur;
    // Les emplacements de chaque cartes, d'abord celles de la pioche puis celles des joueurs
    std::array<sf::Vector2f, 5> emplacementsPioche;
    std::array<sf::Vector2f, 45> emplacementsGrille;
    std::array<sf::Vector2f, 2> emplacementsChoixVisible;
    // Pour savoir qu'est ce qu'on attend
    std::string etatActuel;
    std::string coupActuel;
    // Horloge pour les animations
    sf::Clock clock;
};

void chargerTextures(MoteurGraphique& moteur);

void lancerJeu();

void dessinerTout(MoteurGraphique& mg);

void dessinerCarte(MoteurGraphique& mg, Carte* c, sf::Vector2f centre);

void dessinerDosCarte(MoteurGraphique& mg, sf::Vector2f centre);

// Dessine la grille d'un joueur ainsi que son nom et son nombre de point
void dessinerJoueur(MoteurGraphique& mg, unsigned int joueur, sf::Vector2f centre);

void creerSprites(MoteurGraphique& mg);

void initialiserEmplacementsSelecteur(MoteurGraphique& mg);

void initialiserSelecteur(MoteurGraphique& mg);

std::string recupNomFichier(const Carte& c);

sf::Color couleurCarte(Carte c);

void decalerSelecteur(MoteurGraphique& mg, int cote);
void changerEspaceSelecteur(MoteurGraphique &mg, std::string espace);

void gererUnChoix(MoteurGraphique& mg);