#pragma once

#include <string>

#include "Grille.hpp"
#include "Defausse.hpp"
#include "Pioche.hpp"

// Structure qui représente un joueur
struct Joueur{
    std::string surnom;
    Grille grilleDeJeu;
    Pioche* piocheCommune;
    int nbPoints;
};


