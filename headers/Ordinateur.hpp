#pragma once

#include <string>
#include <array>

#include "Partie.hpp"

// Annuler le dernier coup joué dans une manche
void annulerDernierCoup(Partie& partie);

// Renvoyer une liste de coups contenant tous les coups possibles pour le prochain joueur d'une partie.
ListeDeCoups recupCoupsPossibles(const Partie&);
