#include "Ordinateur.hpp"
#include "Graphics.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

void lancerJeu(){
    // INITIALISATION DE LA PARTIE
    MoteurGraphique mg;

    creerCartesAvecFichier("assets/cartes_pixies.txt", mg.partie.boite);

    initDefausse(mg.partie.defausse);
    remplir(mg.partie.boite, mg.partie.defausse);
    melanger(mg.partie.defausse);

    std::cout << "Le jeu se joue de 2 à 5 joueurs !" << std::endl << "Nombre de joueurs: ";
    std::cin >> mg.partie.nombreJoueurs;
    while(mg.partie.nombreJoueurs > 5 or mg.partie.nombreJoueurs < 2){
        std::cout << "Impossible. Le jeu se joue de 2 à 5 joueurs ! Nombre de joueurs: ";
        std::cin >> mg.partie.nombreJoueurs;
    }
    creerJoueurs(mg.partie.joueurs, mg.partie.nombreJoueurs, &mg.partie.pioche);

    mg.partie.prochainJoueur = rand()%mg.partie.nombreJoueurs;

    if(mg.partie.nombreJoueurs == 2)
        initPioche(mg.partie.pioche, 4);
    else
        initPioche(mg.partie.pioche, mg.partie.nombreJoueurs);
    
    mg.partie.estMancheFinie = false;
    mg.partie.coupsManche.nombre = 0;
    mg.partie.numeroManche = 1;
    remplirPioche(mg.partie.pioche, mg.partie.defausse);

    // INITIALISATION DU MOTEUR GRAPHIQUE
    initialiserDispositionEcran(mg);
    initialiserTexturesEtSprites(mg);
    mg.font = sf::Font("assets/font.ttf");

    mg.espaceSelecteur = "Pioche";
    mg.coupActuel = "";
    
    mg.window = new sf::RenderWindow(sf::VideoMode({1280, 720}), "Pixies");
    
    while (mg.window->isOpen() and not (mg.partie.estMancheFinie and mg.partie.numeroManche == 3))
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
    supprimerDefausse(mg.partie.defausse);
    supprimerBoite(mg.partie.boite);
}

void initialiserTexturesEtSprites(MoteurGraphique& mg){
    
    // Creation des sprites des cartes
    mg.nbSpritesCartes = mg.partie.boite.nbCartes + 1; // On note le nombre de sprites, c'est le nombre de cartes + 1 pour le dos d'une carte
    std::cout << "Chargement des textures et création des sprites." << std::endl;
    for(unsigned int i = 0; i < mg.nbSpritesCartes; ++i){
        // On récupère le nom du fichier à charger
        std::string nomFichier;
        if(i != mg.nbSpritesCartes -1)
            nomFichier = recupNomFichier(mg.partie.boite.cartes[i]);
        else
            nomFichier = "dos.png";
        // On charge la texture
        mg.texturesCartes[i] = sf::Texture("assets/images_pixies/"+nomFichier);
        std::cout << nomFichier << " chargé." << std::endl;
        // On crée le sprite associé à cette texture
        mg.spritesCartes[i] = sf::Sprite(mg.texturesCartes[i]);
        // On redimensionne le sprite pour qu'il fasse 90x126 et on met son origine au centre
        sf::Vector2u dimTexture = mg.texturesCartes[i].getSize();
        mg.scaleFacteursCartes[i] = {mg.tailleCartes.x / dimTexture.x, mg.tailleCartes.y / dimTexture.y};
        mg.spritesCartes[i]->setScale(mg.scaleFacteursCartes[i]);
        mg.spritesCartes[i]->setOrigin({dimTexture.x / 2.f, dimTexture.y / 2.f});
    }

    // On charge la texture du fond
    mg.textureFond = sf::Texture("assets/fondBois.png");

    // On initialise le rectangle en fond lors du choix de la carte visible
    mg.fondChoixVisible.setSize(sf::Vector2f(300.f, 180.f));
    mg.fondChoixVisible.setOrigin(sf::Vector2f(150.f, 90.f));
    mg.fondChoixVisible.setFillColor(sf::Color(200, 200, 200, 200));
    mg.fondChoixVisible.setOutlineColor(sf::Color::White);
    mg.fondChoixVisible.setOutlineThickness(2);
    mg.fondChoixVisible.setPosition(sf::Vector2f(600.f, 350.f));

    // On charge la texture du sélecteur et crée son sprite
    mg.textureSelecteur = sf::Texture("assets/icone-de-feuille-verte.png");
    mg.selecteur = sf::Sprite(mg.textureSelecteur);
    sf::Vector2u dimTexture = mg.textureSelecteur.getSize();
    mg.selecteur->setScale({80.f / dimTexture.x, 80.f / dimTexture.y});
    mg.selecteur->setOrigin({dimTexture.x / 2.f, dimTexture.y / 2.f});
}

