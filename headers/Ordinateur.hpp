#pragma once

#include <string>
#include <array>

#include "Partie.hpp"

// Renvoie une liste de coups contenant tous les coups possibles pour le prochain joueur d'une partie.
ListeDeCoups recupCoupsPossibles(const Partie&);
