#include "Ordinateur.hpp"
#include "Partie.hpp"
#include "Grille.hpp"

void ajouterChoix(ListeDeChoix& listeChoix, std::string choix){
    listeChoix.choix[listeChoix.nbChoix] = choix;
    ++listeChoix.nbChoix;
}

void afficher(ListeDeChoix listeChoix){
    for(unsigned int i = 0; i < listeChoix.nbChoix; ++i)
        std::cout << listeChoix.choix[i] << std::endl;
}

ListeDeChoix recupChoix(const Partie& partie){
    ListeDeChoix choixPossible;
    choixPossible.nbChoix = 0;

    // Raccourci pour après, pour pas avoir à tout réecrire à chaque foix
    const Grille& griJoueur = partie.joueurs[partie.prochainJoueur].grilleDeJeu;

    Carte* carteChoisi;
    // Parcours des cartes de la pioche
    for(unsigned int i = 0; i < partie.pioche.taille; ++i){
        carteChoisi = partie.pioche.cartes[i];
        if(partie.pioche.cartes[i] != nullptr){
            // Si la carte sera place directement, c'est a dire son emplacement est vide
            if(estVideEmplacement(griJoueur, carteChoisi->chiffre-1)){
                ajouterChoix(choixPossible, std::to_string(i));
            
            }// Sinon si il y a seulement une carte face visible, les deux choix sont : garder la piochée visible ou pas
            else if(griJoueur[carteChoisi->chiffre-1].faceCachee == nullptr and griJoueur[carteChoisi->chiffre-1].faceVisible != nullptr){
                ajouterChoix(choixPossible, std::to_string(i)+" v");
                ajouterChoix(choixPossible, std::to_string(i)+" c");
            }// Sinon les choix sont les différentes cases vides
            else{
                for(unsigned int j = 0; j < 9; ++j){
                    if(j!=carteChoisi->chiffre and estVideEmplacement(griJoueur, j))
                        ajouterChoix(choixPossible, std::to_string(i)+" m "+ std::to_string(j));
                }
            }
        }
    }
    return choixPossible;
}