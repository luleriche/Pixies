#pragma once

#include <iostream>
#include <array>
#include "Carte.hpp"
#include "Defausse.hpp"
#include "Grille.hpp"

// Structure pour une pioche.
struct Pioche{
    std::array<Carte*,5> cartes;
    unsigned int taille;
    unsigned int nombreCartesRestantes;
};

// Initialise une pioche avec que des emplacements vide et initialise également sa taille.
void initPioche(Pioche& p, unsigned int taille);

// Affiche les cartes de la pioche avec autant de lignes qu'il y a d'emplacements dans la pioche.
void afficher(Pioche p);

// Remplir une pioche en prenant les premières cartes d'une défausse.
void remplirPioche(Pioche &p, Defausse& d);

// Mettre toutes les cartes d'une pioche dans une défausse en commençant par les dernières.
void mettrePiocheDansDefausse(Pioche &p, Defausse& d);

// Savoir si la pioche est vide ou non.
bool estVidePioche(Pioche p);

// Savoir si la pioche est pleine ou non.
bool estPleinePioche(Pioche p);

// Récupérer un pointeur vers une carte d'une pioche et l'enlevée de celle-ci.
Carte* tierCartePioche(Pioche& p, unsigned int indice);

// Récupère l'indice d'une carte précise dans la pioche
unsigned int indiceCartePioche(Pioche p, Carte c);