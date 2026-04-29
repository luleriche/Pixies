#include <iostream>
#include "Carte.hpp"

void afficher(carte c){
    std::cout << "(" << c.chiffre << " " << c.couleur << " " << c.spirale << ")";
}