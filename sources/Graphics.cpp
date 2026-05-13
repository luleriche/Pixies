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
    // Génération aléatoire du premier joueur.
    partie.prochainJoueur = rand()%partie.nombreJoueurs;

    // On initialise la pioche
    if(partie.nombreJoueurs == 2)
        initPioche(partie.pioche, 4);
    else
        initPioche(partie.pioche, partie.nombreJoueurs);
    // On fait jouer quelque fois des bots
    partie.estMancheFinie = false;
    partie.coupsManche.nombre = 0;
    partie.numeroManche = 1;
    remplirPioche(partie.pioche, partie.defausse);

    MoteurGraphique mg;
    mg.partie = &partie;
    chargerTextures(mg);
    mg.font = sf::Font("assets/font.ttf");
    initialiserEmplacementsSelecteur(mg);
    initialiserSelecteur(mg);
    mg.window = new sf::RenderWindow(sf::VideoMode({1200, 700}), "Pixies");

    
    mg.etatActuel = "Attente Choix Pioche";
    mg.espaceSelecteur = "Pioche";
    mg.coupActuel = "";
    mg.choix = -1;
    while (not mg.partie->estMancheFinie)
    {   

        while (const std::optional event = mg.window->pollEvent())
        {
            if (event->is<sf::Event::Closed>()){
                mg.window->close();
            }
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Right) {
                    decalerSelecteur(mg, 1);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Left) {
                    decalerSelecteur(mg, -1);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Enter) {
                    gererUnChoix(mg);
                }
            }
        }
        mg.window->clear(sf::Color::Black);
        dessinerTout(mg);
        mg.window->display();
    }
}

void chargerTextures(MoteurGraphique& mg){
    // On note le nombre de sprites, c'est le nombre de cartes + 1 pour le dos d'une carte
    mg.nbSpritesCartes = mg.partie->boite.nbCartes + 1;
    std::cout << "Chargement des textures." << std::endl;
    // Creation des sprites des cartes
    for(unsigned int i = 0; i < mg.nbSpritesCartes; ++i){
        // récupère le nom du fichier à charger
        std::string nomFichier;
        if(i != mg.nbSpritesCartes -1)
            nomFichier = recupNomFichier(mg.partie->boite.cartes[i]);
        else
            nomFichier = "dos.png";
        // On charge la texture
        mg.texturesCartes[i] = sf::Texture("assets/images_pixies/"+nomFichier);
        std::cout << nomFichier << " chargé." << std::endl;
        // On change la texture du sprite qui correspond à ça
        mg.spritesCartes[i] = sf::Sprite(mg.texturesCartes[i]);
        // On redimensionne le sprite pour qu'il fasse 90x126 et on met son origine au centre
        sf::Vector2u dimTexture = mg.texturesCartes[i].getSize();
        mg.spritesCartes[i]->setScale({90.f / dimTexture.x, 126.f / dimTexture.y});
        mg.spritesCartes[i]->setOrigin({dimTexture.x / 2.f, dimTexture.y / 2.f});
    }
    // On charge la texture du fond
    mg.textureFond = sf::Texture("assets/fondBois.png");
}

void dessinerTout(MoteurGraphique& mg){
    // Affichage du fond
    sf::RectangleShape fond({1200, 700});
    fond.setTexture(&mg.textureFond);
    mg.window->draw(fond);
    // Affichage de la pioche
    // 10 pixeles entre chaque carte et laisser un espace avant pour la défausse
    for(unsigned int i = 0; i < mg.partie->pioche.taille; ++i){
        Carte* carte = mg.partie->pioche.cartes[i];
        if(carte != nullptr){
            dessinerCarte(mg, carte, sf::Vector2f(1020, 110+i*167));
        }
    }
    // Affichage des deux grilles
    dessinerJoueur(mg, 0, sf::Vector2f(230, 415));
    dessinerJoueur(mg, 1, sf::Vector2f(685, 415));
    // -------- Afficher les textes --------
    sf::Text piocheTxt(mg.font, "Pioche", 56);
    // On récupère les limites
    sf::FloatRect bounds = piocheTxt.getLocalBounds();
    // On met l'origine au centre et on positionne au bon endroit
    piocheTxt.setOrigin(bounds.position + bounds.size / 2.f);
    piocheTxt.setPosition({1120.f, 350});
    piocheTxt.setRotation(sf::degrees(90));
    mg.window->draw(piocheTxt);    
    // Dessin du selecteur de carte
    mg.window->draw(mg.selecteur); 
}


