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

    // On initialise l'espace du selecteur et le coup actuel
    mg.espaceSelecteur = "Pioche";
    mg.coupActuel = "";
    
    // On crée la fenêtre et on met une limite aux fps
    mg.window = new sf::RenderWindow(sf::VideoMode({1280, 720}), "Pixies");
    mg.window->setFramerateLimit(60);
    
    // Tant que la fenêtre est ouverte et que la partie n'est pas finie
    while (mg.window->isOpen() and not (mg.partie.estMancheFinie and mg.partie.numeroManche == 3))
    {      
        // On regarde pour les input
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
                else if (keyPressed->code == sf::Keyboard::Key::Enter and not mg.partie.joueurs[mg.partie.prochainJoueur].estOrdi) {
                    gererUnChoix(mg);
                }
            }
        }
        // On dessine tout après avoir nettoyer l'écran
        mg.window->clear(sf::Color::Black);
        dessinerTout(mg);
        mg.window->display();
        // Tant que c'est à l'ordi de jouer après
        if(mg.partie.joueurs[mg.partie.prochainJoueur].estOrdi and not mg.partie.estMancheFinie){
            // On fait jouer le prochain joueur en mode ordi
            jouerCoup(mg.partie, recupMeilleurCoup(mg.partie, 40, 400));
            gererFinDeCoup(mg);
        }
        
    }
    // Ensuite quand la partie est finie
    while(mg.window->isOpen()){
        // On ferme la fenêtre si on appuie sur entrer ou sur la croix.
        while (const std::optional event = mg.window->pollEvent())
        {
            if (event->is<sf::Event::Closed>()){
                mg.window->close();
            }
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Enter) {
                    mg.window->close();
                }
            }
        }
        // On dessine les scores
        mg.window->clear(sf::Color::Black);
        // Affichage du fond
        sf::RectangleShape fond({1280, 720});
        fond.setTexture(&mg.textureFond);
        mg.window->draw(fond);
        // Pour chaque joueurs
        for(unsigned int i = 0; i < mg.partie.nombreJoueurs; ++i){
            std::string string = mg.partie.joueurs[i].surnom + " " + std::to_string(mg.partie.joueurs[i].nbPoints) + "pts";
            sf::Text joueurText(mg.font, string, 40);
            sf::Rect rect = joueurText.getLocalBounds();
            joueurText.setOrigin(rect.position + rect.size / 2.f);
            joueurText.setPosition({640.f, 320.f + 60*i});
            mg.window->draw(joueurText);
        }
        // Dessin du texte pour quitter
        sf::Text quitterText(mg.font, "Appuyer sur ENTRER pour quitter", 40);
        sf::Rect rect = quitterText.getLocalBounds();
        quitterText.setOrigin(rect.position + rect.size / 2.f);
        quitterText.setPosition({640.f, 320.f + 60*mg.partie.nombreJoueurs});
        mg.window->draw(quitterText);
        // Mettre a jour l'affichage
        mg.window->display();
    }
    // Quand on quite on affiche les points dans la console
    std::cout << "La partie est terminée. Voici les scores :" << std::endl;
    for(unsigned int i = 0; i < mg.partie.nombreJoueurs; ++i){
        std::cout << mg.partie.joueurs[i].surnom << "  " << mg.partie.joueurs[i].nbPoints <<"pts" << std::endl;
    }
    // On libère la mémoire
    supprimerDefausse(mg.partie.defausse);
    supprimerBoite(mg.partie.boite);
}

