#include "Ordinateur.hpp"
#include "Graphics.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

void lancerJeu(){
    // Création de la partie
    Partie partie;

    // Création des cartes dans la mémoire à l'aide d'une boite de cartes
    creerCartesAvecFichier("assets/cartes_pixies.txt", partie.boite);

    // Chargement des cartes et mélange
    initDefausse(partie.defausse);
    remplir(partie.boite, partie.defausse);
    melanger(partie.defausse);
    
    // On demande à l'utilisateur le nombre de joueurs à la partie.
    std::cout << "Le jeu se joue de 2 à 5 joueurs !" << std::endl << "Nombre de joueurs: ";
    std::cin >> partie.nombreJoueurs;
    while(partie.nombreJoueurs > 5 or partie.nombreJoueurs < 2){
        std::cout << "Impossible. Le jeu se joue de 2 à 5 joueurs ! Nombre de joueurs: ";
        std::cin >> partie.nombreJoueurs;
    }
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
    initialiserDispositionEcran(mg);
    chargerTextures(mg);
    mg.font = sf::Font("assets/font.ttf");
    initialiserSelecteur(mg);
    mg.window = new sf::RenderWindow(sf::VideoMode({1280, 720}), "Pixies");
    
    mg.etatActuel = "Attente Choix Pioche";
    mg.espaceSelecteur = "Pioche";
    mg.coupActuel = "";
    while (mg.window->isOpen() and not (mg.partie->estMancheFinie and mg.partie->numeroManche == 3))
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

    supprimerDefausse(partie.defausse);
    supprimerBoite(partie.boite);
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
        mg.spritesCartes[i]->setScale({mg.tailleCartes.x / dimTexture.x, mg.tailleCartes.y / dimTexture.y});
        mg.spritesCartes[i]->setOrigin({dimTexture.x / 2.f, dimTexture.y / 2.f});
    }
    // On charge la texture du fond
    mg.textureFond = sf::Texture("assets/fondBois.png");
}

