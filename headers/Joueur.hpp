#pragma once

#include <string>

#include "Grille.hpp"
#include "Defausse.hpp"
#include "Pioche.hpp"

// Structure qui représente un joueur
struct Joueur{
    std::string surnom;
    Grille grilleDeJeu;
    Pioche* piocheCommune;
    int nbPoints;
};

// Initialise un joueur 
void initJoueur(Joueur & j, Pioche* p, std::string surnom);

//Creer un nombre de joueur precis 
void creerJoueurs(std::array<Joueur, 5>& joueurs, int nbJoueurs, Pioche* piocheCommune);

// Changement de joueur 
void changerDeJoueur(unsigned int& joueurCourant, unsigned int nbJoueurs);

// Change de joueur apres la fin du tour, le dernier joueur commence
void finDeTour(unsigned int& joueurDebut, unsigned int nbJoueurs);

// Change de manche, donc le premier joueur redeviens le premier
// Et la grille de chaque joueur se vide
void changerDeManche(std::array<Joueur, 5>& joueurs, int nbJoueurs, int& mancheCourante, int& joueurDebut);