void initialiserTexturesEtSprites(MoteurGraphique& mg){
    // ------ Creation des sprites des cartes -----
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
    sf::Vector2f tailleFondChoix(400.f, 200.f);
    mg.fondChoixVisible.setSize(tailleFondChoix);
    mg.fondChoixVisible.setOrigin(tailleFondChoix/2.f);
    mg.fondChoixVisible.setFillColor(sf::Color(55, 55, 55, 200));
    mg.fondChoixVisible.setOutlineColor(sf::Color::White);
    mg.fondChoixVisible.setOutlineThickness(4);
    mg.fondChoixVisible.setPosition({640.f, 360.f});

    // On charge la texture du sélecteur et crée son sprite
    mg.textureSelecteur = sf::Texture("assets/icone-de-feuille-verte.png");
    mg.selecteur = sf::Sprite(mg.textureSelecteur);
    sf::Vector2u dimTexture = mg.textureSelecteur.getSize();
    mg.selecteur->setScale({80.f / dimTexture.x, 80.f / dimTexture.y});
    mg.selecteur->setOrigin(sf::Vector2f{dimTexture.x / 2.f, dimTexture.y / 2.f});
    changerEspaceSelecteur(mg, "Pioche");

    // On initialise le fond quand l'ordi réfléchi
    mg.fondOrdi = sf::RectangleShape({400.f, 200.f});
    mg.fondOrdi.setOrigin({200.f, 100.f});
    mg.fondOrdi.setPosition({640.f, 360.f});
    mg.fondOrdi.setFillColor(sf::Color(50, 50, 50, 200));
    mg.fondOrdi.setOutlineColor(sf::Color::White);
    mg.fondOrdi.setOutlineThickness(4);
}

