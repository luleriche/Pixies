#include <cmath>
#include <ctime>
#include <fstream>

//Pour passer les string en flux
#include <sstream>

#include "Ordinateur.hpp"
#include "Partie.hpp"
#include "Grille.hpp"

std::string recupMeilleurCoup(Partie& partie, const unsigned int nbDefausse, const unsigned int nbDescentesParDefausses)
{
    // Début du chronomètre
    clock_t debutRecherche = clock();
    
    
    //std::cout <<"Préparation de la recherche." << std::endl;
    // Liste des coups possibles pour l'ordinateur.
    ListeDeCoupsPossibles coupsPossibles = recupCoupsPossibles(partie);

    // Si il n'y a qu'un coup possible on ne réfléchit pas on le renvoie.
    if(coupsPossibles.nombre == 1){
        return coupsPossibles.coups[0];
    }

    // On crée et initialise le noeud racine
    Noeud racine;
    racine.coupCreateur = "NULL";
    racine.parent = nullptr;
    racine.nbEnfants = 0;
    racine.enfants.fill(nullptr);
    racine.nbVictoires = {0, 0, 0, 0, 0};
    racine.nbVisites = 0;
    racine.coupsNonVisites = coupsPossibles;
    
    // On stock les pointeurs de la défausse initiale pour la reconstruire après car on va la mélanger 20 fois
    unsigned int tailleDefausse = recupTaille(partie.defausse);
    Carte* * ptrDefausseInitiale = new Carte*[tailleDefausse];
    maillon* tmp = partie.defausse;
    for(unsigned int i = 0; i < tailleDefausse; ++i){
        ptrDefausseInitiale[i] = tmp->valeur;
        tmp = tmp->suivant;
    }

    //std::cout << "Début de la recherche" << std::endl;
    for(unsigned int j = 0; j < nbDefausse; ++j)
    {
        // Mélange de la défausse
        melanger(partie.defausse);
        //std::cout << "Défausse mélangée." << " | ";

        // Mise a jour de l'arbre, on enlève tout noeud qui se trouvait après un tirage de cartes et qui donc
        // dépendait de la défausse précédente. On change donc aussi les coups non visités de ceux juste avant un tirage
        metAJourArbre(&racine, partie);
        //std::cout << "Arbre nettoyé." << " | ";

        // Toutes les descentes dans l'arbre
        for(unsigned int k = 0; k < nbDescentesParDefausses; ++k){
            //std::cout << "Descente numéro" << k << " | ";
            // On se place à la racine
            Noeud* noeudActuel = &racine;
            unsigned int profondeur = 0;

            // On descend intelligement à un endroit où on n'a pas testé tous les coups ou à un endroit où la partie est finie.
            // Tant que on a déja visités tous les coups et que la partie n'est pas finie.
            while(noeudActuel->coupsNonVisites.nombre == 0 and not partie.estMancheFinie){
                // On va dans le noeud le plus intéressant à explorer
                noeudActuel = choisirEnfant(noeudActuel, partie.prochainJoueur);
                ++profondeur;
                //std::cout << "Profondeur " << profondeur << " | ";
                
                // Il faut aussi penser à jouer le coup qui y amène pour changer l'état de la partie.
                jouerCoup(partie, noeudActuel->coupCreateur);
            }
            
            // Si il y a un coup à découvrir 
            if(noeudActuel->coupsNonVisites.nombre > 0){
                // On créer un nouveau noeud et on y descend
                noeudActuel = ajouterEnfant(noeudActuel, partie);
                //std::cout << "Nouveau noeud crée." << " | ";
                ++profondeur;
            }
            // On simule une partie aléatoire à partir de là et on récupère celui qui gagne
            unsigned int gagnant = recupLeaderFinMancheAleatoire(partie);
            
            // On remonte l'arbre en mettant à jour les statistiques de chaque noeud où l'on est passé
            while(noeudActuel->parent != nullptr){
                ++noeudActuel->nbVictoires[gagnant];
                ++noeudActuel->nbVisites;
                // On annule les coups en passant
                annulerDernierCoup(partie);
                noeudActuel = noeudActuel->parent;
                --profondeur;
                //std::cout << "Profondeur " << profondeur << " | "; 
            }

            // On met aussi à jour les statistiques de la racine
            ++noeudActuel->nbVictoires[gagnant];
            ++noeudActuel->nbVisites;
            //std::cout << "Retourné en haut de l'arbre." << " | ";
        }
    }

    // On récupère l'enfant qui a amené le plus de fois à une victoire pour le prochain joueur
    //std::cout << "Calcul du coup le plus gagnant." << " | ";
    unsigned indiceMeilleurCoup = 0;
    for(unsigned int i = 1; i < racine.nbEnfants; ++i){
        if(calculerRatioVictoire(racine.enfants[i], partie.prochainJoueur) > calculerRatioVictoire(racine.enfants[indiceMeilleurCoup], partie.prochainJoueur)){
            indiceMeilleurCoup = i;
        }
    }

    // On récupère le coup qui a mené à cet enfant
    std::string meilleurCoup = racine.enfants[indiceMeilleurCoup]->coupCreateur;
    //std::cout << "Meilleur coup " << meilleurCoup << " | ";
    
    // On désalloue l'arbre en désallouant les enfants de la racine
    for(unsigned int i = 0; i < racine.nbEnfants; ++i){
        supprimerArbre(racine.enfants[i]);
    }

    // On remet la défausse comme avant
    tmp = partie.defausse;
    for(unsigned int i = 0; i < tailleDefausse; ++i){
        tmp->valeur = ptrDefausseInitiale[i];
        tmp = tmp->suivant;
    }
    // On désalloue la sauvegarde de notre défausse
    delete[] ptrDefausseInitiale;

    clock_t finRecherche = clock();
    double duree = (double)(finRecherche - debutRecherche) / CLOCKS_PER_SEC;
    std::cout << "Temps de recherche : " << duree << " secondes" << std::endl;

    // On renvoie le meilleur coup :)
    return meilleurCoup;
}

