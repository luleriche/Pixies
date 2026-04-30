#include <iostream>
#include <string>

#include "Carte.hpp"
#include "Console.hpp"

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
        codeCouleur = SETCOUL_ROUGE; // Couleur du rouge
    else if(c.couleur == 'b')
        codeCouleur = SETCOUL_BLEU; // Couleur du bleu
    else if(c.couleur == 'j')
        codeCouleur = SETCOUL_JAUNE; // Couleur du jaune
    else if(c.couleur == 'v')
        codeCouleur = SETCOUL_VERT; // Couleur du vert
    else
        codeCouleur = "";
    
    std::cout << codeCouleur << "(" << c.chiffre << " " << c.couleur << " ";
    if(c.spirale > -1)
        std::cout <<" ";
    std::cout << c.spirale << ")" << RESET_COUL; 
}