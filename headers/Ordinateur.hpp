#pragma once

#include <string>
#include <array>

const int NbMaxChoix = 4;

struct ListeDeChoix{
    std::array<std::string, NbMaxChoix> choix;
    unsigned int nbChoix;
};
