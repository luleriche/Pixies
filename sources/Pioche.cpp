#include <iostream>
#include "Pioche.hpp"
#include "Defausse.hpp"
#include "Carte.hpp"
#include "Grille.hpp"

void initPioche(Pioche& p, unsigned int taille){
    p.taille = taille;
    p.nombreCartesRestantes = 0;
    for(unsigned int i = 0; i < 5; ++i){
        p.cartes[i] = nullptr;
    }
}

void afficher(Pioche p){
    for (unsigned int i = 0; i < p.taille; ++i){
        std::cout << " " << i+1 << "   ";
        if(p.cartes[i] != nullptr)
            afficherEnCouleur(*p.cartes[i]);
        std::cout << std::endl;
    }
}

void remplirPioche(Pioche &p, Defausse& d){
    for (unsigned int i = 0; i < p.taille; ++i){
        p.cartes[i] = tirerCarteDessus(d);
    }
    p.nombreCartesRestantes = p.taille;
}

bool estVidePioche(Pioche p){
    return p.nombreCartesRestantes == 0;
}

bool estPleinePioche(Pioche p){
    return p.nombreCartesRestantes == p.taille;
}

Carte* tierCartePioche(Pioche& p, unsigned int indice){
    Carte* carteTiree = p.cartes[indice];
    p.cartes[indice] = nullptr;
    --p.nombreCartesRestantes;
    return carteTiree;
}