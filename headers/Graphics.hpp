#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include "Partie.hpp"


/*
Les fichiers Graphics.hpp et .cpp ne sont pas totalement documenté et optimisé par manque de temps.
Il faudrait refaire les bases du moteur grpahique comme les boucles de jeu, l'initialisation de la partie, ou l'écran de fin.
*/

// Structure pour le moteur graphique
struct MoteurGraphique{
    sf::RenderWindow* window;
    Partie partie;

    sf::Font font; // La police
    
    sf::Texture textureFond; // La texture de fond

    sf::Texture textureSelecteur; // La texture du selecteur
    std::optional<sf::Sprite> selecteur; // Sprite du selecteur
    sf::Vector2f positionViseeSelecteur;
    bool selecteurEnMouvement;
    std::string espaceSelecteur; // Nom de l'espace ou navigue le sélecteur
    int indiceSelecteur; // Indice de l'endroit ou se trouve le sélecteur dans son espace
    
    std::array<sf::Vector2f, 5> emplacementsPioche; // Les différents emplacements où peuvent se trouver les cartes ou le sélecteur
    std::array<std::array<sf::Vector2f, 9>, 5> emplacementsGrille;
    std::array<sf::Vector2f, 2> emplacementsChoixVisible;

    sf::RectangleShape fondChoixVisible; // Rectangle de fond lors du choix de quelle carte garder visible
    sf::RectangleShape fondOrdi;

    std::array<sf::Texture, 100> texturesCartes; // Les textures et les emplacements pour les sprites des cartes
    sf::Vector2f tailleCartes;
    std::array<std::optional<sf::Sprite>, 100> spritesCartes; // Les sprites des cartes
    std::array<sf::Vector2f, 100> scaleFacteursCartes; // Les facteurs de scales des cartes pour pouvoir faire des petites anim
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

// Lance un jeu en créeant une fenêtre après avoir demandé les joueurs
void lancerJeu();

// Dessine tout sur l'écran d'un moteur grpahique
void dessinerTout(MoteurGraphique& mg);

// Dessine une carte sur la fenetre d'un mg
void dessinerCarte(MoteurGraphique& mg, Carte* c, sf::Vector2f centre, float facteurTaille);

// Dessine le dos d'une carte sur la fenetre d'un mg
void dessinerDosCarte(MoteurGraphique& mg, sf::Vector2f centre, float facteurTaille);

// Dessine la grille d'un joueur ainsi que son nom et son nombre de point sur un mg
void dessinerJoueur(MoteurGraphique& mg, unsigned int joueur);

// Initialise les emplacements des cartes selon le nb de joueurs d'un mg
void initialiserDispositionEcran(MoteurGraphique& mg);

// Récupère le nom du fichier qui correspond à une carte
std::string recupNomFichier(const Carte& c);

// Recupère la couleur d'une carte
sf::Color couleurCarte(Carte c);

// Décale le selecteur d'un cote dans son espace
void decalerSelecteur(MoteurGraphique& mg, int cote);

// Change l'espace du sélecteur d'un mg
void changerEspaceSelecteur(MoteurGraphique &mg, std::string espace);

// Gère un choix, utiliser quand la touche entrer est pressée
void gererUnChoix(MoteurGraphique& mg);

// Gère le moteur graphique et sa partie après avoir jouer un coup(remettre le selecteur au bon endroit)
void gererFinDeCoup(MoteurGraphique& mg);

// Met à jour la position du sélecteur pour un effet bien stylé nan vous trouvez pas ?
void mettreAJourPositionSelecteur(MoteurGraphique& mg);