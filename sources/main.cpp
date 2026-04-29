#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <optional>
#include "Carte.hpp"
#include "Defausse.hpp"
#include "Grille.hpp"
#include "Pioche.hpp"
#include "Joueur.hpp"

int main()
{
    srand(time(nullptr));
    Grille ma_Grille;
    initGrille(ma_Grille);
    afficherGrille(ma_Grille);

    Defausse ma_def;
    initDefausse(ma_def);
    lireFichierDefausse("assets/cartes_pixies.txt", ma_def);
    melanger(ma_def);

    for(int i = 0; i < 9; ++i){
        ajouterCarte(ma_Grille, tirerCarteDessus(ma_def));
        afficherGrille(ma_Grille);
    }


    Pioche p;
    initPioche(p);
    remplirPioche(p, ma_def, 3);
    afficher(p);
    afficher(ma_def);
    prendrePioche(p,ma_Grille);
    afficherGrille(ma_Grille);


    std::array<Joueur, 5> joueurs;
    int nbJoueurs = 3;
    creerJoueurs(joueurs, nbJoueurs, &p);

    for (int i = 0; i < nbJoueurs; i++) {
        std::cout << "Joueur " << i+1 << " : " << joueurs[i].surnom
                << " | points : " << joueurs[i].nbPoints << std::endl;
    }

    int joueurCourant = 0;
    std::cout << "--- changement de tour ---" << std::endl;
    for (int i = 0; i < nbJoueurs; i++) {
        std::cout << "C'est au tour de : " << joueurs[joueurCourant].surnom << std::endl;
        changerDeJoueur(joueurCourant, nbJoueurs);
    }

    int joueurDebut = 0;
    finDeTour(joueurDebut, nbJoueurs);
    std::cout << "Prochain tour commence avec : " << joueurs[joueurDebut].surnom << std::endl;
        
    sf::RenderWindow window(sf::VideoMode({1000, 800}), "SFML 3 Image");

    // Chemin relatif vers l'image
    sf::Texture texture;
    if (!texture.loadFromFile("assets/images_pixies/1_b_6.jpg")) {
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