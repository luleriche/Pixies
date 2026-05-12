#include "Ordinateur.hpp"
#include "Graphics.hpp"
#include <SFML/Graphics.hpp>

void lancerJeu(){
    // Création de la partie
    Partie partie;

    // Création des cartes dans la mémoire à l'aide d'une boite de cartes
    creerCartesAvecFichier("assets/cartes_pixies.txt", partie.boite);

    // Chargement des cartes et mélange
    initDefausse(partie.defausse);
    remplir(partie.boite, partie.defausse);
    melanger(partie.defausse);
    
    partie.nombreJoueurs = 2;
    creerJoueurs(partie.joueurs, partie.nombreJoueurs, &partie.pioche);

    // On initialise la pioche
    if(partie.nombreJoueurs == 2)
        initPioche(partie.pioche, 4);
    else
        initPioche(partie.pioche, partie.nombreJoueurs);
    // On fait jouer quelque fois des bots
    partie.estMancheFinie = false;
    partie.coupsManche.nombre = 0;

    remplirPioche(partie.pioche, partie.defausse);
    std::cout << "DEBUT DE LA MANCHE " << partie.numeroManche << std::endl;
    
    for(int i = 0; i < 20; ++i)
        jouerCoup(partie, recupCoupsPossibles(partie).coups[0]);

    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1200, 700}), "Pixies");
    
    RenduPartie renduPartie;

    renduPartie.partie = &partie;
    chargerCartesTextures(renduPartie);
    renduPartie.fondTexture = sf::Texture("assets/fondBois.png");
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
    renduPartie.nbSprites = renduPartie.partie->boite.nbCartes;
    std::cout << "Chargement des textures." << std::endl;
    for(unsigned int i = 0; i < renduPartie.nbSprites; ++i){
        // On charge la texture
        std::string nomFichier = recupNomFichier(renduPartie.partie->boite.cartes[i]);
        const sf::Texture texture("assets/images_pixies/"+nomFichier);
        std::cout << nomFichier << " chargé." << std::endl;
        // On la stock dans les textures du rendu
        renduPartie.cartesTextures[i] = texture;
        // On la met dans le sprite
        renduPartie.cartesSprites[i] = sf::Sprite(renduPartie.cartesTextures[i]);
        // On redimensionne le sprite
        // Si tu veux une taille précise (ex: 86x120) :
        sf::Vector2u dimTexture = texture.getSize();
        renduPartie.cartesSprites[i]->setScale({90.f / dimTexture.x, 126.f / dimTexture.y});
        renduPartie.cartesSprites[i]->setOrigin({dimTexture.x / 2.f, dimTexture.y / 2.f});
    }
    std::cout << std::endl;
}

void afficher(RenduPartie& renduPartie){
    // Affichage du fond
    sf::RectangleShape fond({1200, 700});
    fond.setTexture(&renduPartie.fondTexture);
    renduPartie.window->draw(fond);
    // Affichage de la pioche
    // 10 pixeles entre chaque carte et laisser un espace avant pour la défausse
    for(unsigned int i = 0; i < renduPartie.partie->pioche.taille; ++i){
        Carte* carte = renduPartie.partie->pioche.cartes[i];
        if(carte != nullptr){
            afficherRenduCarte(renduPartie, carte, sf::Vector2f(1020, 110+i*167));
        }
    }
    // Affichage des deux grilles
    afficherRenduGrille(renduPartie, 0, sf::Vector2f(230, 415));
    afficherRenduGrille(renduPartie, 1, sf::Vector2f(685, 415));
    // -------- Afficher les textes --------
    sf::Font font("assets/font.ttf");
    sf::Text piocheTxt(font, "Pioche", 56);
    // On récupère les limites
    sf::FloatRect bounds = piocheTxt.getLocalBounds();
    // On met l'origine au centre
    piocheTxt.setOrigin(bounds.position + bounds.size / 2.f);
    piocheTxt.setPosition({1120.f, 350});
    piocheTxt.setRotation(sf::degrees(90));
    renduPartie.window->draw(piocheTxt);    
}


void afficherRenduCarte(RenduPartie& rendu, Carte* c, sf::Vector2f centre){
    if(c != nullptr){
        unsigned int boiteIndice = c->boiteIndice;
        rendu.cartesSprites[boiteIndice]->setPosition(centre);
        rendu.window->draw(*rendu.cartesSprites[boiteIndice]);
    }
}

void afficherRenduGrille(RenduPartie& renduPartie, unsigned int joueur, sf::Vector2f centre){
    for(unsigned int i = 0; i < 9; ++i){
        sf::Vector2f decalage = sf::Vector2f((-1.0+i%3)*110.0, (-1.0 + i/3)*177.0);
        Carte* cVisible = renduPartie.partie->joueurs[joueur].grilleDeJeu[i].faceVisible;
        Carte* cCachee = renduPartie.partie->joueurs[joueur].grilleDeJeu[i].faceCachee;
        afficherRenduCarte(renduPartie, cCachee, centre+decalage+sf::Vector2f(-10, -10));
        afficherRenduCarte(renduPartie, cVisible, centre+decalage);
    }
    sf::Font font("assets/font.ttf");
    // Dessin du nom et des pts 
    sf::Text nomTxt(font, renduPartie.partie->joueurs[joueur].surnom, 48);
    sf::Text ptsTxt(font, std::to_string(renduPartie.partie->joueurs[joueur].nbPoints)+" pts", 26);
    // On récupère les limites
    sf::FloatRect nomRect = nomTxt.getLocalBounds();
    sf::FloatRect ptsRect = ptsTxt.getLocalBounds();
    // On met l'origine au centre
    nomTxt.setOrigin(nomRect.position + nomRect.size / 2.f);
    ptsTxt.setOrigin(ptsRect.position + ptsRect.size / 2.f);
    // On positionne les textes
    nomTxt.setPosition(centre - sf::Vector2f(0, 305));
    ptsTxt.setPosition(centre - sf::Vector2f(0, 270));
    // On les dessine
    renduPartie.window->draw(nomTxt);
    renduPartie.window->draw(ptsTxt);
}

std::string recupNomFichier(const Carte& c){
    std::string nomfic = "";
    nomfic += std::to_string(c.chiffre);
    nomfic += '_';
    nomfic += c.couleur;
    nomfic += '_';
    nomfic += std::to_string(c.spirale);
    nomfic += ".png";
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
    else
        return sf::Color::White;
}

