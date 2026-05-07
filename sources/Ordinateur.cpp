#include <cmath>

#include "Ordinateur.hpp"
#include "Partie.hpp"
#include "Grille.hpp"

void initNoeud(Noeud& noeud, Partie& partie, std::string coup, Noeud* parent){
    // On initialise les différentes valeurs du noeud
    noeud.coupCreateur = coup;
    noeud.parent = parent;
    noeud.nbEnfants = 0;
    noeud.nbVictoires = {0, 0, 0, 0, 0};
    noeud.nbVisites = 0;

    // On récupère les coups possibles dans l'état actuel de la partie.
    // Il faut donc initialiser un noeud après avoir joué le coup si c'est un enfant
    // Sinon si c'est la racine il ne faut jouer aucun coup.
    noeud.coupsNonVisites = recupCoupsPossibles(partie);
}

void ajouterEnfant(Noeud& noeud, Partie& partie){
    // On choisit le coup que l'on va visité, c'est le dernier de la liste des coups non visités
    std::string coupVisite = noeud.coupsNonVisites.coups[noeud.coupsNonVisites.nombre-1];
    --noeud.coupsNonVisites.nombre;
    // On joue ce coup
    jouerCoup(partie, coupVisite);
    // On crée un enfant et on l'initialise
    noeud.enfants[noeud.nbEnfants] = new Noeud;
    initNoeud(*noeud.enfants[noeud.nbEnfants], partie, coupVisite, &noeud);
    ++noeud.nbEnfants;
    // On annule le dernier coup joué pour rester ou on est actuellement.
    annulerDernierCoup(partie);
}

bool ajoutEnfantPossible(const Noeud& n){
    return n.coupsNonVisites.nombre > 0;
}

