#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <optional>
#include "Partie.hpp"
#include "Graphics.hpp"

int main()
{
    srand(time(nullptr));
    lancerJeu();
    return 0;
}