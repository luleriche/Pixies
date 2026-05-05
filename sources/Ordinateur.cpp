#include "Ordinateur.hpp"
#include "Partie.hpp"
#include "Grille.hpp"


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