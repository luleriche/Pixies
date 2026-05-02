#pragma once

#include <string>
#include "Carte.hpp"

// Définition de la défausse/pile de Cartes.
struct maillon{
    Carte* valeur;
    maillon* suivant;
};
using Defausse = maillon*;

// Définition d'un boite de cartes, qui restera inchangé et qui garde les pointeurs vers les cartes
using BoiteCartes = Carte*;

// Affichr les Cartes d'une défausse en partant de celle du dessus
void afficher(Defausse d);

// Initialise une défausse vide
void initDefausse(Defausse& d);

// Ajoute une carte au début d'une défausse.
void ajoutDebutDefausse(Defausse& defausse, Carte* ptrCarte);

// Ajoute une carte à la fin d'une défausse.
void ajoutFinDefausse(Defausse& d, Carte* c);

// Récupère un pointeur vers la première carte d'une défausse et l'enlève de celle-ci
// Renvoie nullptr si il n'y a pas de carte
Carte* tirerCarteDessus(Defausse& d);

// Récupère un pointeur vers la n-ième carte d'une défausse et l'enlève de celle-ci
Carte* tirerCarteIndice(Defausse& d, int indice);

// Mélange une défausse de manière aléatoire.
void melanger(Defausse& d);

// Mets toutes les cartes d'une boite dans une défausse.
void remplir(BoiteCartes boite, unsigned int nbCartes, Defausse& defausse);

// Remplie une défausse a partir d'un fichier
void creerCartesAvecFichier(std::string nomFic, BoiteCartes& boite, unsigned int &nbCartes);

// Désalloue toute la mémoire prise par les cartes d'une boite
void supprimerBoite(BoiteCartes& boite);