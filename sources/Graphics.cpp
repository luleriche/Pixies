#include "Graphics.hpp"
#include <SFML/Graphics.hpp>

void lancerJeu(){
    // Création de la partie
    Partie partie;

    // Création des cartes dans la mémoire à l'aide d'une boite de cartes
    BoiteCartes boite;
    creerCartesAvecFichier("assets/cartes_pixies.txt", boite);
    partie.boite = boite;

    // Chargement des cartes et mélange
    Defausse defausse;
    initDefausse(defausse);
    remplir(boite, defausse);
    melanger(defausse);
    partie.defausse = defausse;

    // Création de la pioche
    Pioche pioche;
    partie.pioche = pioche;
    
    partie.nombreJoueurs = 2;
    creerJoueurs(partie.joueurs, partie.nombreJoueurs, &partie.pioche);

    // On initialise la pioche
    if(partie.nombreJoueurs == 2)
        initPioche(partie.pioche, 4);
    else
        initPioche(partie.pioche, partie.nombreJoueurs);
    remplirPioche(partie.pioche, partie.defausse);
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1000, 700}), "Pixies");
    RenduPartie renduPartie;
    renduPartie.partie = partie;
    chargerCartesTextures(renduPartie);
    renduPartie.window = &window;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);
        afficher(renduPartie);
        window.display();
    }
}

void chargerCartesTextures(RenduPartie& renduPartie){
    renduPartie.nbSprites = renduPartie.partie.boite.nbCartes;
    std::cout << "Chargement des textures." << std::endl;
    for(unsigned int i = 0; i < renduPartie.nbSprites; ++i){
        
        std::string nomFichier = recupNomFichier(renduPartie.partie.boite.cartes[i]);
        const sf::Texture texture("assets/images_pixies/"+nomFichier);
        std::cout << nomFichier << " chargé." << std::endl;
        renduPartie.cartesTextures[i] = texture;
        renduPartie.cartesSprites[i] = sf::Sprite(renduPartie.cartesTextures[i]);
        renduPartie.cartesSprites[i]->setScale(sf::Vector2f(0.07f, 0.07f));
    }
    std::cout << std::endl;
}

void afficher(RenduPartie& renduPartie){
    // Affichage de la pioche
    for(unsigned int i = 0; i < renduPartie.partie.pioche.taille; ++i){
        Carte* carte = renduPartie.partie.pioche.cartes[i];
        if(carte != nullptr){
            unsigned int boiteIndice = carte->boiteIndice;
            renduPartie.cartesSprites[boiteIndice]->setPosition(sf::Vector2f(i*50, 10));
            renduPartie.window->draw(*renduPartie.cartesSprites[boiteIndice]);
        }
    }
}

std::string recupNomFichier(const Carte& c){
    std::string nomfic = "";
    nomfic += std::to_string(c.chiffre);
    nomfic += '_';
    nomfic += c.couleur;
    nomfic += '_';
    nomfic += std::to_string(c.spirale);
    nomfic += ".jpg";
    return nomfic;
}

sf::Color couleurCarte(Carte c){
    if(c.couleur == 'r')
        return sf::Color::Red;
    else if(c.couleur == 'v')
        return sf::Color::Green;
    else if(c.couleur == 'j')
        return sf::Color::Yellow;
    else if(c.couleur == 'b')
        return sf::Color::Blue;
    else if(c.couleur == 's')
        return sf::Color::Magenta;
}

