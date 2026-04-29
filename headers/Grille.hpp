#pragma once

#include <array>
#include "Carte.hpp"

// Structure pour un emplacement de la Grille d'un joueur.
struct emplacementGrille{
    Carte* faceVisible;
    Carte* faceCachee;
};

// Initialise un emplacement de Grille comme étant vide.
void initEmplacement(emplacementGrille & emplGrille);

// Affiche un emplacement de Grille sous la forme : c. cachée / c. visible.
void afficher(emplacementGrille emplGrille);

// Définition de la Grille d'un joueur
using Grille = std::array<emplacementGrille, 9>;

// Initialise une Grille vide
void initGrille(Grille & g);

// Affiche une Grille
void afficherGrille(Grille & g);

// Ajoute une Carte à la Grille 
void ajouterCarte(Grille& g, Carte* c);

// Fonction qui regarde si la Grille à min une Carte par emplacement si oui renvoie true
bool finJeu(Grille g);