std::string recupMeilleurCoup(Partie& partie){
    std::cout <<"Préparation de la recherche." << std::endl;
    
    // Liste des coups possibles pour l'ordinateur.
    ListeDeCoups coupsPossibles = recupCoupsPossibles(partie);
    // Compteurs qui vont compter le nombre de fois qu'un coup est le meilleur
    std::array<unsigned int, NbMaxCoups> compteursMeilleursCoups = {0};

    // Si il n'y a qu'un coup possible on n'a pas à réfléchir
    if(coupsPossibles.nombre == 1){
        return coupsPossibles.coups[0];
    }

    // On crée  et initialise le noeud racine
    Noeud racine;
    racine.coupCreateur = "NULL";
    racine.parent = nullptr;
    racine.nbEnfants = 0;
    racine.nbVictoires = {0};
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

    std::cout << "---------- Début de la recherche ----------" << std::endl;
    // On fait la recherche du meilleur coups pour 20 défausses différentes.
    for(unsigned int j = 0; j < 20; ++j){
        std::cout << "        Défausse numéro " << j << "        " << std::endl;
        // Mélange de la défausse
        melanger(partie.defausse);
        // On fait 1000 descentes
        for(int k = 0; k < 50; ++k){
            std::cout << "Simulation n°" << k << std::endl;
            // On commence à la racine
            Noeud* noeudActuel = &racine;
            std::cout << "Début de la descente dans l'arbre." << std::endl;
            // On descend intelligement à un endroit où on n'a pas testé tous les coups
            // Tant que on a déja testé tous les enfants actuels et que la partie n'est pas finie
            while(not ajoutEnfantPossible(*noeudActuel) and not partie.estMancheFinie){
                // On va dans le noeud le plus intéressant à explorer
                noeudActuel = choisirEnfant(noeudActuel, partie.prochainJoueur);
                // Et on joue le coup qui y amène
                jouerCoup(partie, noeudActuel->coupCreateur);
            }
            std::cout << "Arrivé en bas de l'arbre." << std::endl;
            // Une fois arrivé à un endroite où on peut ajouter un noeud (vérifier que l'on peut car c'est possible d'être à la fin)
            if(ajoutEnfantPossible(*noeudActuel)){
                std::cout << "Création d'un nouveau noeud." << std::endl;
                // On créer un nouveau noeud
                ajouterEnfant(*noeudActuel, partie);
                // On descend à ce noeud
                noeudActuel = noeudActuel->enfants[noeudActuel->nbEnfants-1];
                jouerCoup(partie, noeudActuel->coupCreateur);
            }
            std::cout << "Nouveau noeud enfant crée." << std::endl;
            // On simule une partie aléatoire à partir de là et on récupère celui qui gagne
            unsigned int gagnant = recupLeaderFinMancheAleatoire(partie);
            
            // On retourne tout en haut en mettant à jour les statistiques de chaque noeud où l'on passe
            std::cout << "Début de la remontée de l'arbre." << std::endl;
            while(noeudActuel->parent != nullptr){
                ++noeudActuel->nbVictoires[gagnant];
                ++noeudActuel->nbVisites;
                // On annule les coups en passant
                annulerDernierCoup(partie);
                noeudActuel = noeudActuel->parent;
            }
            // On met aussi à jour les statistiques de la racine
            ++noeudActuel->nbVictoires[gagnant];
            ++noeudActuel->nbVisites;
            std::cout << "Retourné en haut de l'arbre. Noeud actuel parent :" << noeudActuel->parent << std::endl;
        }
        // Maintenant que on a fait toutes les descentes et simulations nécéssaires pour être précis.
        // On regarde quel coup au début à donner le meilleur ratio de victoires
        unsigned int indiceMeilleurRatio = 0;
        float meilleurRatio = calculerRatioVictoire(*racine.enfants[0], partie.prochainJoueur);
        std::cout << "Coup 0  " << racine.enfants[0]->coupCreateur << " : ratio de victoire " << meilleurRatio << std::endl;
        for(unsigned int i = 1; i < racine.nbEnfants; ++i){
            float noeudRatio = calculerRatioVictoire(*racine.enfants[i], partie.prochainJoueur);
            if(noeudRatio > meilleurRatio){
                meilleurRatio = noeudRatio;
                indiceMeilleurRatio = i;
            }
            std::cout << "Coup " << i << " " << racine.enfants[i]->coupCreateur << " : ratio de victoire " << noeudRatio << std::endl;
        }
        // On ajoute 1 au compteur des meilleurs coups pour celui qui vient de l'être avec une défausse aléatoire
        ++compteursMeilleursCoups[indiceMeilleurRatio];
        std::cout << "Défausse numéro "<< j << " : le coup " << racine.enfants[indiceMeilleurRatio]->coupCreateur << " a eu le meilleur ratio pour cette défausse." << std::endl;
        // On désalloue l'arbre de recherche et reinialise la racine
        reinitRacine(racine, coupsPossibles);
        std::cout << "Arbre de recherche détruit." << std::endl;
    }
    std::cout << "Calcul du coup le plus gagnant." << std::endl;
    // On récupère celui qui a le plus de fois été le meilleur coup
    unsigned indiceMeilleurCoup = 0;
    for(unsigned int i = 1; i < coupsPossibles.nombre; i++){
        if(compteursMeilleursCoups[i] > compteursMeilleursCoups[indiceMeilleurCoup]){
            indiceMeilleurCoup = i;
        }
    }
    std::string meilleurCoup = coupsPossibles.coups[indiceMeilleurCoup];
    std::cout << "L'ordi a décidé que " << meilleurCoup << " était le meilleur coup à jouer." << std::endl;
    
    // On remet la défausse comme avant
    tmp = partie.defausse;
    for(unsigned int i = 0; i < tailleDefausse; ++i){
        tmp->valeur = ptrDefausseInitiale[i];
        tmp = tmp->suivant;
    }
    delete[] ptrDefausseInitiale;
    return meilleurCoup;
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

ListeDeCoups recupCoupsPossibles(const Partie& partie){
    ListeDeCoups coupsPossibles;
    coupsPossibles.nombre = 0;

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
                    if(j!=indiceDestGrille and estVideEmplacement(griJoueur, j))
                        ajouterCoup(coupsPossibles, std::to_string(joueur)+std::to_string(i)+"m"+std::to_string(j));
                }
            }
        }
    }
    return coupsPossibles;
}

void jouerCoupAlea(Partie& partie){
    ListeDeCoups coupsPossibles = recupCoupsPossibles(partie);
    jouerCoup(partie, coupsPossibles.coups[rand()%coupsPossibles.nombre]);
}

