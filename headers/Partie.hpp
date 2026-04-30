#pragma once

#include "Defausse.hpp"
#include "Pioche.hpp"
#include "Joueur.hpp"

struct Partie{
    Defausse defausse;
    Pioche pioche;
    std::array<Joueur, 5> joueurs;
    unsigned int nombreJoueurs;
    unsigned int joueurActuel;
    unsigned numeroManche;
};

// Lance une nouvelle partie a partir de zéro 
void lancerUneNouvellePartie();
