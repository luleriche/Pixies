#include "Ordinateur.hpp"
#include "Partie.hpp"
#include "Grille.hpp"

void ajouterCoup(ListeDeCoups& lc, std::string coup){
    lc.coups[lc.nombre] = coup;
    ++lc.nombre;
}

void afficher(ListeDeCoups lc){
    for(unsigned int i = 0; i < lc.nombre; ++i)
        std::cout << lc.coups[i] << std::endl;
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

void jouerCoup(Partie& partie, std::string coup){
    // Joueur qui joue le coup
    unsigned int joueur = coup[0] - '0';
    // Indice de la carte à prendre dans la pioche
    unsigned int indiceCartePioche = coup[1] - '0';
    // Pointeur vers la carte à jouer
    Carte* cartePiochee = partie.pioche.cartes[indiceCartePioche];
    // Type du coup voulant être joué
    char typeCoup = coup[2];
    // Indice où la carte sera mise dans la grille
    unsigned int indiceDestGrille = coup[3] - '0';
    
    // Référence vers la grille du joueur qui va jouer
    Grille& griJoueur = partie.joueurs[joueur].grilleDeJeu;

    // On enlève la carte à jouer de la pioche
    partie.pioche.cartes[indiceCartePioche] = nullptr;

    // Si le coup est de la mettre directement dans sa case face visible.
    if(typeCoup == 'd'){
        griJoueur[indiceDestGrille].faceVisible = cartePiochee;
    }
    // Si le coup est de choisir la carte choisie comme cachée face à celle qui était la avant
    else if(typeCoup == 'c'){
        // La carte face visible reste la même (celle qui était là avant)
        // On met la carte face cachée
        griJoueur[indiceDestGrille].faceCachee = cartePiochee;
    }
    // Si le coup est de choisir la carte choisie comme visible face à celle qui était la avant
    else if(typeCoup == 'v'){
        // La carte qui était visible avant devient cachée et on mets celle choisis en tant que visible
        griJoueur[indiceDestGrille].faceCachee = griJoueur[indiceDestGrille].faceVisible;
        griJoueur[indiceDestGrille].faceVisible = cartePiochee;
    }
    // Si le coup est de la mettre dans une autre case vide
    else{
        griJoueur[indiceDestGrille].faceCachee = cartePiochee;
    }
}

void annulerCoup(Partie& partie, std::string coup){
    // Joueur qui a joué le coup
    unsigned int joueurPrecedent = coup[0] - '0';
    // Type du coup qui a été joué
    char typeCoup = coup[2];
    // Indice où se trouvait la carte jouée dans la pioche avant
    unsigned int indiceDestPioche = coup[1] - '0';
    // Indice où a été mise la carte joué dans la grille
    unsigned int indiceCarteGrille = coup[3] - '0';

    // Si la pioche est pleine, avant elle ne contenait qu'une seule carte. Il faut donc remettre les cartes de la pioche dans la défausse.
    if(estPleinePioche(partie.pioche)){
        // On prends les cartes de la pioche en partant de la dernière et on les remets au dessus de la défausse.
        for(unsigned int i = partie.pioche.taille; i >= 0; --i){
            ajoutDebutDefausse(partie.defausse, partie.pioche.cartes[i]);
            partie.pioche.cartes[i] = nullptr;
        }
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
}