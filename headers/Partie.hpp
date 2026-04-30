#pragma once

#include "Defausse.hpp"
#include "Pioche.hpp"
#include "Joueur.hpp"

struct Partie{
    unsigned numeroManche;
    Defausse defausse;
    Pioche pioche;
    std::array<Joueur, 5> joueurs;
    unsigned int nombreJoueurs;
    unsigned int joueurActuel;
};

void nouvellePartie(Partie& p, unsigned int nbJoueurs);
