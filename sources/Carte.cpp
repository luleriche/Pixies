#include <iostream>
#include "Carte.hpp"

void afficher(Carte c){
    std::cout << "(" << c.chiffre << " " << c.couleur << " " << c.spirale << ")";
}