void dessinerTout(MoteurGraphique& mg){
    // Affichage du fond
    sf::RectangleShape fond({1280, 720});
    fond.setTexture(&mg.textureFond);
    mg.window->draw(fond);
    // Affichage de la pioche
    for(unsigned int i = 0; i < mg.partie->pioche.taille; ++i){
        Carte* carte = mg.partie->pioche.cartes[i];
        if(carte != nullptr){
            dessinerCarte(mg, carte, mg.emplacementsPioche[i]);
        }
    }
    // Affichage des deux grilles
    for(unsigned int i = 0; i < mg.partie->nombreJoueurs; ++i){
        dessinerJoueur(mg, i);
    }
    // -------- Afficher les textes --------
    sf::Text piocheTxt(mg.font, "Pioche", 56);
    // On récupère les limites
    sf::FloatRect bounds = piocheTxt.getLocalBounds();
    // On met l'origine au centre et on positionne au bon endroit
    piocheTxt.setOrigin(bounds.position + bounds.size / 2.f);
    piocheTxt.setPosition(mg.txtPiochePosition);
    if(mg.partie->nombreJoueurs == 2 or mg.partie->nombreJoueurs > 3)
        piocheTxt.setRotation(sf::degrees(90));
    mg.window->draw(piocheTxt);

    // Si on est dans le choix de la carte visible
    if(mg.etatActuel == "Attente Choix Visible"){
        mg.window->draw(mg.fondChoixVisible);
        Carte* cartePioche = mg.partie->pioche.cartes[mg.coupActuel[1]-'0'];
        Carte* carteGrille = mg.partie->joueurs[mg.partie->prochainJoueur].grilleDeJeu[cartePioche->chiffre-1].faceVisible;
        // On dessine les deux choix
        dessinerDosCarte(mg, mg.emplacementsChoixVisible[0]+mg.decalageDos);
        dessinerCarte(mg, cartePioche, mg.emplacementsChoixVisible[0]);
        dessinerDosCarte(mg, mg.emplacementsChoixVisible[1]+mg.decalageDos);
        dessinerCarte(mg, carteGrille, mg.emplacementsChoixVisible[1]);
    }
    // Dessin du selecteur de carte
    float t = mg.clock.getElapsedTime().asSeconds();
    float scale = 1.0 + 0.1f * std::sin(t * 5.0f);

    mg.selecteur.setScale(sf::Vector2f(scale, scale));
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

void dessinerJoueur(MoteurGraphique& mg, unsigned int joueur){
    // On dessine les cartes de la grille du joueur
    const Joueur& j = mg.partie->joueurs[joueur];
    for(unsigned int i = 0; i < 9; ++i){
        Carte* carteVisible = j.grilleDeJeu[i].faceVisible;
        Carte* carteCachee = j.grilleDeJeu[i].faceCachee;
        if(carteVisible == nullptr and carteCachee != nullptr)
            dessinerDosCarte(mg, mg.emplacementsGrille[joueur][i]);
        else{
            if(carteCachee != nullptr)
                dessinerDosCarte(mg, mg.emplacementsGrille[joueur][i]+mg.decalageDos);
            dessinerCarte(mg, carteVisible, mg.emplacementsGrille[joueur][i]);
        }
    }
    // Dessin du nom et des points du joueur 
    sf::Text ptsTxt(mg.font, std::to_string(j.nbPoints)+" pts", 26);
    // On récupère les limites de cahque texte
    int nomTaillePolice = 48;
    sf::Text nomTxt(mg.font, j.surnom,  nomTaillePolice);
    while(nomTxt.getLocalBounds().size.x > 2.1*mg.tailleCartes.x){
        nomTaillePolice--;
        nomTxt = sf::Text(mg.font, j.surnom, nomTaillePolice);
    }

    sf::FloatRect nomRect = nomTxt.getLocalBounds();
    sf::FloatRect ptsRect = ptsTxt.getLocalBounds();
    // On met l'origine des textes au centre
    nomTxt.setOrigin({nomRect.position.x, nomRect.position.y + nomRect.size.y});
    ptsTxt.setOrigin({ptsRect.position.x + ptsRect.size.x, ptsRect.position.y + ptsRect.size.y});
    // On positionne les textes
    nomTxt.setPosition(mg.emplacementsGrille[joueur][0] - mg.tailleCartes/2.f + sf::Vector2f(0, 1.3*mg.decalageDos.y));
    ptsTxt.setPosition(mg.emplacementsGrille[joueur][2] + sf::Vector2f(mg.tailleCartes.x/2.f, -mg.tailleCartes.y/2.f) + sf::Vector2f(0, 1.3*mg.decalageDos.y));
    // On les dessine
    mg.window->draw(nomTxt);
    mg.window->draw(ptsTxt);
}

void initialiserDispositionEcran(MoteurGraphique& mg){
    mg.emplacementsChoixVisible = {sf::Vector2f(500.f, 350.f), sf::Vector2f(700.f, 350.f)};
    if(mg.partie->nombreJoueurs == 2){
        for(unsigned int i = 0; i < 4; ++i){
            mg.emplacementsPioche[i] = sf::Vector2f(1120, 110+i*167);
        }
        sf::Vector2f centre;
        for(unsigned int j = 0; j < 2; ++j){
            if(j == 0)
                centre = {230, 415};
            else
                centre = {730, 415};
            for(unsigned int empl = 0; empl < 9; ++empl){
                sf::Vector2f decalage = sf::Vector2f((-1.0+empl%3)*110.0, (-1.0 + empl/3)*177.0);
                mg.emplacementsGrille[j][empl] = sf::Vector2f(centre + decalage);
            }
        }
        mg.txtPiochePosition = {1220.f, 350};
        mg.tailleCartes = sf::Vector2f(90.f, 126.f);
        mg.decalageDos = sf::Vector2f(-10.f, -10.f);
    }
    else if(mg.partie->nombreJoueurs == 3){
        mg.tailleCartes = sf::Vector2f(88.f, 134.f);
        sf::Vector2f& t = mg.tailleCartes;

        mg.decalageDos = sf::Vector2f(-10.f, -10.f);

        sf::Vector2f ecartCarte(23.f, 30.f);

        // Emplacements des cartes de la pioche
        mg.txtPiochePosition = {130.f, 81.f};
        for(unsigned int i = 0; i < 3; ++i){
            mg.emplacementsPioche[i] = {300+i*(t.x+ecartCarte.x), 81};
        }
        // Emplacements des cartes des grilles
        sf::Vector2f centre;
        for(unsigned int j = 0; j < 3; ++j){
            if(j == 0)
                centre = {210, 450};
            else if(j == 1)
                centre = {640, 450};
            else
                centre = {1070, 450};

            for(int empl = 0; empl < 9; ++empl){
                sf::Vector2f decalage = {(-1+empl%3)*(t.x+ecartCarte.x), (-1 + empl/3)*(t.y+ecartCarte.y)};
                mg.emplacementsGrille[j][empl] = {centre + decalage};
            }
        }
    }else if(mg.partie->nombreJoueurs == 4){
        mg.tailleCartes = {90.f, 126.f};
        sf::Vector2f& t = mg.tailleCartes;

        mg.taillePetitesCartes = 0.8f*mg.tailleCartes;
        sf::Vector2f& pt = mg.taillePetitesCartes;

        mg.decalageDos = sf::Vector2f(-10.f, -10.f);

        sf::Vector2f ecartCarte(23.f, 30.f);
        mg.txtPiochePosition = {1220.f, 350};
        
        mg.taillePetitesCartes = 0.8f*mg.tailleCartes;
        for(unsigned int i = 0; i < 4; ++i){
            mg.emplacementsPioche[i] = sf::Vector2f(1120, 110+i*167);
        }
        sf::Vector2f centre;
        for(unsigned int j = 0; j < 5; ++j){
            if(j == 0)
                centre = {230, 415};
            else
                centre = {730, 415};
            for(unsigned int empl = 0; empl < 9; ++empl){
                sf::Vector2f decalage = sf::Vector2f((-1.0+empl%3)*110.0, (-1.0 + empl/3)*177.0);
                mg.emplacementsGrille[j][empl] = sf::Vector2f(centre + decalage);
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
    mg.selecteur.setOutlineColor(sf::Color::Red);
    mg.selecteur.setOutlineThickness(2);
    mg.indiceSelecteur = 0;
    mg.selecteur.setPosition(mg.emplacementsPioche[0]);

    mg.fondChoixVisible.setSize(sf::Vector2f(300.f, 180.f));
    mg.fondChoixVisible.setOrigin(sf::Vector2f(150.f, 90.f));
    mg.fondChoixVisible.setFillColor(sf::Color(200, 200, 200, 200));
    mg.fondChoixVisible.setOutlineColor(sf::Color::White);
    mg.fondChoixVisible.setOutlineThickness(2);
    mg.fondChoixVisible.setPosition(sf::Vector2f(600.f, 350.f));
}

void decalerSelecteur(MoteurGraphique& mg, int cote){
    if(mg.espaceSelecteur == "Pioche")
    {
        std::cout << "Selecteur decale vers la droite" << std::endl;
        mg.indiceSelecteur = (mg.indiceSelecteur+cote)%mg.partie->pioche.taille;
        while(mg.partie->pioche.cartes[mg.indiceSelecteur] == nullptr)
            mg.indiceSelecteur = (mg.indiceSelecteur+cote)%mg.partie->pioche.taille;
        mg.selecteur.setPosition(mg.emplacementsPioche[mg.indiceSelecteur]);
    }
    else if(mg.espaceSelecteur == "Grille"){
        mg.indiceSelecteur = (mg.indiceSelecteur+cote)%9;
        while(mg.partie->joueurs[mg.partie->prochainJoueur].grilleDeJeu[mg.indiceSelecteur].faceVisible != nullptr or mg.partie->joueurs[mg.partie->prochainJoueur].grilleDeJeu[mg.indiceSelecteur].faceCachee != nullptr)
            mg.indiceSelecteur = (mg.indiceSelecteur+cote)%9;
        mg.selecteur.setPosition(mg.emplacementsGrille[mg.partie->prochainJoueur][mg.indiceSelecteur]);
    }
    else if(mg.espaceSelecteur == "Choix Visible"){
        mg.indiceSelecteur = (mg.indiceSelecteur+1)%2;
        mg.selecteur.setPosition(mg.emplacementsChoixVisible[mg.indiceSelecteur]);
    }
}

void gererUnChoix(MoteurGraphique& mg){
    // Si on attendait un choix parmi les cartes de la pioche
    if(mg.etatActuel == "Attente Choix Pioche"){
        // On connait donc les deux premiers caractères du coup, le joueur et l'indice de la carte dans la pioche
        mg.coupActuel += std::to_string(mg.partie->prochainJoueur) + std::to_string(mg.indiceSelecteur);
        std::cout << "Coup qui sera joué " << mg.coupActuel <<"..." << std::endl;
        // On va maintenant changer le type d'attente selon l'emplacement de la carte chois dans la grille
        const Grille& grille = mg.partie->joueurs[mg.partie->prochainJoueur].grilleDeJeu;
        const Carte* carteChoisi = mg.partie->pioche.cartes[mg.indiceSelecteur];

        // Si il n'y a pas de carte face visible
        if(grille[carteChoisi->chiffre-1].faceVisible == nullptr){
            // Alors on connait le coup et on peut le jouer, le joueur n'a plus rine à choisir.
            // Le coup est direct et la carte va à l'emplacement de son chiffre
            mg.coupActuel += "d"+ std::to_string(carteChoisi->chiffre-1);
            std::cout << "Coup qui sera joué " << mg.coupActuel << std::endl;
            jouerCoup(*mg.partie, mg.coupActuel);
            std::cout << "Coup joué" << std::endl;
            gererFinDeCoup(mg);
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
            changerEspaceSelecteur(mg, "Choix Visible");
            // On se met dans le mode ou on attend le choix de laquelle on laisse visible
            mg.etatActuel = "Attente Choix Visible";
        }
    }
    // Si on attendait un chois parmi les emplacements de la grille
    else if(mg.etatActuel == "Attente Choix Grille"){
        // On connait maintenant l'emplacement où va aller la carte
        // On peut donc écrire le coup et le jouer
        mg.coupActuel += std::to_string(mg.indiceSelecteur-mg.partie->prochainJoueur*9);
        std::cout << "Coup qui sera joué " << mg.coupActuel << std::endl;
        jouerCoup(*mg.partie, mg.coupActuel);
        std::cout << "Coup joué."<< std::endl;
        gererFinDeCoup(mg);
    }
    // Si on attendait le choix de la carte qui sera visible
    else if(mg.etatActuel == "Attente Choix Visible"){
        // Si le choix est l'emplacement 0, qui est celui de la carte de la pioche
        if(mg.indiceSelecteur == 0)
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
        gererFinDeCoup(mg);
    }
}

void changerEspaceSelecteur(MoteurGraphique &mg, std::string espace){
    mg.espaceSelecteur = espace;
    std::cout << "Nouveau espace pour le sélecteur " << espace << std::endl;
    if(mg.espaceSelecteur == "Pioche")
    {
        // On met le selecteur au premier emplacement de la pioche
        mg.indiceSelecteur = 0;
        // Tant qu'il n'y a pas de carte à l'endroit du sélecteur
        while(mg.partie->pioche.cartes[mg.indiceSelecteur] == nullptr)
            // On décale vers la droite
            mg.indiceSelecteur = (mg.indiceSelecteur+1)%mg.partie->pioche.taille;
        // On met à jour sa position
        mg.selecteur.setPosition(mg.emplacementsPioche[mg.indiceSelecteur]);
    }
    else if(mg.espaceSelecteur == "Grille"){
        // On met le selecteur au premier emplacement de la grille du prochain joueur
        mg.indiceSelecteur = 0;
        // Tant que l'emplacement où se trouve le sélecteur contient une carte
        const Grille& grille = mg.partie->joueurs[mg.partie->prochainJoueur].grilleDeJeu;
        while(grille[mg.indiceSelecteur].faceVisible != nullptr or grille[mg.indiceSelecteur].faceCachee != nullptr)
            // On va à l'emplacement suivant
            mg.indiceSelecteur =  (mg.indiceSelecteur+1)%9;
        // On met à jour sa position
        mg.selecteur.setPosition(mg.emplacementsGrille[mg.partie->prochainJoueur][mg.indiceSelecteur]);
    }
    else if(mg.espaceSelecteur == "Choix Visible"){
        // On met le sélecteur à l'emplacement d'indice zero, il y a forcément une carte
        mg.indiceSelecteur = 0;
        mg.selecteur.setPosition(mg.emplacementsChoixVisible[mg.indiceSelecteur]);
    }
}

void gererFinDeCoup(MoteurGraphique& mg){
    // Si le choix d'avant à fait finir la manche
    if(mg.partie->estMancheFinie){
        // On met à jour tout pour commencer la prochaine manche
        toutRemettreDansDefausse(*mg.partie);
        if(mg.partie->numeroManche == 3){
            supprimerDefausse(mg.partie->defausse);
            supprimerBoite(mg.partie->boite);
        }
        else{
            melanger(mg.partie->defausse);
            ++mg.partie->numeroManche;
            mg.partie->estMancheFinie = false;
            mg.partie->coupsManche.nombre = 0;
            remplirPioche(mg.partie->pioche, mg.partie->defausse);
            // On remet les valeurs qui permetteront d'attendre le choix dans la pioche du prochain joueur
            mg.etatActuel = "Attente Choix Pioche";
            changerEspaceSelecteur(mg, "Pioche");
            // On remet le prochain coup vide
            mg.coupActuel = "";
        }
    }else{
        // On remet les valeurs qui permetteront d'attendre le choix dans la pioche du prochain joueur
        mg.etatActuel = "Attente Choix Pioche";
        changerEspaceSelecteur(mg, "Pioche");
        // On remet le prochain coup vide
        mg.coupActuel = "";
    }
    
}