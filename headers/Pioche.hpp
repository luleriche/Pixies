#pragma once

#include <array>
#include "Carte.hpp"


// Structure pour la pioche
struct Pioche{
    std::array<Carte*,5> cartes;
    unsigned int taille;
};

void remplirPioche(Pioche &p){
    
}