void dessinerCarte(MoteurGraphique& mg, Carte* c, sf::Vector2f centre){
    if(c != nullptr){
        // On récupère la position dans la boite de jeu de la carte
        unsigned int boiteIndice = c->boiteIndice;
        // On récupère le sprite qui est à cet indice dans notre liste de sprite
        mg.spritesCartes[boiteIndice]->setPosition(centre);
        mg.window->draw(*mg.spritesCartes[boiteIndice]);
    }
}

void dessinerDosCarte(MoteurGraphique& mg, sf::Vector2f centre){
    // On récupère le sprite qui est à cet indice dans notre liste de sprite
    mg.spritesCartes[mg.nbSpritesCartes - 1]->setPosition(centre);
    mg.window->draw(*mg.spritesCartes[mg.nbSpritesCartes - 1]);
}

void dessinerJoueur(MoteurGraphique& mg, unsigned int joueur, sf::Vector2f centre){
    // On dessine les cartes de la grille du joueur
    const Joueur& j = mg.partie->joueurs[joueur];
    for(unsigned int i = 0; i < 9; ++i){
        sf::Vector2f decalage = sf::Vector2f((-1.0+i%3)*110.0, (-1.0 + i/3)*177.0);
        Carte* cVisible = j.grilleDeJeu[i].faceVisible;
        Carte* cCachee = j.grilleDeJeu[i].faceCachee;
        dessinerCarte(mg, cCachee, centre+decalage+sf::Vector2f(-10, -10));
        dessinerCarte(mg, cVisible, centre+decalage);
    }
    // Dessin du nom et des points du joueur 
    sf::Text nomTxt(mg.font, j.surnom, 48);
    sf::Text ptsTxt(mg.font, std::to_string(j.nbPoints)+" pts", 26);
    // On récupère les limites de cahque texte
    sf::FloatRect nomRect = nomTxt.getLocalBounds();
    sf::FloatRect ptsRect = ptsTxt.getLocalBounds();
    // On met l'origine des textes au centre
    nomTxt.setOrigin(nomRect.position + nomRect.size / 2.f);
    ptsTxt.setOrigin(ptsRect.position + ptsRect.size / 2.f);
    // On positionne les textes
    nomTxt.setPosition(centre - sf::Vector2f(0, 305));
    ptsTxt.setPosition(centre - sf::Vector2f(0, 270));
    // On les dessine
    mg.window->draw(nomTxt);
    mg.window->draw(ptsTxt);
}

