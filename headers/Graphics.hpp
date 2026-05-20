#pragma once

#include <SFML/Graphics.hpp>

#include "Partie.hpp"


struct MoteurGraphique{
    sf::RenderWindow* window;
    Partie partie;

    sf::Font font; // La police
    
    sf::Texture textureFond; // La texture de fond

    sf::Texture textureSelecteur; // La texture du selecteur
    std::optional<sf::Sprite> selecteur; // Sprite du selecteur
    std::string espaceSelecteur; // Nom de l'espace ou navigue le sélecteur
    int indiceSelecteur; // Indice de l'endroit ou se trouve le sélecteur dans son espace
    
    std::array<sf::Vector2f, 5> emplacementsPioche; // Les différents emplacements où peuvent se trouver les cartes ou le sélecteur
    std::array<std::array<sf::Vector2f, 9>, 5> emplacementsGrille;
    std::array<sf::Vector2f, 2> emplacementsChoixVisible;

    sf::RectangleShape fondChoixVisible; // Rectangle de fond lors du choix de quelle carte garder visible

    std::array<sf::Texture, 100> texturesCartes; // Les textures et les emplacements pour les sprites des cartes
    sf::Vector2f tailleCartes;
    std::array<std::optional<sf::Sprite>, 100> spritesCartes; // Les sprites des cartes
    unsigned int nbSpritesCartes;

    sf::Vector2f decalageDos; // Le décalage des cartes face cachée par rapport à celle face visible
    
    
    // L'emplacement du texte pioche
    sf::Vector2f txtPiochePosition;
    // Pour savoir qu'est ce qu'on attend
    std::string coupActuel;
    // Horloge pour les animations
    sf::Clock clock;
};

// Charge toutes les textures nécéssaire au jeu et initialises tous les sprites(leur taille, origines etc..)
void initialiserTexturesEtSprites(MoteurGraphique& mg);

void lancerJeu();

void dessinerTout(MoteurGraphique& mg);

void dessinerCarte(MoteurGraphique& mg, Carte* c, sf::Vector2f centre);

void dessinerDosCarte(MoteurGraphique& mg, sf::Vector2f centre);

// Dessine la grille d'un joueur ainsi que son nom et son nombre de point
void dessinerJoueur(MoteurGraphique& mg, unsigned int joueur);

void initialiserDispositionEcran(MoteurGraphique& mg);

std::string recupNomFichier(const Carte& c);

sf::Color couleurCarte(Carte c);

// Change l'indice du sélecteur et également sa position
void decalerSelecteur(MoteurGraphique& mg, int cote);

void changerEspaceSelecteur(MoteurGraphique &mg, std::string espace);

void gererUnChoix(MoteurGraphique& mg);

void gererFinDeCoup(MoteurGraphique& mg);