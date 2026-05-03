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

    // Raccourci pour après, pour pas avoir à tout réecrire à chaque foix
    const Grille& griJoueur = partie.joueurs[partie.prochainJoueur].grilleDeJeu;

    Carte* carteChoisi;
    // Parcours des cartes de la pioche
    for(unsigned int i = 0; i < partie.pioche.taille; ++i){
        if(partie.pioche.cartes[i] != nullptr){
            carteChoisi = partie.pioche.cartes[i];
            // Si la carte sera place directement, c'est a dire son emplacement est vide
            if(estVideEmplacement(griJoueur, carteChoisi->chiffre-1)){
                ajouterCoup(coupsPossibles, std::to_string(i+1)+" d");
            
            }// Sinon si il y a seulement une carte face visible, les deux choix sont : garder la piochée visible ou pas
            else if(griJoueur[carteChoisi->chiffre-1].faceCachee == nullptr and griJoueur[carteChoisi->chiffre-1].faceVisible != nullptr){
                ajouterCoup(coupsPossibles, std::to_string(i+1)+" v");
                ajouterCoup(coupsPossibles, std::to_string(i+1)+" c");
            }// Sinon les choix sont les différentes cases vides où on peut mettre la carte
            else{
                for(unsigned int j = 0; j < 9; ++j){
                    if(j!=carteChoisi->chiffre and estVideEmplacement(griJoueur, j))
                        ajouterCoup(coupsPossibles, std::to_string(i+1)+" m "+ std::to_string(j+1));
                }
            }
        }
    }
    return coupsPossibles;
}

void jouerCoup(Partie& partie, std::string coup){
    // Indice de la carte choisi par le coup dans la pioche
    unsigned int indiceCarteChoisi = coup[0] - '1';
    std::cout << indiceCarteChoisi;
    // Pointeur vers la carte choisie
    Carte* cartePiochee = partie.pioche.cartes[indiceCarteChoisi];
    // Type de coup voulant être joué
    char typeCoup = coup[2];
    
    // Référence vers la grille du joueur qui va jouer
    Grille& griJoueur = partie.joueurs[partie.prochainJoueur].grilleDeJeu;

    // On enlève la carte piochée de la pioche
    partie.pioche.cartes[indiceCarteChoisi] = nullptr;

    // Si le coup est de la mettre directement dans sa case.
    if(typeCoup == 'd'){
        griJoueur[cartePiochee->chiffre-1].faceVisible = cartePiochee;
    }
    // Si le coup eest de choisir la carte piochée comme cachée face à celle qui était la avant
    else if(typeCoup == 'c'){
        griJoueur[cartePiochee->chiffre-1].faceCachee = cartePiochee;
    }
    // Si le coup eest de choisir la carte piochée comme visible face à celle qui était la avant
    else if(typeCoup == 'v'){
        griJoueur[cartePiochee->chiffre-1].faceCachee = griJoueur[cartePiochee->chiffre-1].faceVisible;
        griJoueur[cartePiochee->chiffre-1].faceVisible = cartePiochee;
    }
    // Si le coup est de la mettre dans une autre case vide
    else{
        unsigned int caseChoisie = coup[4] - '0';
        griJoueur[caseChoisie-1].faceVisible = cartePiochee;
    }
}