void initNoeud(Noeud* const noeud, const Partie& partie, const std::string& coupCreateur, Noeud* const parent){
    // On initialise les différentes valeurs du noeud
    noeud->coupCreateur = coupCreateur;
    noeud->parent = parent;
    noeud->nbEnfants = 0;
    noeud->enfants.fill(nullptr);
    noeud->nbVictoires = {0, 0, 0, 0, 0};
    noeud->nbVisites = 0;

    // On récupère les coups possibles dans l'état actuel de la partie.
    // Il faut donc initialiser un noeud après avoir joué le coup créateur.
    noeud->coupsNonVisites = recupCoupsPossibles(partie);
}

ListeDeCoupsPossibles recupCoupsPossibles(const Partie& partie){
    ListeDeCoupsPossibles coupsPossibles;
    coupsPossibles.nombre = 0;
    if(partie.estMancheFinie)
    return coupsPossibles;
    
    // Raccourcis pour après, pour pas avoir à tout réecrire à chaque foix
    const unsigned int& joueur = partie.prochainJoueur;
    const Grille& griJoueur = partie.joueurs[joueur].grilleDeJeu;
    
    Carte* carteChoisi;
    // Indice dans la grille ou sera envoyé la carte
    unsigned int indiceDestGrille;
    
    // Parcours des cases de la pioche
    for(unsigned int i = 0; i < partie.pioche.taille; ++i){
        // Si il y a une carte à cet endroit
        if(partie.pioche.cartes[i] != nullptr){
            carteChoisi = partie.pioche.cartes[i];
            // La destination de base est le chiffre de la carte
            indiceDestGrille = carteChoisi->chiffre-1;
            // Si à cette destination il n'y a pas de carte visible, il n'y a qu'un coup possible
            if(griJoueur[indiceDestGrille].faceVisible == nullptr){
                ajouterCoup(coupsPossibles, std::to_string(joueur)+std::to_string(i)+"d"+std::to_string(indiceDestGrille));
            }// Sinon si il y a seulement une carte face visible, deux coups sont possibles : garder la carte piochée visible ou pas
            else if(griJoueur[indiceDestGrille].faceCachee == nullptr){
                ajouterCoup(coupsPossibles, std::to_string(joueur)+std::to_string(i)+"v"+std::to_string(indiceDestGrille));
                ajouterCoup(coupsPossibles, std::to_string(joueur)+std::to_string(i)+"c"+std::to_string(indiceDestGrille));
            }// Sinon les coups possibles sont les différentes cases vides où on peut mettre la carte
            else{
                for(unsigned int j = 0; j < 9; ++j){
                    if(estVideEmplacement(griJoueur, j))
                    ajouterCoup(coupsPossibles, std::to_string(joueur)+std::to_string(i)+"m"+std::to_string(j));
                }
            }
        }
    }
    return coupsPossibles;
}

