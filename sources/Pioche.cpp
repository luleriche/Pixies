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

void mettrePiocheDansDefausse(Pioche &p, Defausse& d){
    for(int i = p.taille-1; i >= 0; --i){
        if(p.cartes[i] != nullptr){
            ajoutDebutDefausse(d, p.cartes[i]);
            p.cartes[i] = nullptr;
        }
    }
    p.nombreCartesRestantes = 0;
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

unsigned int indiceCartePioche(Pioche p, Carte c){
    for(unsigned int indice = 0; indice < p.taille; ++indice){
        if (p.cartes[indice] != nullptr and c.couleur == p.cartes[indice]->couleur and c.chiffre == p.cartes[indice]->chiffre and c.spirale == p.cartes[indice]->spirale){
            return indice;
        }
    }
    std::cout << "La carte "; afficherEnCouleur(c); std::cout << " n'est pas dans la pioche." << std::endl;
    return -1;
}