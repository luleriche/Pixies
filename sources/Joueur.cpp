#include <string>

#include "Joueur.hpp"
#include "Grille.hpp"
#include "Defausse.hpp"
#include "Pioche.hpp"

void initJoueur(Joueur & j, Pioche* p, std::string surnom){
    j.surnom = surnom;
    initGrille(j.grilleDeJeu);
    j.piocheCommune = p;
    j.nbPoints = 0;
}

void creerJoueurs(std::array<Joueur, 5>& joueurs, int nbJoueurs, Pioche* piocheCommune) {
    std::string surnom;
    for (int i = 0; i < nbJoueurs; i++) {
        std::cout << "Surnom du joueur " << i+1 << " : ";
        std::cin >> surnom;
        initJoueur(joueurs[i], piocheCommune, surnom);
    }
}

void changerDeJoueur(unsigned int& joueurCourant, unsigned int nbJoueurs){
    joueurCourant = (joueurCourant +1) % nbJoueurs;
}

void finDeTour(unsigned int& joueurDebut, unsigned int nbJoueurs) {
    joueurDebut = (joueurDebut - 1 + nbJoueurs) % nbJoueurs;
}

void changerDeManche(std::array<Joueur, 5>& joueurs, int nbJoueurs, int& mancheCourante, int& joueurDebut) {
    mancheCourante++;
    joueurDebut = 0;
    for (int i = 0; i < nbJoueurs; i++) {
        initGrille(joueurs[i].grilleDeJeu);
    }
}

void ajoutePointsDeGrille(Joueur& j, unsigned int numeroManche){
    j.nbPoints += comptePoints(j.grilleDeJeu, numeroManche);
}