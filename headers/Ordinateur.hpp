#pragma once

#include <string>
#include <array>

#include "Partie.hpp"

const int NbMaxChoix = 4;

struct ListeDeChoix{
    std::array<std::string, NbMaxChoix> choix;
    unsigned int nbChoix;
};

ListeDeChoix recupChoix(Partie p);
