#pragma once

#include <string>
#include "Carte.hpp"

// Définition de la défausse/pile de cartes.
struct maillon{
    carte* valeur;
    maillon* suivant;
};
using defausse = maillon*;

// Affichr les cartes d'une défausse en partant de celle du dessus
void afficher(defausse d);

// Initialise une défausse vide
void initDefausse(defausse& d);

// Ajoute une carte à la fin de la défausse.
void ajoutFinDefausse(defausse& d, carte c);

// Récupère un pointeur vers la première carte de la défausse et l'enlève de celle-ci
carte* tirerCarteDessus(defausse& d);

// Récupère un pointeur vers la n-ième carte de la défausse et l'enlève de celle-ci
carte* tirerCarteIndice(defausse& d, int indice);

// Mélange une défausse de manière aléatoire.
void melanger(defausse& d);

// Remplie une défausse a partir d'un fichier
void lireFichierDefausse(std::string nomFic, defausse& d);