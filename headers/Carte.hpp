#pragma once

// Structure pour une carte de jeu.
struct Carte{
    char couleur; // Couleur de la carte r/j/v/b et s pour multicolore
    unsigned int chiffre; // Chiffre sur la carte
    int spirale; // < 0 : nombre de croix, < 7 : nombre de spirales, 9 : carte spéciale
    unsigned int boiteIndice; // Indice de la carte dans la boite
};

// Affiche la carte dans la console style (2 r -9)
void afficher(Carte c);

// Affiche la carte dans la console en couleurs style (2 r -9)
void afficherEnCouleur(Carte c);

// Affiche la carte dans la console en gris style (2 r -9)
void afficherEnGris(Carte c);