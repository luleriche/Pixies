#include <iostream>
#include <string>

#include "Carte.hpp"
#include "Console.hpp"

void afficher(Carte c){
    // Affichage des premières information : (4 r
    std::cout << "(" << c.chiffre << " " << c.couleur << " ";

    // Affichage des spirales, gestion du cas ou c'est une valeur spéciale
    if(c.spirale == 9){
        std::cout <<" +";
    }else{
        if(c.spirale > -1)
            std::cout <<" ";
        std::cout << c.spirale;            
    }

    std::cout << ")"; 
}

void afficherEnCouleur(Carte c){
    std::string codeCouleur;
    if(c.couleur == 'r')
        std::cout << SETCOUL_ROUGE;
    else if(c.couleur == 'b')
        std::cout << SETCOUL_BLEU;
    else if(c.couleur == 'j')
        std::cout << SETCOUL_JAUNE;
    else if(c.couleur == 'v')
        std::cout << SETCOUL_VERT;

    afficher(c);
    std::cout << RESET_COUL;
}

void afficherEnGris(Carte c){
    std::cout << SETCOUL_GRIS;
    afficher(c);
    std::cout << RESET_COUL;
}