void dessinerTout(MoteurGraphique& mg){
    // Temps depuis le début pour les animations
    float t = mg.clock.getElapsedTime().asSeconds();
    float scale = 1.0 + 0.06f * std::sin(t * 2.5);

    // Affichage du fond
    sf::RectangleShape fond({1280, 720});
    fond.setTexture(&mg.textureFond);
    mg.window->draw(fond);


    // Affichage des cartes de la pioche et de son texte
    for(unsigned int i = 0; i < mg.partie.pioche.taille; ++i){
        Carte* carte = mg.partie.pioche.cartes[i];
        if(carte != nullptr){
            if(mg.espaceSelecteur == "Pioche" and mg.indiceSelecteur == static_cast<int>(i))
                dessinerCarte(mg, carte, mg.emplacementsPioche[i], scale);
            else
                dessinerCarte(mg, carte, mg.emplacementsPioche[i], 1);
        }
    }
    sf::Text piocheTxt(mg.font, "Pioche", 56);
    // On récupère les limites
    sf::FloatRect bounds = piocheTxt.getLocalBounds();
    // On met l'origine au centre et on positionne au bon endroit
    piocheTxt.setOrigin(bounds.position + bounds.size / 2.f);
    piocheTxt.setPosition(mg.txtPiochePosition);
    if(mg.partie.nombreJoueurs == 2 or mg.partie.nombreJoueurs > 3)
        piocheTxt.setRotation(sf::degrees(90));
    mg.window->draw(piocheTxt);


    // Affichage des grilles des joueurs
    for(unsigned int i = 0; i < mg.partie.nombreJoueurs; ++i){
        dessinerJoueur(mg, i);
    }

    // Si on est dans le choix de la carte visible
    if(mg.espaceSelecteur == "Choix Visible"){
        mg.window->draw(mg.fondChoixVisible);
        Carte* cartePioche = mg.partie.pioche.cartes[mg.coupActuel[1]-'0'];
        Carte* carteGrille = mg.partie.joueurs[mg.partie.prochainJoueur].grilleDeJeu[cartePioche->chiffre-1].faceVisible;
        // On dessine les deux choix
        if(mg.indiceSelecteur == 0){
            dessinerDosCarte(mg, mg.emplacementsChoixVisible[0]+mg.decalageDos, scale);
            dessinerCarte(mg, cartePioche, mg.emplacementsChoixVisible[0], scale);
            dessinerDosCarte(mg, mg.emplacementsChoixVisible[1]+mg.decalageDos, 1);
            dessinerCarte(mg, carteGrille, mg.emplacementsChoixVisible[1], 1);
        }else{
            dessinerDosCarte(mg, mg.emplacementsChoixVisible[0]+mg.decalageDos, 1);
            dessinerCarte(mg, cartePioche, mg.emplacementsChoixVisible[0], 1);
            dessinerDosCarte(mg, mg.emplacementsChoixVisible[1]+mg.decalageDos, scale);
            dessinerCarte(mg, carteGrille, mg.emplacementsChoixVisible[1], scale);
        }
    }

    mg.window->draw(*mg.selecteur); 
}

void dessinerCarte(MoteurGraphique& mg, Carte* c, sf::Vector2f centre, float facteurTaille){
    if(c != nullptr){
        // On récupère la position dans la boite de jeu de la carte
        unsigned int boiteIndice = c->boiteIndice;
        // On récupère le sprite qui est à cet indice dans notre liste de sprite
        mg.spritesCartes[boiteIndice]->setPosition(centre);
        mg.spritesCartes[boiteIndice]->setScale(mg.scaleFacteursCartes[boiteIndice]*facteurTaille);
        mg.window->draw(*mg.spritesCartes[boiteIndice]);
    }
}

void dessinerDosCarte(MoteurGraphique& mg, sf::Vector2f centre, float facteurTaille){
    // On récupère le sprite qui est à cet indice dans notre liste de sprite
    unsigned int i = mg.nbSpritesCartes - 1;
    mg.spritesCartes[i]->setPosition(centre);
    mg.spritesCartes[i]->setScale(mg.scaleFacteursCartes[i]*facteurTaille);
    mg.window->draw(*mg.spritesCartes[i]);
}

