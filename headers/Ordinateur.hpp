#pragma once

#include <string>
#include <array>

#include "Partie.hpp"

const int NbMaxCoups = 32;

struct ListeDeCoups{
    std::array<std::string, NbMaxCoups> coups;
    unsigned int nombre;
};

// Ajoute un coup à une liste de coups.
void ajouterCoup(ListeDeCoups& lc, std::string coup);

// Affiche une liste de coup, une ligne par coup.
void afficher(ListeDeCoups lc);

// Renvoie une liste de coups contenant tous les coups possibles pour le prochain joueur d'une partie.
ListeDeCoups recupCoupsPossibles(const Partie&);

// Change l'état d'une partie comme si quelqu'un jouait un coup donné en paramètre.
void jouerCoup(Partie& partie, std::string coup);

// Change l'état d'une partie comme si on annulait un coup passé en paramètre.
void annulerCoup(Partie& partie, std::string coup);
