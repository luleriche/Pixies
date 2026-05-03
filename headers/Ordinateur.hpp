#pragma once

#include <string>
#include <array>

#include "Partie.hpp"

const int NbMaxChoix = 32;

struct ListeDeChoix{
    std::array<std::string, NbMaxChoix> choix;
    unsigned int nbChoix;
};

void ajouterChoix(ListeDeChoix& listeChoix, std::string choix);

void afficher(ListeDeChoix choix);

ListeDeChoix recupChoix(const Partie&);

void jouerCoup(Partie& partie, std::string coup);
