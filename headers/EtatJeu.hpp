#pragma once

#include <string>
#include "Partie.hpp"

// Ouvre le fichier et écrit le numéro de manche
void ouvrirFichierIA(const std::string& nomFichier, const Partie& partie);

// Ecrit le numéro de manche et les scores sur des lignes séparées
void ecrireDebutManche(const Partie& partie);

// Ecrit les cartes disponibles dans la pioche
void ecrireCartesDisponibles(const Partie& partie);

// Ecrit un coup joué dans le fichier
void ecrireCoupJoue(const Partie& partie, const std::string& coup);

// Ferme le fichier
void fermerFichierIA();

// Lit un fichier d'état, reconstruit la partie, calcule le meilleur coup et l'écrit dans le fichier
void jouerCoupDepuisFichier(const std::string& nomFichier);