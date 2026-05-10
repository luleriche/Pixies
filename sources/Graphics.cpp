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
    
    for(int i = 0; i < 10; ++i)
        faireJouerProchain(partie);


    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1000, 700}), "Pixies");
    RenduPartie renduPartie;
    renduPartie.partie = &partie;
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
        renduPartie.cartesSprites[i]->setScale({80.f / dimTexture.x, 120.f / dimTexture.y});
        renduPartie.cartesSprites[i]->setOrigin({40, 60});
    }
    std::cout << std::endl;
}

void afficher(RenduPartie& renduPartie){
    // Affichage de la pioche
    // 10 pixeles entre chaque carte et laisser un espace avant pour la défausse
    for(unsigned int i = 0; i < renduPartie.partie->pioche.taille; ++i){
        Carte* carte = renduPartie.partie->pioche.cartes[i];
        if(carte != nullptr){
            afficherRenduCarte(renduPartie, carte, sf::Vector2f(140+i*100, 70));
        }
    }
    // Affichage des deux grilles
    afficherRenduGrille(renduPartie, 0, sf::Vector2f(200, 380));
    afficherRenduGrille(renduPartie, 1, sf::Vector2f(800, 380));
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
        sf::Vector2f decalage = sf::Vector2f((-1.0+i%3)*100, (-1.0 + i/3)*140.0);
        Carte* cVisible = renduPartie.partie->joueurs[joueur].grilleDeJeu[i].faceVisible;
        Carte* cCachee = renduPartie.partie->joueurs[joueur].grilleDeJeu[i].faceCachee;
        afficherRenduCarte(renduPartie, cCachee, centre+decalage+sf::Vector2f(-7, -7));
        afficherRenduCarte(renduPartie, cVisible, centre+decalage);
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
    else
        return sf::Color::White;
}