void annulerDernierCoup(Partie& partie){
    // Si la manche était terminé alors on enlève les points des joueurs d'abord et on change l'état
    if(partie.estMancheFinie){
        for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
            partie.joueurs[i].nbPoints -= comptePoints(partie.joueurs[i].grilleDeJeu, partie.numeroManche);
        }
        partie.estMancheFinie = false;
    }
    
    std::string coup = recupDernierCoup(partie);
    // Joueur du coup précedent
    unsigned int joueurPrecedent = coup[0] - '0';
    // Type du coup
    char typeCoup = coup[2];
    // Indice où se trouvait la carte jouée dans la pioche avant
    unsigned int indiceDestPioche = coup[1] - '0';
    // Indice où a été mise la carte joué dans la grille
    unsigned int indiceCarteGrille = coup[3] - '0';
    
    // Si la pioche est pleine, avant elle ne contenait qu'une seule carte. Il faut donc remettre les cartes de la pioche dans la défausse.
    if(estPleinePioche(partie.pioche)){
        // On prends les cartes de la pioche en partant de la dernière et on les remets au dessus de la défausse.
        mettrePiocheDansDefausse(partie.pioche, partie.defausse);
    }
    
    // Référence vers la grille du joueur qui a joué avant
    Grille& griJoueur = partie.joueurs[joueurPrecedent].grilleDeJeu;
    
    // Si le coup était de la mettre directement dans sa case.
    if(typeCoup == 'd'){
        // On la met dans la pioche à son ancienne place et on l'enlève de la grille
        partie.pioche.cartes[indiceDestPioche] = griJoueur[indiceCarteGrille].faceVisible;
        griJoueur[indiceCarteGrille].faceVisible = nullptr;
    }
    // Si le coup était de choisir la carte jouée comme cachée face à celle qui était là avant
    else if(typeCoup == 'c'){
        // On la met dans la pioche à son ancienne place et on l'enlève de la grille
        partie.pioche.cartes[indiceDestPioche] = griJoueur[indiceCarteGrille].faceCachee;
        griJoueur[indiceCarteGrille].faceCachee = nullptr;
    }
    // Si le coup était de choisir la carte jouée comme visible face à celle qui était la avant
    else if(typeCoup == 'v'){
        // On la met dans la pioche à son ancienne place, remet l'ancienne carte visible et on l'enlève de la grille
        partie.pioche.cartes[indiceDestPioche] = griJoueur[indiceCarteGrille].faceVisible;
        griJoueur[indiceCarteGrille].faceVisible = griJoueur[indiceCarteGrille].faceCachee;
        griJoueur[indiceCarteGrille].faceCachee = nullptr;
    }
    // Si le coup était de la mettre dans une autre case vide
    else{
        // On la met dans la pioche à son ancienne place et l'enlève de la grille
        partie.pioche.cartes[indiceDestPioche] = griJoueur[indiceCarteGrille].faceCachee;
        griJoueur[indiceCarteGrille].faceCachee = nullptr;
    }
    ++partie.pioche.nombreCartesRestantes;
    // On enlève le coup de la liste des coups de la manche
    supprimeDernierCoup(partie.coupsManche);
    // On remet le joueur au joueur précédent
    partie.prochainJoueur = joueurPrecedent;
}

