#include <iostream>
#include "Pioche.hpp"
#include "Defausse.hpp"
#include "Carte.hpp"
#include "Grille.hpp"

void initPioche(Pioche& p){
    for (unsigned int i = 0; i<5; ++i){
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
}

bool estVidePioche(Pioche p){
    unsigned int i = 0;
    while(i < p.taille and p.cartes[i] == nullptr){
        ++i;
    }
    return i == p.taille;
}

void supprimerCartePioche(Pioche & p, unsigned int i){
    p.cartes[i] = nullptr;
}

Carte* prendrePioche(Pioche& p, unsigned int taillePioche){
    unsigned int choix;
    std::cout << "Pioche :" << std::endl;
    afficher(p);
    std::cout << "Votre choix : ";
    std::cin >> choix;
    while (choix > taillePioche or choix < 0 or p.cartes[choix - 1] == nullptr){
        std::cout<< "Erreur! Il n'y a pas de carte ici, réessayer : ";
        std::cin>> choix;
    }
    Carte* temp = p.cartes[choix - 1];
    supprimerCartePioche(p, choix-1);
    return temp;
}