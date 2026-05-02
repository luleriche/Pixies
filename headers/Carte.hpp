#pragma once

// Structure pour une carte de jeu.
struct Carte{
    char couleur; 
    unsigned int chiffre; // Chiffre sur la carte
    int spirale; // < 0 : nombre de croix, < 7 : nombre de spirales, 9 : carte spéciale
};

// Affiche la carte dans la console style (2 r -9)
void afficher(Carte c);

// Affiche la carte dans la console en couleurs style (2 r -9)
void afficherEnCouleur(Carte c);

// Affiche la carte dans la console en gris style (2 r -9)
void afficherEnGris(Carte c);