void jouerCoupAlea(Partie& partie){
    ListeDeCoupsPossibles coupsPossibles = recupCoupsPossibles(partie);
    jouerCoup(partie, coupsPossibles.coups[rand()%coupsPossibles.nombre]);
}

void jouerCoupMaximisePoints(Partie& partie){
    ListeDeCoupsPossibles coupsPossibles = recupCoupsPossibles(partie);
    const unsigned int joueur = partie.prochainJoueur;
    std::string meilleurCoup = coupsPossibles.coups[0];
    int meilleursPoints = -100;
    // On parcours les coups possibles pour le joueur
    for(unsigned int i = 0; i < coupsPossibles.nombre; ++i){
        jouerCoup(partie, coupsPossibles.coups[i]);
        int nbPoints = comptePoints(partie.joueurs[joueur].grilleDeJeu, partie.numeroManche);
        if(nbPoints > meilleursPoints){
            meilleursPoints = nbPoints;
            meilleurCoup = coupsPossibles.coups[i];
        }
        annulerDernierCoup(partie);
    }
    jouerCoup(partie, meilleurCoup);
}

unsigned int recupLeaderFinMancheAleatoire(Partie& partie){
    // On stock le nombre de coups joués pour y revenir après
    unsigned int nbCoupsInitial = partie.coupsManche.nombre;
    // On va au bout de la manche et on stock le leader
    while(not partie.estMancheFinie){
        jouerCoupAlea(partie);
    }
    // On remets à l'état initial
    unsigned int leader = recupLeader(partie);
    while(partie.coupsManche.nombre > nbCoupsInitial){
        annulerDernierCoup(partie);
    }
    return leader;
}

Noeud* ajouterEnfant(Noeud* const noeudActuel, Partie& partie){
    // On choisit le coup que l'on va joué, c'est le dernier de la liste des coups non visités
    std::string coupVisite = noeudActuel->coupsNonVisites.coups[noeudActuel->coupsNonVisites.nombre-1];
    --noeudActuel->coupsNonVisites.nombre;
    // On joue ce coup
    jouerCoup(partie, coupVisite);
    // On crée un enfant et on l'initialise
    noeudActuel->enfants[noeudActuel->nbEnfants] = new Noeud;
    initNoeud(noeudActuel->enfants[noeudActuel->nbEnfants], partie, coupVisite, noeudActuel);
    ++noeudActuel->nbEnfants;
    // On renvoie le pointeur vers le noeud enfant crée
    return noeudActuel->enfants[noeudActuel->nbEnfants-1];
}

Noeud* choisirEnfant(const Noeud* const n, unsigned int joueur){
    // Le nombre totale de visites du parents, utiles pour le calcul du score UCT
    unsigned int nbTotalSimulations = n->nbVisites;
    // On parcours les enfants du noeud pour savoir lequel à la meilleur score UCT
    float meilleurUCT = -1;
    Noeud* meilleurEnfant = nullptr;
    for(unsigned int i = 0; i < n->nbEnfants; ++i){
        Noeud* enfant = n->enfants[i];
        // On utilise comme température une valeur approchée de racine de 2
        float UCTscore = calculerUCT(nbTotalSimulations, n->enfants[i]->nbVisites, calculerRatioVictoire(enfant, joueur), 1.41);
        if(UCTscore > meilleurUCT){
            meilleurUCT = UCTscore;
            meilleurEnfant = n->enfants[i];
        }
    }
    return meilleurEnfant;
}

void supprimerArbre(Noeud* const noeudRacine){
    if(noeudRacine != nullptr){
        // On supprime les enfants du noeud
        for(unsigned int i = 0; i < noeudRacine->nbEnfants; i++){
            supprimerArbre(noeudRacine->enfants[i]);
        }
        // On supprime le noeudRacine
        delete noeudRacine;
    }
}

