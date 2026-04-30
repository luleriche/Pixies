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
    lancerUneNouvellePartie();
    return 0;
}