#pragma once

#include <string>
#include <array>

#include "Partie.hpp"

const int NbMaxCoups = 32;

struct ListeDeCoups{
    std::array<std::string, NbMaxCoups> coups;
    unsigned int nombre;
};

void ajouterCoup(ListeDeCoups& lc, std::string coup);

void afficher(ListeDeCoups lc);

ListeDeCoups recupCoupsPossibles(const Partie&);

void jouerCoup(Partie& partie, std::string coup);
