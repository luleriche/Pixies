#include <iostream>
#include <string>
#include "Carte.hpp"

void afficher(Carte c){
    std::cout << "(" << c.chiffre << " " << c.couleur << " ";
    if(c.spirale > -1)
        std::cout <<" " << c.spirale << ")";
    else
        std::cout << c.spirale << ")";
}

void afficherEnCouleur(Carte c){
    // \033[0m reset la couleur
    std::string codeCouleur;
    if(c.couleur == 'r')
        codeCouleur = "\033[31m";
    else if(c.couleur == 'b')
        codeCouleur = "\033[34m";
    else if(c.couleur == 'j')
        codeCouleur = "\033[33m";
    else if(c.couleur == 'v')
        codeCouleur = "\033[32m";
    else if(c.couleur == 'n')
        codeCouleur = "";
    
    std::cout << codeCouleur << "(" << c.chiffre << " " << c.couleur << " ";
    if(c.spirale > -1)
        std::cout <<" " << c.spirale << ")" << "\033[0m";
    else
        std::cout << c.spirale << ")" << "\033[0m";
}