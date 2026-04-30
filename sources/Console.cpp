#include <iostream>
#include <string>

#include "Console.hpp"

void effaceConsole() {
    // \033[2J vide la console, \033[1;1H replace le curseur en haut à gauche
    std::cout << "\033[2J\033[1;1H";
}