#include <cmath>

#include "Ordinateur.hpp"
#include "Partie.hpp"
#include "Grille.hpp"

void initNoeud(Noeud* noeud, Partie& partie, std::string coupCreateur, Noeud* parent){
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

void ajouterEnfant(Noeud* noeud, Partie& partie){
    // On choisit le coup que l'on va visité, c'est le dernier de la liste des coups non visités
    std::string coupVisite = noeud->coupsNonVisites.coups[noeud->coupsNonVisites.nombre-1];
    --noeud->coupsNonVisites.nombre;
    // On joue ce coup
    jouerCoup(partie, coupVisite);
    // On crée un enfant et on l'initialise
    noeud->enfants[noeud->nbEnfants] = new Noeud;
    initNoeud(noeud->enfants[noeud->nbEnfants], partie, coupVisite, noeud);
    ++noeud->nbEnfants;
    // On annule le dernier coup joué pour rester ou on est actuellement.
    annulerDernierCoup(partie);
}

bool ajoutEnfantPossible(const Noeud* n){
    return n->coupsNonVisites.nombre > 0;
}

std::string recupMeilleurCoup(Partie& partie){
    //std::cout <<"Préparation de la recherche." << std::endl;
    
    // Liste des coups possibles pour l'ordinateur.
    ListeDeCoups coupsPossibles = recupCoupsPossibles(partie);

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

    // Information sur la recherche
    unsigned int nbDefaussesTestees = 20;
    unsigned int nbDescentesParDefausse = 400;

    //std::cout << "Début de la recherche" << std::endl;
    for(unsigned int j = 0; j < nbDefaussesTestees; ++j)
    {
        // Mélange de la défausse
        melanger(partie.defausse);
        //std::cout << "Défausse mélangée." << " | ";

        // Mise a jour de l'arbre, on enlève tout noeud qui se trouvait après un tirage de cartes et qui donc
        // dépendait de la défausse précédente. On change donc aussi les coups non visités de ceux juste avant un tirage
        metAJourArbre(&racine, partie);
        //std::cout << "Arbre nettoyé." << " | ";

        // Toutes les descentes dans l'arbre
        for(unsigned int k = 0; k < nbDescentesParDefausse; ++k){
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
                // On créer un nouveau noeud
                ajouterEnfant(noeudActuel, partie);
                //std::cout << "Nouveau noeud crée." << " | ";
                // On descend à ce noeud
                noeudActuel = noeudActuel->enfants[noeudActuel->nbEnfants-1];
                ++profondeur;
                //std::cout << "Profondeur " << profondeur << " | ";
                jouerCoup(partie, noeudActuel->coupCreateur);
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
        if(racine.enfants[i]->nbVictoires[partie.prochainJoueur] > racine.enfants[indiceMeilleurCoup]->nbVictoires[partie.prochainJoueur]){
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

    // On renvoie le meilleur coup :)
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

void jouerCoupAlea(Partie& partie){
    ListeDeCoups coupsPossibles = recupCoupsPossibles(partie);
    if(coupsPossibles.nombre == 0){
        std::cerr << "--- CRASH 2 JOUEURS ---" << std::endl;
        std::cerr << "Joueur actuel : " << partie.prochainJoueur << std::endl;
        std::cerr << "Nombres cartes : " << partie.pioche.nombreCartesRestantes << std::endl;
        std::cerr << "Un joueur a finit " << (unJoueurAFinit(partie) ? "Oui" : "Non")  << std::endl;
        std::cerr << "La manche est-elle finie ? " << (partie.estMancheFinie ? "Oui" : "Non") << std::endl;
        afficher(partie);
    }
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
    finirMancheAleatoirement(partie);
    // On remets à l'état initial
    unsigned int leader = recupLeader(partie);
    while(partie.coupsManche.nombre > nbCoupsInitial){
        annulerDernierCoup(partie);
    }
    return leader;
}

Noeud* choisirEnfant(Noeud* n, unsigned int joueur){
    // Le nombre totale de visites du parents, utiles pour le calcul du score UCT
    double nbTotalSimulations = n->nbVisites;
    // On parcours les enfants du noeud pour savoir lequel à la meilleur score UCT
    float meilleurUCT = -1;
    Noeud* meilleurEnfant = nullptr;
    for(unsigned int i = 0; i < n->nbEnfants; ++i){
        Noeud* enfant = n->enfants[i];
        float UCTscore = calculerUCT(nbTotalSimulations, n->enfants[i]->nbVisites, calculerRatioVictoire(enfant, joueur), 1.41);
        if(UCTscore > meilleurUCT){
            meilleurUCT = UCTscore;
            meilleurEnfant = n->enfants[i];
        }   
    }
    return meilleurEnfant;
}

float calculerRatioVictoire(Noeud* n, unsigned int joueur){
    return 1.0*n->nbVictoires[joueur]/n->nbVisites;
}

float calculerUCT(int nbVisitesParent, int nbVisitesEnfant, float ratioVictoire, float temperature){
    float exploration = std::sqrt(std::log(nbVisitesParent) / nbVisitesEnfant);
    return ratioVictoire + temperature * exploration;
}

// Supprimes les noeuds qui se trouvent après le premier tirage de carte dans la défausse et met à jour les coups non visités selon la défausse.
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

void supprimerArbre(Noeud* noeudRacine){
    if(noeudRacine != nullptr){
        // On supprime les enfants du noeud
        for(unsigned int i = 0; i < noeudRacine->nbEnfants; i++){
            supprimerArbre(noeudRacine->enfants[i]);
        }
        // On supprime le noeudRacine
        delete noeudRacine;
    }
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