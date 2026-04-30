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

// Rempli la pioche de 3 à 5 cartes en focntion du nombre de joueur
void remplirPioche(Pioche &p, Defausse& d, unsigned int nb_joueur);

//Renvoie vrai si la pioche est vide et faux sinon
bool estVidePioche(Pioche p);

// Supprime une carte d'indice i de la pioche 
void supprimerCartePioche(Pioche & p, unsigned int i);

// Ajoute une carte dans une grille g de la pioche p
Carte* prendrePioche(Pioche& p, unsigned int taillePioche);