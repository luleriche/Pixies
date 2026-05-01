#pragma once

#include <string>
#include "Carte.hpp"

// Définition de la défausse/pile de Cartes.
struct maillon{
    Carte* valeur;
    maillon* suivant;
};
using Defausse = maillon*;

// Affichr les Cartes d'une défausse en partant de celle du dessus
void afficher(Defausse d);

// Initialise une défausse vide
void initDefausse(Defausse& d);

// Ajoute une Carte à la fin de la défausse.
void ajoutFinDefausse(Defausse& d, Carte c);

void deplacerDebutDefausse(Defausse& defausse, Carte* ptrCarte);

// Récupère un pointeur vers la première Carte de la défausse et l'enlève de celle-ci
Carte* tirerCarteDessus(Defausse& d);

// Récupère un pointeur vers la n-ième Carte de la défausse et l'enlève de celle-ci
Carte* tirerCarteIndice(Defausse& d, int indice);

// Mélange une défausse de manière aléatoire.
void melanger(Defausse& d);

// Remplie une défausse a partir d'un fichier
void lireFichierDefausse(std::string nomFic, Defausse& d);