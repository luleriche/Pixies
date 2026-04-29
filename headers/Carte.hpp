#pragma once

// Structure pour une carte de jeu.
struct carte{
    char couleur; 
    unsigned int chiffre; // Chiffre sur la carte
    int spirale; // < 0 : nombre de croix, < 7 : nombre de spirales, 9 : carte spéciale
};

void afficher(carte c);