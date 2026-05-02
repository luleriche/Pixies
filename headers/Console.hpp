#pragma once

#include <string>

// FONCTIONS ET CONSTANTES UTILES POUR L'AFFICHAGE DANS LA CONSOLE

const std::string SETCOUL_ROUGE = "\033[31m";
const std::string SETCOUL_VERT = "\033[32m";
const std::string SETCOUL_BLEU = "\033[34m";
const std::string SETCOUL_JAUNE = "\033[33m";
const std::string SETCOUL_GRIS = "\033[90m";
const std::string SETCOUL_MAGENTA = "\033[35m";
const std::string CLIGNOTE = "\033[5;31m";
const std::string GRAS = "\033[1;31m";

const std::string RESET_COUL = "\033[0m";


void effaceConsole();