void metAJourArbre(Noeud* noeud, Partie& partie){
    // Si il ne reste qu'une seule carte dans la défausse
    if(partie.pioche.nombreCartesRestantes == 1){
        // Alors il faut supprimer les enfants de chacun de nos enfants
        // Car ils représentent des coups qui dépendent des cartes qui seront tirées de la défausse
        for(unsigned int i = 0; i < noeud->nbEnfants; ++i){
            // Suppression des enfants des enfants
            Noeud* const enfant = noeud->enfants[i];
            for(unsigned int j = 0; j < enfant->nbEnfants; ++j){
                supprimerArbre(enfant->enfants[j]);
                enfant->enfants[j] = nullptr;
            }
            enfant->nbEnfants = 0;
            // Mais aussi remettre à jour leur coups non visités
            jouerCoup(partie, enfant->coupCreateur);
            enfant->coupsNonVisites = recupCoupsPossibles(partie);
            annulerDernierCoup(partie);
        }
    }
    // Sinon on descend dans l'arbre
    else{
        for(unsigned int i = 0; i < noeud->nbEnfants; ++i){
            jouerCoup(partie, noeud->enfants[i]->coupCreateur);
            metAJourArbre(noeud->enfants[i], partie);
            annulerDernierCoup(partie);
        }
    }
}

float calculerRatioVictoire(const Noeud* const n, unsigned int joueur){
    return 1.0*n->nbVictoires[joueur]/n->nbVisites;
}

float calculerUCT(const int nbVisitesParent, const int nbVisitesEnfant, const float ratioVictoire, const float temperature){
    float exploration = std::sqrt(std::log(nbVisitesParent) / nbVisitesEnfant);
    return ratioVictoire + temperature * exploration;
}

// ---------- CODE POUR LA LECTURE D'UN FICHIER ---------

