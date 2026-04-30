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
        codeCouleur = "\033[31m"; // Couleur du rouge
    else if(c.couleur == 'b')
        codeCouleur = "\033[34m"; // Couleur du bleu
    else if(c.couleur == 'j')
        codeCouleur = "\033[33m"; // Couleur du jaune
    else if(c.couleur == 'v')
        codeCouleur = "\033[32m"; // Couleur du vert
    
    // Si la carte est spéciale
    if(c.couleur == 'n'){
        std::cout << "\033[31m(\033[34m" << c.chiffre << " \033[33m" << c.couleur << " \033[32m";
        if(c.spirale > -1)
            std::cout <<" ";
        std::cout << c.spirale << "\033[31m)" << "\033[0m";
    }
    else{
        std::cout << codeCouleur << "(" << c.chiffre << " " << c.couleur << " ";
        if(c.spirale > -1)
            std::cout <<" ";
        std::cout << c.spirale << ")" << "\033[0m"; 
    }
    
}