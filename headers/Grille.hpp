#pragma once

#include <array>
#include "Carte.hpp"

// Structure pour un emplacement de la grille d'un joueur.
struct emplacementGrille{
    carte* faceVisible;
    carte* faceCachee;
};

// Initialise un emplacement de grille comme étant vide.
void initEmplacement(emplacementGrille & emplGrille);

// Affiche un emplacement de grille sous la forme : c. cachée / c. visible.
void afficher(emplacementGrille emplGrille);

// Définition de la grille d'un joueur
using grille = std::array<emplacementGrille, 9>;

// Initialise une grille vide
void initGrille(grille & g);

// Affiche une grille
void afficherGrille(grille & g);

// Ajoute une carte à la grille 
void ajouterCarte(grille& g, carte* c);

// Fonction qui regarde si la grille à min une carte par emplacement si oui renvoie true
bool finJeu(grille g);