void lireFichierPartie(const std::string nomFichier, Partie& partie){
    std::ifstream fichier;
    fichier.open(nomFichier);

    //---- Initialisation de la partie avant la lecture dans le fichier ----
    
    // Création des cartes dans la mémoire à l'aide d'une boite de cartes
    creerCartesAvecFichier("assets/cartes_pixies.txt", partie.boite);

    // initialisation de la défausse
    initDefausse(partie.defausse);
    remplir(partie.boite, partie.defausse);

    // On met la taille de la pioche à 5
    partie.pioche.taille = 5;

    // Creation des joueurs
    partie.nombreJoueurs = 5;
    for(unsigned int i = 0; i < 5; ++i){
        initJoueur(partie.joueurs[i], &partie.pioche, std::to_string(i), false);
    }

    partie.estMancheFinie = false;
    partie.coupsManche.nombre = 0;

    if(fichier.is_open()){
        std::cout << "Fichier de la partie correctement ouvert." << std::endl;
        // Le premier élément du fichier est le numero de la manche
        fichier >> partie.numeroManche;
        // Ensuite il y a les points des joueurs
        for(unsigned int i = 0; i < 5; ++i){
            fichier >> partie.joueurs[i].nbPoints;
        }
        std::string finDeLigne;
        std::getline(fichier, finDeLigne);
        std::cout << "Tous les points des joueurs ont été lu." << std::endl;
        // Puis ça altèrne entre les 5 cinqs cartes la pioche et les coups
        do{
            // Lecture des 5 coups
            unsigned int i = 0;
            std::string stringCarte;
            // Tant qu'on a pas lu cinq cartes et que la lecture se passe bien
            while(i < 5 and std::getline(fichier, stringCarte)){
                std::cout << "Ligne de carte lue : " << stringCarte << std::endl;
                Carte cartePioche = lireCarteDeFichier(stringCarte);
                partie.pioche.cartes[i] = tirerCartePrecise(partie.defausse, cartePioche);
                ++partie.pioche.nombreCartesRestantes;
                std::cout << "Carte ajouté à la pioche :"; afficherEnCouleur(*partie.pioche.cartes[i]); std::cout << std::endl;
                // On ajoute 1 au compteur de cartes lues
                ++i;
            }
            // Si on a pas lu 5 cartes dans la défausse, il y a un problème
            if(i < 5){
                std::cout << "Il n'y a pas toutes les cartes de la pioche à la fin du fichier !" << std::endl;
                return;
            }
            std::cout << "Pioche pleine :" << std::endl; afficher(partie.pioche); std::cout << std::endl;
            
            // On stock le nombre de coups qu'on va joué pour savoir quand c'est la fin
            unsigned int nbCoupsJoues = 0;
            
            std::string ligneCoup;
            // Tant que la lecture de la prochaine ligne se passe bien et qu'on a joué moins de 5 coups
            while(nbCoupsJoues < 5 and std::getline(fichier, ligneCoup)){
                std::cout << "Ligne de coup lue : " << ligneCoup << std::endl;
                // On traduit la ligne pour avoir le coup à jouer dans notre format
                std::string coup = traduireCoupFichierVersNormal(ligneCoup, partie);
                std::cout << "Le coup qui va être joué est " << coup << std::endl;

                // Si c'est le premier coup de la manche on met la valeur premier joueur, car elle n'est pas renseigné avant dans le fichier
                if(partie.coupsManche.nombre == 0)
                    // Le joueur est le premier caractère du coup
                    partie.prochainJoueur = coup[0]- '0';
    
                // On joue le coup
                jouerCoup(partie, coup);
                ++nbCoupsJoues;
                std::cout << "Un coup a été joué." << std::endl;
                afficher(partie);
            }
            // Si On s'est arrêté car on a joué 5 coups
            if(nbCoupsJoues == 5){
                std::cout << "5 coups ont été joués." << std::endl;
                // Il faut vider la pioche car le dernier coup l'a remplit avec les cartes au dessus de la défausse
                mettrePiocheDansDefausse(partie.pioche, partie.defausse);
                std::cout << "La pioche a été vidée et remise dans la défausse." << std::endl;
                afficher(partie);
            }
        }while(fichier.good() and not partie.estMancheFinie);
    }else
        std::cout << "Erreur à l'ouverture du fichier de la partie." << std::endl;
}

void jouerCoupOrdiEtEcrire(Partie& partie, std::string nomFichier){
    std::cout << "Début du calcul du meilleur coup." << std::endl;
    std::string coup = recupMeilleurCoup(partie, 20, 400);
    std::string coupFichier = traduireCoupNormalVersFichier(coup, partie);
    std::cout << "Le meilleur coup a été trouvé et traduit pour le fichier." << std::endl;
    std::cout << "Coup " << coup << std::endl << "Coup écrit " << coupFichier << std::endl;
    std::ofstream fichier;
    // On ouvre le fichier à la fin
    fichier.open(nomFichier, std::ios::app);
    if(fichier.is_open()){
        std::cout << "Le fichier s'est ouvert correctement." << std::endl;
        // On met un saut de ligne et notre coup
        fichier << std::endl << coupFichier;
        std::cout << "Le coup a bien été écrit dans le fichier." << std::endl;
    }else{
        std::cout << "L'ouverture du fichier a échoué." << std::endl;
    }
}

std::string traduireCoupNormalVersFichier(std::string coup, const Partie& partie){
    std::string traduction = "";
    // Le premier caractère est le numéro du joueur, dans notre format c'était l'indice du joueur
    traduction += std::to_string(coup[0] - '0' + 1);
    // Ensuite il y a une virgule
    traduction += ',';
    // Ensuite il y a le chiffre, la couleur, les spirales et si la carte est spéciale ou non.
    Carte* carteChoisie = partie.pioche.cartes[coup[1]-'0'];
    traduction += std::to_string(carteChoisie->chiffre) + ' ' + carteChoisie->couleur + ' ';
    if(carteChoisie->spirale == 9){
        traduction += "0 1";
    }else{
        traduction += std::to_string(carteChoisie->spirale) + " 0";
    }
    // Ensuite une virgule, la case de destination de la carte et une autre virgule
    traduction += ',' + std::to_string(coup[3] - '1' +1) + ',';
    // Et le coup se termine par si la carte finit visible ou caché
    if(coup[2] == 'm' or coup[2] == 'c'){
        traduction += 'c';
    }else{
        traduction += 'v';
    }
    return traduction;
}