void finirMancheAleatoirement(Partie& partie){
    while(not partie.estMancheFinie){
        jouerCoupAlea(partie);
    }
}

unsigned int recupLeaderFinMancheAleatoire(Partie& partie){
    // On stock le nombre de coups joués pour y revenir après
    unsigned int nbCoupsInitial = partie.coupsManche.nombre;
    // On va au bout de la manche et on stock le leader
    std::cout << "Début de la simulation. Nombre de coups :" << nbCoupsInitial << std::endl;
    finirMancheAleatoirement(partie);
    // On remets à l'état initial
    std::cout << "Fin de la simulation. Nombre de coups :" << partie.coupsManche.nombre << std::endl;
    unsigned int leader = recupLeader(partie);
    std::cout << "Début de l'annulation de tous les coups joués." << std::endl;
    while(partie.coupsManche.nombre > nbCoupsInitial){
        annulerDernierCoup(partie);
    }
    std::cout << "Tous les coups ont été annulés. Nombre de coups de la partie :" << partie.coupsManche.nombre << std::endl;
    return leader;
}

Noeud* choisirEnfant(Noeud* n, unsigned int joueur){
    // Le nombre totale de visites du parents, utiles pour le calcul du score UCT
    double nbTotalSimulations = n->nbVisites;
    // On parcours les enfants du noeud pour savoir lequel à la meilleur score UCT
    float meilleurUCT = -1;
    Noeud* meilleurEnfant;
    for(unsigned int i = 0; i < n->nbEnfants; ++i){
        Noeud* enfant = n->enfants[i];
        float UCTscore = calculerUCT(nbTotalSimulations, n->enfants[i]->nbVisites, calculerRatioVictoire(*enfant, joueur), 1.41);
        if(UCTscore > meilleurUCT){
            meilleurUCT = UCTscore;
            meilleurEnfant = n->enfants[i];
        }   
    }
    return meilleurEnfant;
}

float calculerRatioVictoire(Noeud n, unsigned int joueur){
    return 1.0*n.nbVictoires[joueur]/n.nbVisites;
}

float calculerUCT(int nbVisitesParent, int nbVistesEnfant, float ratioVictoire, float temperature){
    float exploration = std::sqrt(std::log(nbVisitesParent) / nbVistesEnfant);
    return ratioVictoire + temperature * exploration;
}

void supprimerArbre(Noeud* ptrRacine){
    if (ptrRacine == nullptr) return;
    // On supprime les arbres qui commence aux enfants du noeud
    for (unsigned int i = 0; i < ptrRacine->nbEnfants; ++i) {
        supprimerArbre(ptrRacine->enfants[i]);
    }
    delete ptrRacine;
}

void reinitRacine(Noeud& racine, ListeDeCoups coupsNonVisites){
    // Pour chaque noeud enfant de la racine
    for (unsigned int i = 0; i < racine.nbEnfants; ++i) {
        if (racine.enfants[i] != nullptr){
            // On supprime tous les noeuds qui suivent et l'enfant lui même
            supprimerArbre(racine.enfants[i]);
            racine.enfants[i] = nullptr;
        }
    }
    racine.nbEnfants = 0;
    racine.nbVictoires = {0};
    racine.nbVisites = 0;
    racine.coupsNonVisites = coupsNonVisites;
}

/*

CODE POUR PAS PERDRE LA DEFAUSSE DU DEBUT

// On stock les pointeurs de la défausse initiale pour la reconstruire après
    unsigned int tailleDefausse = recupTaille(partie.defausse);
    Carte* * ptrDefausseInitiale = new Carte*[tailleDefausse];
    maillon* tmp = partie.defausse;
    for(unsigned int i = 0; i < tailleDefausse; ++i){
        ptrDefausseInitiale[i] = tmp->valeur;
        tmp = tmp->suivant;
    }


    // On remet la défausse comme avant
    tmp = partie.defausse;
    for(unsigned int i = 0; i < tailleDefausse; ++i){
        tmp->valeur = ptrDefausseInitiale[i];
        tmp = tmp->suivant;
    }
    delete[] ptrDefausseInitiale;

*/