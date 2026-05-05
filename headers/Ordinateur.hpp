#pragma once

#include <string>
#include <array>

#include "Partie.hpp"

// Annuler le dernier coup joué dans une manche
void annulerDernierCoup(Partie& partie);

// Renvoyer une liste de coups contenant tous les coups possibles pour le prochain joueur d'une partie.
ListeDeCoups recupCoupsPossibles(const Partie& partie);

// Fais jouer un coup aléatoire pour le prochain joueur
void jouerCoupAlea(Partie& partie);

// Finir la manche actuelle avec uniquement des coups aléatoire
void finirMancheAleatoirement(Partie& partie);

// Récupérer le joueur en tête si l'on simule une fin de manche aléatoire
unsigned int recupLeaderFinMancheAleatoire(Partie& partie);