void dessinerJoueur(MoteurGraphique& mg, unsigned int joueur){
    // On dessine les cartes de la grille du joueur
    const Joueur& j = mg.partie.joueurs[joueur];
    for(unsigned int i = 0; i < 9; ++i){
        Carte* carteVisible = j.grilleDeJeu[i].faceVisible;
        Carte* carteCachee = j.grilleDeJeu[i].faceCachee;
        if(carteVisible == nullptr and carteCachee != nullptr)
            dessinerDosCarte(mg, mg.emplacementsGrille[joueur][i], 1);
        else{
            if(carteCachee != nullptr)
                dessinerDosCarte(mg, mg.emplacementsGrille[joueur][i]+mg.decalageDos, 1);
            dessinerCarte(mg, carteVisible, mg.emplacementsGrille[joueur][i], 1);
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
    if(mg.partie.nombreJoueurs == 2){
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
    else if(mg.partie.nombreJoueurs == 3){
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

void decalerSelecteur(MoteurGraphique& mg, int cote){
    // Si le sélecteur est dans la pioche
    if(mg.espaceSelecteur == "Pioche")
    {
        do // On décale l'indice du selecteur d'un cote jusqu'a ce qu'il soit sur une carte
        { 
            mg.indiceSelecteur = (mg.indiceSelecteur+cote)%mg.partie.pioche.taille;
        }while(mg.partie.pioche.cartes[mg.indiceSelecteur] == nullptr);
        mg.selecteur->setPosition(mg.emplacementsPioche[mg.indiceSelecteur]+mg.tailleCartes/2.f); // On change la position du selecteur
    }
    else if(mg.espaceSelecteur == "Grille")
    {
        do // On décale l'indice du selecteur d'un cote jusqu'a ce qu'il soit sur un emplacement vide
        {
            mg.indiceSelecteur = (9+mg.indiceSelecteur+cote)%9;
        }while(mg.partie.joueurs[mg.partie.prochainJoueur].grilleDeJeu[mg.indiceSelecteur].faceVisible != nullptr or mg.partie.joueurs[mg.partie.prochainJoueur].grilleDeJeu[mg.indiceSelecteur].faceCachee != nullptr);

        mg.selecteur->setPosition(mg.emplacementsGrille[mg.partie.prochainJoueur][mg.indiceSelecteur]+mg.tailleCartes/2.f);
    }
    else if(mg.espaceSelecteur == "Choix Visible")
    {
        mg.indiceSelecteur = (mg.indiceSelecteur+1)%2;
        mg.selecteur->setPosition(mg.emplacementsChoixVisible[mg.indiceSelecteur]+mg.tailleCartes/2.f);
    }
}

void gererUnChoix(MoteurGraphique& mg){

    // Si le sélecteur se trouvait dans la pioche
    if(mg.espaceSelecteur == "Pioche")
    {
        // On connait donc les deux premiers caractères du coup, le joueur et l'indice de la carte dans la pioche
        mg.coupActuel += std::to_string(mg.partie.prochainJoueur) + std::to_string(mg.indiceSelecteur);
        std::cout << "Coup qui sera joué " << mg.coupActuel << "..." << std::endl;

        // On va maintenant changer l'espace du selecteur selon l'emplacement où doit aller la carte choisie dans la grille du prochain joueur
        const Grille& grille = mg.partie.joueurs[mg.partie.prochainJoueur].grilleDeJeu;
        const Carte* carteChoisi = mg.partie.pioche.cartes[mg.indiceSelecteur];

        // Si il n'y a pas de carte face visible
        if(grille[carteChoisi->chiffre-1].faceVisible == nullptr)
        {
            // Alors on connait le coup et on peut le jouer, le joueur n'a plus rine à choisir.
            // Le coup est direct et la carte va à l'emplacement de son chiffre
            mg.coupActuel += "d"+ std::to_string(carteChoisi->chiffre-1);
            std::cout << "Coup qui sera joué " << mg.coupActuel << std::endl;
            jouerCoup(mg.partie, mg.coupActuel);
            std::cout << "Coup joué" << std::endl;
            gererFinDeCoup(mg);
        }
        // Si il y a une carte visible et aussi une cachée
        else if(grille[carteChoisi->chiffre-1].faceCachee != nullptr)
        {
            // On sait que le coup sera de la mettre autre part
            mg.coupActuel += "m";
            // On va attendre désormais le choix d'une case vide dans la grille
            changerEspaceSelecteur(mg, "Grille");
        }
        // Sinon, il y a une carte visible et pas de cachée
        else{
            // On va attendre le choix de la carte qui reste visible
            // Les deux emplacements du selecteur sont donc la carte choisi dans la pioche
            // Et la carte visible à son emplacement
            changerEspaceSelecteur(mg, "Choix Visible");
        }
    }


    // Si on attendait un chois parmi les emplacements de la grille
    else if(mg.espaceSelecteur == "Grille")
    {
        // On connait maintenant l'emplacement où va aller la carte
        // On peut donc écrire le coup et le jouer
        mg.coupActuel += std::to_string(mg.indiceSelecteur);
        std::cout << "Coup qui sera joué " << mg.coupActuel << std::endl;
        jouerCoup(mg.partie, mg.coupActuel);
        std::cout << "Coup joué."<< std::endl;
        gererFinDeCoup(mg);
    }


    // Si on attendait le choix de la carte qui sera visible entre deux
    else if(mg.espaceSelecteur == "Choix Visible")
    {
        // Si le choix est l'emplacement 0, qui est celui de la carte de la pioche
        if(mg.indiceSelecteur == 0)
            // Le coup est donc de mettre la carte pioché visible. On peut récupérer son indice dans la pioche
            // grâce au deuxième caractère du coup actuel donc aussi l'emplacement dans la grille
            mg.coupActuel += "v" + std::to_string(mg.partie.pioche.cartes[mg.coupActuel[1]-'0']->chiffre-1);
        else
            // Sinon cela veut dire qu'on la met face cachée, de même pour l'indice dans la pioche et l'emplacement
            // dans la grille
            mg.coupActuel += "c" + std::to_string(mg.partie.pioche.cartes[mg.coupActuel[1]-'0']->chiffre-1);
        // On joue le coup
        std::cout << "Coup qui sera joué " << mg.coupActuel << std::endl;
        jouerCoup(mg.partie, mg.coupActuel);
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
        while(mg.partie.pioche.cartes[mg.indiceSelecteur] == nullptr)
            // On décale vers la droite
            mg.indiceSelecteur = (mg.indiceSelecteur+1)%mg.partie.pioche.taille;
        // On met à jour sa position
        mg.selecteur->setPosition(mg.emplacementsPioche[mg.indiceSelecteur]);
    }
    else if(mg.espaceSelecteur == "Grille")
    {
        // On met le selecteur au premier emplacement de la grille du prochain joueur
        mg.indiceSelecteur = 0;
        // Tant que l'emplacement où se trouve le sélecteur contient une carte
        const Grille& grille = mg.partie.joueurs[mg.partie.prochainJoueur].grilleDeJeu;
        while(grille[mg.indiceSelecteur].faceVisible != nullptr or grille[mg.indiceSelecteur].faceCachee != nullptr)
            // On va à l'emplacement suivant
            mg.indiceSelecteur =  (mg.indiceSelecteur+1)%9;
        // On met à jour sa position
        mg.selecteur->setPosition(mg.emplacementsGrille[mg.partie.prochainJoueur][mg.indiceSelecteur]);
    }
    else if(mg.espaceSelecteur == "Choix Visible"){
        // On met le sélecteur à l'emplacement d'indice zero, il y a forcément une carte
        mg.indiceSelecteur = 0;
        mg.selecteur->setPosition(mg.emplacementsChoixVisible[mg.indiceSelecteur]);
    }
}

void gererFinDeCoup(MoteurGraphique& mg){
    // Si le choix d'avant à fait finir la manche
    if(mg.partie.estMancheFinie)
    {
        // On met tout à jour pour commencer la prochaine manche
        toutRemettreDansDefausse(mg.partie);
        // Si c'est la fin de la dernière manche
        if(mg.partie.numeroManche == 3)
        {
            supprimerDefausse(mg.partie.defausse);
            supprimerBoite(mg.partie.boite);
        }
        // Sinon
        else{
            // On remet toyt en place et on retourne le selecteur dans la pioche
            melanger(mg.partie.defausse);
            ++mg.partie.numeroManche;
            mg.partie.estMancheFinie = false;
            mg.partie.coupsManche.nombre = 0;
            remplirPioche(mg.partie.pioche, mg.partie.defausse);
            changerEspaceSelecteur(mg, "Pioche");
            // On remet le prochain coup vide
            mg.coupActuel = "";
        }
    }else{
        changerEspaceSelecteur(mg, "Pioche");
        // On remet le prochain coup vide
        mg.coupActuel = "";
    }
    
}