void initialiserEmplacementsSelecteur(MoteurGraphique& mg){
    if(mg.partie->nombreJoueurs == 2){
        for(unsigned int i = 0; i < 4; ++i){
            mg.emplacementsPioche[i] = sf::Vector2f(1020, 110+i*167);
        }
        sf::Vector2f centre;
        for(unsigned int j = 0; j < 2; ++j){
            if(j == 0)
                centre = {230, 415};
            else
                centre = {685, 415};
            for(unsigned int empl = 0; empl < 9; ++empl){
                sf::Vector2f decalage = sf::Vector2f((-1.0+empl%3)*110.0, (-1.0 + empl/3)*177.0);
                mg.emplacementsGrille[j*9+empl] = sf::Vector2f(centre + decalage);
            }
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

void initialiserSelecteur(MoteurGraphique& mg){
    mg.selecteur.setSize(sf::Vector2f(90.f, 126.f));
    mg.selecteur.setOrigin(sf::Vector2f(45.f, 63.f));
    mg.selecteur.setFillColor(sf::Color::Transparent);
    mg.selecteur.setOutlineColor(sf::Color::Green);
    mg.selecteur.setOutlineThickness(2);
    mg.emplacementSelecteur = 0;
    mg.selecteur.setPosition(mg.emplacementsGrille[0]);
}

void decalerSelecteur(MoteurGraphique& mg, int cote){
    if(mg.espaceSelecteur == "Pioche")
    {
        std::cout << "Selecteur decale vers la droite" << std::endl;
        mg.emplacementSelecteur = (mg.emplacementSelecteur+cote)%mg.partie->pioche.taille;
        while(mg.partie->pioche.cartes[mg.emplacementSelecteur] == nullptr)
            mg.emplacementSelecteur = (mg.emplacementSelecteur+cote)%mg.partie->pioche.taille;
        mg.selecteur.setPosition(mg.emplacementsPioche[mg.emplacementSelecteur]);
    }
    else if(mg.espaceSelecteur == "Grille"){
        mg.emplacementSelecteur = mg.partie->prochainJoueur*9 + (mg.emplacementSelecteur-mg.partie->prochainJoueur*9+cote)%9;
        while(mg.partie->joueurs[mg.partie->prochainJoueur].grilleDeJeu[mg.emplacementSelecteur].faceVisible != nullptr or mg.partie->joueurs[mg.partie->prochainJoueur].grilleDeJeu[mg.emplacementSelecteur].faceCachee != nullptr)
            mg.emplacementSelecteur = mg.partie->prochainJoueur*9 + (mg.emplacementSelecteur-mg.partie->prochainJoueur*9+cote)%9;
        mg.selecteur.setPosition(mg.emplacementsGrille[mg.emplacementSelecteur]);
    }
    else if(mg.espaceSelecteur == "Choix Visible"){
        mg.emplacementSelecteur = (mg.emplacementSelecteur+1)%2;
        mg.selecteur.setPosition(mg.emplacementsChoixVisible[mg.emplacementSelecteur]);
    }
}

void gererUnChoix(MoteurGraphique& mg){
    // Si on attendait un choix parmi les cartes de la pioche
    if(mg.etatActuel == "Attente Choix Pioche"){
        // On connait donc les deux premiers caractères du coup, le joueur et l'indice de la carte dans la pioche
        mg.coupActuel += std::to_string(mg.partie->prochainJoueur) + std::to_string(mg.emplacementSelecteur);
        std::cout << "Coup qui sera joué " << mg.coupActuel <<"..." << std::endl;
        // On va maintenant changer le type d'attente selon l'emplacement de la carte chois dans la grille
        const Grille& grille = mg.partie->joueurs[mg.partie->prochainJoueur].grilleDeJeu;
        const Carte* carteChoisi = mg.partie->pioche.cartes[mg.emplacementSelecteur];

        // Si il n'y a pas de carte face visible
        if(grille[carteChoisi->chiffre-1].faceVisible == nullptr){
            // Alors on connait le coup et on peut le jouer, le joueur n'a plus rine à choisir.
            // Le coup est direct et la carte va à l'emplacement de son chiffre
            mg.coupActuel += "d"+ std::to_string(carteChoisi->chiffre-1);
            std::cout << "Coup qui sera joué " << mg.coupActuel << std::endl;
            jouerCoup(*mg.partie, mg.coupActuel);
            std::cout << "Coup joué" << std::endl;
            // On remet les valeurs qui permetteront d'attendre le choix dans la pioche du prochain joueur
            mg.etatActuel = "Attente Choix Pioche";
            // Pas besoin de changer l'espace du selecteur, il est déja sur la pioche
            // Si la pioche vient de se remplir, cad il y a une carte à l'endroit ou on vient d'en prendre une
            if(mg.partie->pioche.cartes[mg.emplacementSelecteur] != nullptr){
                // On se met sur la première carte de la pioche, pour rendre fluide.
                mg.emplacementSelecteur = 0;
                mg.selecteur.setPosition(mg.emplacementsPioche[0]);
            }
            // Sinon il faut changer d'emplacement car l'actuelle est vide
            else
                decalerSelecteur(mg, 1);
            // On remet le prochain coup vide
            mg.coupActuel = "";
        }
        // Si il y a une carte visible et aussi une cachée
        else if(grille[carteChoisi->chiffre-1].faceCachee != nullptr){
            // On sait que le coup sera de la mettre autre part
            mg.coupActuel += "m";
            // On va attendre désormais le choix d'une case vide dans la grille
            mg.etatActuel = "Attente Choix Grille";
            changerEspaceSelecteur(mg, "Grille");
        }
        // Sinon, il y a une carte visible et pas de cachée
        else{
            // On va attendre le choix de la carte qui reste visible
            // Les deux emplacements du selecteur sont donc la carte choisi dans la pioche
            // Et la carte visible à son emplacement
            mg.emplacementsChoixVisible = {mg.selecteur.getPosition(), mg.emplacementsGrille[mg.partie->prochainJoueur*9+carteChoisi->chiffre-1]};
            changerEspaceSelecteur(mg, "Choix Visible");
            // On se met dans le mode ou on attend le choix de laquelle on laisse visible
            mg.etatActuel = "Attente Choix Visible";
        }
    }
    // Si on attendait un chois parmi les emplacements de la grille
    else if(mg.etatActuel == "Attente Choix Grille"){
        // On connait maintenant l'emplacement où va aller la carte
        // On peut donc écrire le coup et le jouer
        mg.coupActuel += std::to_string(mg.emplacementSelecteur-mg.partie->prochainJoueur*9);
        std::cout << "Coup qui sera joué " << mg.coupActuel << std::endl;
        jouerCoup(*mg.partie, mg.coupActuel);
        std::cout << "Coup joué."<< std::endl;
        // On remet les valeurs qui permetteront d'attendre le choix dans la pioche du prochain joueur
        mg.etatActuel = "Attente Choix Pioche";
        changerEspaceSelecteur(mg, "Pioche");
        // On remet le prochain coup vide
        mg.coupActuel = "";
    }
    // Si on attendait le choix de la carte qui sera visible
    else if(mg.etatActuel == "Attente Choix Visible"){
        // Si le choix est l'emplacement 0, qui est celui de la carte de la pioche
        if(mg.emplacementSelecteur == 0)
            // Le coup est donc de mettre la carte pioché visible. On peut récupérer son indice dans la pioche
            // grâce au deuxième caractère du coup actuel donc aussi l'emplacement dans la grille
            mg.coupActuel += "v" + std::to_string(mg.partie->pioche.cartes[mg.coupActuel[1]-'0']->chiffre-1);
        else
            // Sinon cela veut dire qu'on la met face caché, de même pour l'indice dans la pioche et l'emplacement
            // dans la grille
            mg.coupActuel += "c" + std::to_string(mg.partie->pioche.cartes[mg.coupActuel[1]-'0']->chiffre-1);
        // On joue le coup
        std::cout << "Coup qui sera joué " << mg.coupActuel << std::endl;
        jouerCoup(*mg.partie, mg.coupActuel);
        std::cout << "Coup joué." << std::endl;
        // On remet les valeurs qui permetteront d'attendre le choix dans la pioche du prochain joueur
        mg.etatActuel = "Attente Choix Pioche";
        changerEspaceSelecteur(mg, "Pioche");
        // On remet le prochain coup vide
        mg.coupActuel = "";
    }
}

void changerEspaceSelecteur(MoteurGraphique &mg, std::string espace){
    mg.espaceSelecteur = espace;
    std::cout << "Nouveau espace pour le sélecteur " << espace << std::endl;
    if(mg.espaceSelecteur == "Pioche")
    {
        // On met le selecteur au premier emplacement de la pioche
        mg.emplacementSelecteur = 0;
        // Tant qu'il n'y a pas de carte à l'endroit du sélecteur
        while(mg.partie->pioche.cartes[mg.emplacementSelecteur] == nullptr)
            // On décale vers la droite
            mg.emplacementSelecteur = (mg.emplacementSelecteur+1)%mg.partie->pioche.taille;
        // On met à jour sa position
        mg.selecteur.setPosition(mg.emplacementsPioche[mg.emplacementSelecteur]);
    }
    else if(mg.espaceSelecteur == "Grille"){
        // On met le selecteur au premier emplacement de la grille du prochain joueur
        mg.emplacementSelecteur = mg.partie->prochainJoueur*9;
        // Tant que l'emplacement où se trouve le sélecteur contient une carte
        const Grille& grille = mg.partie->joueurs[mg.partie->prochainJoueur].grilleDeJeu;
        while(grille[mg.emplacementSelecteur-mg.partie->prochainJoueur*9].faceVisible != nullptr or grille[mg.emplacementSelecteur-mg.partie->prochainJoueur*9].faceCachee != nullptr)
            // On va à l'emplacement suivant
            mg.emplacementSelecteur = mg.partie->prochainJoueur*9 + (mg.emplacementSelecteur-mg.partie->prochainJoueur*9+1)%9;
        // On met à jour sa position
        mg.selecteur.setPosition(mg.emplacementsGrille[mg.emplacementSelecteur]);
    }
    else if(mg.espaceSelecteur == "Choix Visible"){
        // On met le sélecteur à l'emplacement d'indice zero, il y a forcément une carte
        mg.emplacementSelecteur = 0;
        mg.selecteur.setPosition(mg.emplacementsChoixVisible[mg.emplacementSelecteur]);
    }
}

