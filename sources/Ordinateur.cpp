#include "Ordinateur.hpp"
#include "Partie.hpp"
#include "Grille.hpp"

void annulerDernierCoup(Partie& partie){
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