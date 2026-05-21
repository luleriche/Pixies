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
    bool estOrdi;
};

// Initialise un joueur 
void initJoueur(Joueur & j, Pioche* p, const std::string surnom, bool estOrdi = false);

//Créer un nombre de joueur precis 
void creerJoueurs(std::array<Joueur, 5>& joueurs, const int nbJoueurs, Pioche* piocheCommune);

// Ajoute les points de la grille d'un joueur à celui-ci
void ajoutePointsDeGrille(Joueur& j, const unsigned int numeroManche);