std::string traduireCoupFichierVersNormal(const std::string& ligneCoup, const Partie& partie){
    // On passe le string en flux de lecture
    std::stringstream ss(ligneCoup);
    
    // VARIABLES POUR LA LECTURE
    // Numéro du joueur (0, 1, 2... ou 4)
    unsigned int joueur;
    // Indice de la case de destination de la carte dans la grille (0, 1, 2... ou 8)
    unsigned int indiceDestGrille;
    // Chiffre qui indique si la carte est spéciale ou non (0 ou 1)
    unsigned int speciale;
    // Type du coup (v ou c dans le fichier - v,c,d ou m dans notre format)
    char typeCoup;
    // Virgule du fichier à ne pas prendre en compte
    char virgule;
    // Carte qui est choisie par le joueur parmi celle de la pioche
    Carte carteChoisi;

    // Lecture des données du fichier
    ss >> joueur >> virgule >> carteChoisi.chiffre >> carteChoisi.couleur >> carteChoisi.spirale >> speciale >> virgule >> indiceDestGrille >> virgule >> typeCoup;
    // Si la lecture s'est bien passé, c'est à dire il y avait vraiment un coup dans le fichier
    // Correction dans le cas où la carte choisie est spéciale, dans notre format on met un neuf
    if(speciale == 1)
        carteChoisi.spirale = 9;
    // Correction dans le cas où la carte choisie est multicolore, dans notre format on met un s
    if(carteChoisi.couleur == 'n')
        carteChoisi.couleur = 's';
    // Correction des indice, dans les coups du fichier on met les numéros de la case et du joueur, dans notre format les indices
    --indiceDestGrille; --joueur;
    // On récpère l'indice de la carte choisie dans la piche car dans notre format on indique cet indice et non la carte elle même.
    unsigned int indiceCarteChoisie = indiceCartePioche(partie.pioche, carteChoisi);
    
    // On s'occupe maintenant de trouver le type du coup selon la grille du joueur
    if(indiceDestGrille != carteChoisi.chiffre-1)
        // Si la carte ne va pas dans son emplacement, alors le coup est forcément de type m, pour mettre
        typeCoup = 'm';
    else if(partie.joueurs[joueur].grilleDeJeu[indiceDestGrille].faceVisible == nullptr)
        // Si il n'y a pas de carte face visible à l'emplacement de la carte, alors le coup est forcément direct
        typeCoup = 'd';
    // Sinon on garde le v ou le c car et le seul restant. Ce sont les coups quand il y a déja une carte face visible à l'emplacement.

    // On forme le coup avec les différentes informations que l'on a récuupéré
    std::string coup = std::to_string(joueur)+std::to_string(indiceCarteChoisie)+typeCoup+std::to_string(indiceDestGrille);

    return coup;
}

Carte lireCarteDeFichier(const std::string& txtCarte){
    // On transforme la ligne en un flux de lecture
    std::stringstream ss(txtCarte);
    // On lit les attributs de la carte (chiffre, couleur, spirale)
    Carte carte;
    int speciale; // Nombre qui indiquera si la carte est spéciale ou non
    ss >> carte.chiffre >> carte.couleur >> carte.spirale >> speciale;
    if(speciale == 1) // On gère le cas ou la carte est spéciale
        carte.spirale = 9;
    if(carte.couleur == 'n') // On change aussi la couleur si elle est multicolore car dans notre format c'est un s
        carte.couleur = 's';
    return carte;
}
