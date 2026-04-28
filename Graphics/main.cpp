#include <iostream>
#include <array>

const unsigned int NMaxCartes = 70;

// Structure pour une carte de jeu
struct carte{
    char couleur;
    unsigned int chiffre; // Chiffre sur la carte
    int spirale; // < 0 : nombre de croix, < 7 : nombre de spirales, 9 : carte spéciale
};

void afficher(carte c){
    std::cout << "Carte : " << c.couleur << c.chiffre << c.spirale << std::endl;
}

// Structure pour la défausse/pile de cartes
struct defausse{
    std::array<carte, NMaxCartes> tasDeCartes;
    unsigned int taille;
};

// Structure pour un emplacement de la grille d'un joueur
struct emplacementGrille{
    carte faceVisible;
    carte faceCachee;
};

// Structure pour la grille d'un joueur
struct grille{
    std::array<std::array<emplacementGrille, 3>, 3> cases;
};



int main()
{
    return 0;
}