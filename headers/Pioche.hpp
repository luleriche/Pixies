#pragma once

#include <iostream>
#include <array>
#include "Carte.hpp"
#include "Defausse.hpp"
#include "Grille.hpp"

// Structure pour la pioche
struct Pioche{
    std::array<Carte*,5> cartes;
    unsigned int taille;
};

// Initialise une pioche par nullptr
void initPioche(Pioche& p);

// Affiche les cartes de la pioche 
void afficher(Pioche p);

// Fonction qui remplit une pioche en prenant les cartes au dessus de la défausse
void remplirPioche(Pioche &p, Defausse& d);

//Renvoie vrai si une pioche est vide et faux sinon
bool estVidePioche(Pioche p);

//Renvoie vrai si une pioche est vide et faux sinon
bool estPleinePioche(Pioche p);

// Ajoute une carte dans une grille g de la pioche p
Carte* prendrePioche(Pioche& p);