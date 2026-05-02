#include "Ordinateur.hpp"
#include "Partie.hpp"

ListeDeChoix recupChoix(Partie partie){
    ListeDeChoix choixPossible;
    choixPossible.nbChoix = 0;

    // Parcours des cartes de la pioche
    for(unsigned int i = 0; i < partie.pioche.taille; ++i){
        if(partie.pioche.cartes[i])
            return;
    }
}