void dessinerTout(MoteurGraphique& mg){
    // Temps depuis le début pour les animations
    float t = mg.clock.getElapsedTime().asSeconds();
    // Facteur d'agranddisement pour les animations
    float scale = 1.0 + 0.06f * std::sin(t * 2.5);

    // Affichage du fond
    sf::RectangleShape fond({1280, 720});
    fond.setTexture(&mg.textureFond);
    mg.window->draw(fond);

    // Affichage des cartes de la pioche et de son texte
    for(unsigned int i = 0; i < mg.partie.pioche.taille; ++i){
        Carte* carte = mg.partie.pioche.cartes[i];
        if(carte != nullptr){
            // Si la carte est à l'emplacement ou est le selecteur
            if(mg.espaceSelecteur == "Pioche" and mg.indiceSelecteur == static_cast<int>(i))
                dessinerCarte(mg, carte, mg.emplacementsPioche[i], scale); // On la dessine agrandi un peu
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
    if(mg.partie.nombreJoueurs == 2) // A deux joueurs le texte est a la vertical
        piocheTxt.setRotation(sf::degrees(90));
    mg.window->draw(piocheTxt);


    // Affichage des grilles des joueurs
    for(unsigned int i = 0; i < mg.partie.nombreJoueurs; ++i){
        dessinerJoueur(mg, i);
    }

    // Si on est dans le choix de la carte visible
    if(mg.espaceSelecteur == "Choix Visible"){
        mg.window->draw(mg.fondChoixVisible); // On dessine le fond
        // On récupère les deux cartes
        Carte* cartePioche = mg.partie.pioche.cartes[mg.coupActuel[1]-'0'];
        Carte* carteGrille = mg.partie.joueurs[mg.partie.prochainJoueur].grilleDeJeu[cartePioche->chiffre-1].faceVisible;
        // On met selon la position du selecteur, les facteurs d'agrandissements des deux emplacements
        std::array<float, 2> facteurs;
        if(mg.indiceSelecteur == 0){
            facteurs = {scale, 1};
        }else{
            facteurs = {1, scale};
        }
        // On dessine les 4 cartes (2 visibles + 2 cachées)
        dessinerDosCarte(mg, mg.emplacementsChoixVisible[0], facteurs[0]);
        dessinerCarte(mg, cartePioche, mg.emplacementsChoixVisible[0], facteurs[0]);
        dessinerDosCarte(mg, mg.emplacementsChoixVisible[1], facteurs[1]);
        dessinerCarte(mg, carteGrille, mg.emplacementsChoixVisible[1], facteurs[1]);
    }
    // Sinon si on est en train de choisir un emplacement dans la grille
    else if(mg.espaceSelecteur == "Grille"){
        // On dessine un dos de carte ou est le selecteur
        dessinerDosCarte(mg, mg.emplacementsGrille[mg.partie.prochainJoueur][mg.indiceSelecteur]-mg.decalageDos, scale);
    }

    mettreAJourPositionSelecteur(mg);
    mg.window->draw(*mg.selecteur);

    // Si le prochain jouer est un ordi
    if(mg.partie.joueurs[mg.partie.prochainJoueur].estOrdi){
        // On ecrit que l'ordi réflchi
        sf::Text ordiAttente(mg.font, "Attente du coup de l'ordi.", 43);
        sf::Rect txtRect = ordiAttente.getLocalBounds();
        ordiAttente.setFillColor(sf::Color::Red);
        ordiAttente.setOrigin(txtRect.position + txtRect.size /2.f);
        ordiAttente.setPosition({640.f, 360.f});
        mg.window->draw(mg.fondOrdi);
        mg.window->draw(ordiAttente);
    }
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
    mg.spritesCartes[i]->setPosition(centre+mg.decalageDos);
    mg.spritesCartes[i]->setScale(mg.scaleFacteursCartes[i]*facteurTaille);
    mg.window->draw(*mg.spritesCartes[i]);
}

void dessinerJoueur(MoteurGraphique& mg, unsigned int joueur){
    float facteurCartes;
    if(mg.partie.prochainJoueur == joueur)
        facteurCartes = 1;
    else
        facteurCartes = 0.8;
    // On dessine les cartes de la grille du joueur
    const Joueur& j = mg.partie.joueurs[joueur];
    for(unsigned int i = 0; i < 9; ++i){
        Carte* carteVisible = j.grilleDeJeu[i].faceVisible;
        Carte* carteCachee = j.grilleDeJeu[i].faceCachee;
        if(carteVisible == nullptr and carteCachee != nullptr)
            dessinerDosCarte(mg, mg.emplacementsGrille[joueur][i]-mg.decalageDos, facteurCartes);
        else{
            if(carteCachee != nullptr)
                dessinerDosCarte(mg, mg.emplacementsGrille[joueur][i], facteurCartes);
            dessinerCarte(mg, carteVisible, mg.emplacementsGrille[joueur][i], facteurCartes);
        }
    }
    // Dessin du nom et des points du joueur 
    sf::Color couleur;
    if(mg.partie.prochainJoueur == joueur)
        couleur = sf::Color::Green;
    else
        couleur = sf::Color::White;
    sf::Text ptsTxt(mg.font, std::to_string(j.nbPoints)+" pts", 26);
    ptsTxt.setFillColor(couleur);
    // On récupère les limites de cahque texte
    int nomTaillePolice = 48;
    sf::Text nomTxt(mg.font, j.surnom, nomTaillePolice);
    while(nomTxt.getLocalBounds().size.x > 2.1*mg.tailleCartes.x){
        nomTaillePolice--;
        nomTxt = sf::Text(mg.font, j.surnom, nomTaillePolice);
    }
    nomTxt.setFillColor(couleur);

    sf::FloatRect nomRect = nomTxt.getLocalBounds();
    sf::FloatRect ptsRect = ptsTxt.getLocalBounds();
    
    if(mg.partie.nombreJoueurs != 5){
        // On met l'origine des textes au centre
        nomTxt.setOrigin({nomRect.position.x, nomRect.position.y + nomRect.size.y});
        ptsTxt.setOrigin(ptsRect.position + ptsRect.size);
        // On positionne les textes
        nomTxt.setPosition(mg.emplacementsGrille[joueur][0] - mg.tailleCartes/2.f + sf::Vector2f(0, 1.3*mg.decalageDos.y));
        ptsTxt.setPosition(mg.emplacementsGrille[joueur][2] + sf::Vector2f(mg.tailleCartes.x/2.f, -mg.tailleCartes.y/2.f) + sf::Vector2f(0, 1.3*mg.decalageDos.y));
    }else{
        nomTxt.setOrigin({nomRect.position.x, nomRect.position.y + nomRect.size.y});
        ptsTxt.setOrigin(ptsRect.position + ptsRect.size);
        nomTxt.setPosition(mg.emplacementsGrille[joueur][2] + sf::Vector2f(mg.tailleCartes.x/2.f - mg.decalageDos.y, -mg.tailleCartes.y/2.f));
        ptsTxt.setPosition(mg.emplacementsGrille[joueur][8] + sf::Vector2f(mg.tailleCartes.x/2.f - mg.decalageDos.y, mg.tailleCartes.y/2.f - mg.decalageDos.x));
        nomTxt.setRotation(sf::degrees(90));
        ptsTxt.setRotation(sf::degrees(90));
    }
    // On les dessine
    mg.window->draw(nomTxt);
    mg.window->draw(ptsTxt);
}

void initialiserDispositionEcran(MoteurGraphique& mg){
    mg.emplacementsChoixVisible = {sf::Vector2f(540.f, 360.f), sf::Vector2f(740.f, 360.f)};
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
    }else if(mg.partie.nombreJoueurs == 4){
        mg.tailleCartes = sf::Vector2f(65.f, 95.f);
        sf::Vector2f& t = mg.tailleCartes;

        mg.decalageDos = sf::Vector2f(-6.f, -6.f);
        sf::Vector2f ecartCarte(12.f, 15.f);

        mg.txtPiochePosition = {150.f, 140.f};
        for(unsigned int i = 0; i < 4; ++i){
            mg.emplacementsPioche[i] = {350.f + i * (t.x + ecartCarte.x), 140.f};
        }

        // 4 grilles alignées verticalement au même niveau (Y = 450)
        sf::Vector2f centre;
        for(unsigned int j = 0; j < 4; ++j){
            // On espace les centres de 300 pixels sur l'axe X
            centre = {140.f + j * 330.f, 500.f};

            for(int empl = 0; empl < 9; ++empl){
                sf::Vector2f decalage = {(-1 + empl % 3) * (t.x + ecartCarte.x), (-1 + empl / 3) * (t.y + ecartCarte.y)};
                mg.emplacementsGrille[j][empl] = {centre + decalage};
            }
        }
    }
    else if(mg.partie.nombreJoueurs == 5){
        mg.tailleCartes = sf::Vector2f(65.f, 95.f);
        sf::Vector2f& t = mg.tailleCartes;

        mg.decalageDos = sf::Vector2f(-6.f, -6.f);
        sf::Vector2f ecartCarte(12.f, 15.f);

        mg.txtPiochePosition = {200.f, 180.f};
        for(unsigned int i = 0; i < 5; ++i){
            mg.emplacementsPioche[i] = {400.f + i * (t.x + ecartCarte.x), 180.f};
        }

        sf::Vector2f centre;
        for(unsigned int j = 0; j < 4; ++j){
            centre = {160.f + j * 300.f, 520.f};

            for(int empl = 0; empl < 9; ++empl){
                sf::Vector2f decalage = {(-1 + empl % 3) * (t.x + ecartCarte.x), (-1 + empl / 3) * (t.y + ecartCarte.y)};
                mg.emplacementsGrille[j][empl] = {centre + decalage};
            }
        }
        centre = {1060.f, 180.f};

        for(int empl = 0; empl < 9; ++empl){
            sf::Vector2f decalage = {(-1 + empl % 3) * (t.x + ecartCarte.x), (-1 + empl / 3) * (t.y + ecartCarte.y)};
            mg.emplacementsGrille[4][empl] = {centre + decalage};
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
            mg.indiceSelecteur = (mg.partie.pioche.taille+mg.indiceSelecteur+cote)%mg.partie.pioche.taille;
        }while(mg.partie.pioche.cartes[mg.indiceSelecteur] == nullptr);
        mg.positionViseeSelecteur = mg.emplacementsPioche[mg.indiceSelecteur]+mg.tailleCartes/2.f; // On change la position du selecteur
        mg.selecteurEnMouvement = true;
    }
    else if(mg.espaceSelecteur == "Grille")
    {
        do // On décale l'indice du selecteur d'un cote jusqu'a ce qu'il soit sur un emplacement vide
        {
            mg.indiceSelecteur = (9+mg.indiceSelecteur+cote)%9;
        }while(mg.partie.joueurs[mg.partie.prochainJoueur].grilleDeJeu[mg.indiceSelecteur].faceVisible != nullptr or mg.partie.joueurs[mg.partie.prochainJoueur].grilleDeJeu[mg.indiceSelecteur].faceCachee != nullptr);

        mg.positionViseeSelecteur = mg.emplacementsGrille[mg.partie.prochainJoueur][mg.indiceSelecteur]+mg.tailleCartes/2.f;
        mg.selecteurEnMouvement = true;
    }
    else if(mg.espaceSelecteur == "Choix Visible")
    {
        mg.indiceSelecteur = (mg.indiceSelecteur+1)%2;
        mg.positionViseeSelecteur = mg.emplacementsChoixVisible[mg.indiceSelecteur]+mg.tailleCartes/2.f;
        mg.selecteurEnMouvement = true;
    }
}

void gererUnChoix(MoteurGraphique& mg){

    // Si le sélecteur se trouvait dans la pioche
    if(mg.espaceSelecteur == "Pioche")
    {
        // On connait donc les deux premiers caractères du coup, le joueur et l'indice de la carte dans la pioche
        mg.coupActuel += std::to_string(mg.partie.prochainJoueur) + std::to_string(mg.indiceSelecteur);
        

        // On va maintenant changer l'espace du selecteur selon l'emplacement où doit aller la carte choisie dans la grille du prochain joueur
        const Grille& grille = mg.partie.joueurs[mg.partie.prochainJoueur].grilleDeJeu;
        const Carte* carteChoisi = mg.partie.pioche.cartes[mg.indiceSelecteur];

        // Si il n'y a pas de carte face visible
        if(grille[carteChoisi->chiffre-1].faceVisible == nullptr)
        {
            // Alors on connait le coup et on peut le jouer, le joueur n'a plus rine à choisir.
            // Le coup est direct et la carte va à l'emplacement de son chiffre
            mg.coupActuel += "d"+ std::to_string(carteChoisi->chiffre-1);
            jouerCoup(mg.partie, mg.coupActuel);
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
        jouerCoup(mg.partie, mg.coupActuel);
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
        jouerCoup(mg.partie, mg.coupActuel);
        gererFinDeCoup(mg);
    }
}

void changerEspaceSelecteur(MoteurGraphique &mg, std::string espace){
    mg.espaceSelecteur = espace;
    if(mg.espaceSelecteur == "Pioche")
    {
        // On met le selecteur au premier emplacement de la pioche
        mg.indiceSelecteur = 0;
        // Tant qu'il n'y a pas de carte à l'endroit du sélecteur
        while(mg.partie.pioche.cartes[mg.indiceSelecteur] == nullptr)
            // On décale vers la droite
            mg.indiceSelecteur = (mg.partie.pioche.taille+mg.indiceSelecteur+1)%mg.partie.pioche.taille;
        // On met à jour sa position
        mg.positionViseeSelecteur = mg.emplacementsPioche[mg.indiceSelecteur]+mg.tailleCartes/2.f;
        mg.selecteurEnMouvement = true;
    }
    else if(mg.espaceSelecteur == "Grille")
    {
        // On met le selecteur au premier emplacement de la grille du prochain joueur
        mg.indiceSelecteur = 0;
        // Tant que l'emplacement où se trouve le sélecteur contient une carte
        const Grille& grille = mg.partie.joueurs[mg.partie.prochainJoueur].grilleDeJeu;
        while(grille[mg.indiceSelecteur].faceVisible != nullptr or grille[mg.indiceSelecteur].faceCachee != nullptr)
            // On va à l'emplacement suivant
            mg.indiceSelecteur =  (9+mg.indiceSelecteur+1)%9;
        // On met à jour sa position
        mg.positionViseeSelecteur = mg.emplacementsGrille[mg.partie.prochainJoueur][mg.indiceSelecteur]+mg.tailleCartes/2.f;
        mg.selecteurEnMouvement = true;
    }
    else if(mg.espaceSelecteur == "Choix Visible"){
        // On met le sélecteur à l'emplacement d'indice zero, il y a forcément une carte
        mg.indiceSelecteur = 0;
        mg.positionViseeSelecteur = mg.emplacementsChoixVisible[mg.indiceSelecteur]+mg.tailleCartes/2.f;
        mg.selecteurEnMouvement = true;
    }
}

void mettreAJourPositionSelecteur(MoteurGraphique&mg){
    if(mg.selecteurEnMouvement){
        float distance = std::sqrt(std::pow(mg.selecteur->getPosition().x - mg.positionViseeSelecteur.x, 2) + std::pow(mg.selecteur->getPosition().y - mg.positionViseeSelecteur.y, 2));
        if(distance > 10)
            mg.selecteur->setPosition(mg.selecteur->getPosition() + (mg.positionViseeSelecteur-mg.selecteur->getPosition())*0.3f);
        else{
            mg.selecteur->setPosition(mg.positionViseeSelecteur);
            mg.selecteurEnMouvement = false;
        }
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
            // On fait tout ça pour pas que ça crash mais l'app va se fermer direct de toute manière
            melanger(mg.partie.defausse);
            remplirPioche(mg.partie.pioche, mg.partie.defausse);
            changerEspaceSelecteur(mg, "Pioche");
            // On remet le prochain coup vide
            mg.coupActuel = "";
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