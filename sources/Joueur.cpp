#include <string>

#include "Joueur.hpp"
#include "Grille.hpp"
#include "Defausse.hpp"
#include "Pioche.hpp"

void initJoueur(Joueur & j, Pioche* p, std::string surnom, bool estOrdi){
    j.surnom = surnom;
    initGrille(j.grilleDeJeu);
    j.piocheCommune = p;
    j.nbPoints = 0;
    j.estOrdi = estOrdi;
}

void creerJoueurs(std::array<Joueur, 5>& joueurs, const int nbJoueurs, Pioche* piocheCommune) {
    std::string surnom;
    for (int i = 0; i < nbJoueurs; i++) {
        std::cout << "Surnom du joueur " << i+1 << " : ";
        std::cin >> surnom;
        char carOrdi;
        std::cout << "Ce joueur est-il un ordinateur ? (y / n) : ";
        std::cin >> carOrdi;
        while(carOrdi != 'y' and carOrdi != 'n'){
            std::cout << "Erreur ! Ce joueur est-il un ordinateur ? (y / n) : ";
            std::cin >> carOrdi;
        }
        if(carOrdi == 'y')
            initJoueur(joueurs[i], piocheCommune, surnom, true);
        else if(carOrdi == 'n')
            initJoueur(joueurs[i], piocheCommune, surnom, false);
    }
}

void ajoutePointsDeGrille(Joueur& j, const unsigned int numeroManche){
    j.nbPoints += comptePoints(j.grilleDeJeu, numeroManche);
}