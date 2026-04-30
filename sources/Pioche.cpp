#include <iostream>
#include "Pioche.hpp"
#include "Defausse.hpp"
#include "Carte.hpp"
#include "Grille.hpp"

void initPioche(Pioche& p){
    for (int i = 0; i<5; ++i){
        p.cartes[i] = nullptr; 
    }
}

void afficher(Pioche p){
    for (int i = 0; i<5; ++i){
        if (p.cartes[i] != nullptr){
            afficher(*p.cartes[i]);
            std::cout<< " | ";
        }
    }
    std::cout<<std::endl;
}

void remplirPioche(Pioche &p, Defausse& d, unsigned int nb_joueur){
    if (nb_joueur == 2 or nb_joueur == 4){
        for (int i = 0; i< 4; ++i){
            p.cartes[i] = tirerCarteDessus(d);
        }
    }
    else if (nb_joueur == 3){
        for (int i = 0; i< 3; ++i){
            p.cartes[i] = tirerCarteDessus(d);
        }
    }
    else{
        for (int i = 0; i< 5; ++i){
            p.cartes[i] = tirerCarteDessus(d);
        }
    }
}

bool estVidePioche(Pioche p){
    unsigned i = 0;
    while(i < 5){
        if (p.cartes[i] != nullptr){
            return false;
        }
        else{
            i++;
        }
    }
    return true;
}

void supprimerCartePioche(Pioche & p, unsigned int i){
    p.cartes[i] = nullptr;
}

Carte* prendrePioche(Pioche& p, Grille& g){
    unsigned int choix;
    std::cout<<"Piocher une carte :"<< std::endl;
    for (int i = 0; i<5; ++i){
        if (p.cartes[i] != nullptr){
            std::cout<< i +1; afficher(*p.cartes[i]); std::cout<<" / ";
        }
    }
    std::cin >> choix;
    if (p.cartes[choix-1] == nullptr){
        while (p.cartes[choix - 1] == nullptr){
            std::cout<< "Erreur! La carte n'existe pas, veuillez choisir une autre carte" << std::endl;
            std::cin>> choix;
        }
    }
    Carte* temp = p.cartes[choix - 1];
    supprimerCartePioche(p, choix-1);
    return p.cartes[choix -1];
}