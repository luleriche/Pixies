#pragma once

#include <string>
#include "Carte.hpp"





// Définition d'un boite de cartes, qui restera inchangé et qui garde les pointeurs vers les cartes.
struct BoiteCartes{
    Carte* cartes;
    unsigned int nbCartes;
};

// Créer les cartes de jeu indiquées dans un fichier et les mettre dans une boîte.
void creerCartesAvecFichier(std::string nomFic, BoiteCartes& boite);

// Désallouer toute la mémoire occupée par les cartes d'une boîte.
void supprimerBoite(BoiteCartes& boite);



// Définition de la défausse(ou pile) de cartes qui est représenté par une liste chainée.
struct maillon{
    Carte* valeur;
    maillon* suivant;
};
using Defausse = maillon*;

// Initialiser une défausse comme vide.
void initDefausse(Defausse& d);

// Remplir une défausse avec toutes les cartes d'une boîte.
void remplir(const BoiteCartes& boite, Defausse& defausse);

// Ajouter une carte au début d'une défausse.
void ajoutDebutDefausse(Defausse& defausse, Carte* ptrCarte);

// Ajouter une carte à la fin d'une défausse.
void ajoutFinDefausse(Defausse& defausse, Carte* ptrCarte);

// Récupérer le nombre de cartes dans une défausse.
unsigned int recupTaille(const Defausse& d);

// Mélanger une défausse de manière aléatoire.
void melanger(Defausse& d);

// Récupérer un pointeur vers la première carte d'une défausse et l'enlèver de celle-ci.
// Renvoie nullptr si il n'y a pas de cartes dans la défausse.
Carte* tirerCarteDessus(Defausse& d);

// Récupérer un pointeur vers la n-ième carte d'une défausse et l'enlèver de celle-ci.
// Renvoie nullptr si il n'y a pas de cartes dans la défausse.
Carte* tirerCarteIndice(Defausse& d, const int indice);

// Récupérer un pointeur vers une carte précise d'une défausse et l'enlèver de celle-ci.
// Renvoie nullptr si il n'y a pas la carte recherchée dans la défausse.
Carte* tirerCartePrecise(Defausse & d, Carte c);

// Afficher toutes les cartes d'une défausse en ligne en partant de celle du dessus.
void afficher(const Defausse d);

// Afficher les n premières cartes d'une défausse en ligne en partant de celle du dessus.
void afficherNCartes(const Defausse d, const unsigned int n);

// Désallouer tous les maillons d'une défausse.
void supprimerDefausse(Defausse& d);






