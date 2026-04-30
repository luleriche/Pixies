#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <optional>
#include "Partie.hpp"

int main()
{
    srand(time(nullptr));
    Partie maPartie;
    nouvellePartie(maPartie, 2);
    return 0;
}