#include <iostream>
#include <fstream>
#include "Defausse.hpp"

void afficher(Defausse d){
    if(d != nullptr){
        afficher(*(d->valeur));
        afficher(d->suivant);
    }else{
        std::cout << std::endl;
    }
}

// Initialise une défausse vide
void initDefausse(Defausse& d){
    d = nullptr;
}

// Ajoute une Carte à la fin de la défausse.
void ajoutFinDefausse(Defausse& d, Carte c){
    if(d == nullptr){
        maillon* nouv = new maillon;
        nouv->valeur = new Carte(c);
        nouv->suivant = d;
        d = nouv;
    }else{
        ajoutFinDefausse(d->suivant, c);
    }
}

// Récupère un pointeur vers la première Carte de la défausse et l'enlève de celle-ci
Carte* tirerCarteDessus(Defausse& d){
    if(d != nullptr){
        maillon* premier = d;
        d = d->suivant;
        return premier->valeur;
    }
    else{
        std::cout << "Défausse vide impossible de tirer la première Carte." << std::endl;
        return nullptr;
    }
}

// Récupère un pointeur vers la n-ième Carte de la défausse et l'enlève de celle-ci
Carte* tirerCarteIndice(Defausse& d, int indice){
    if(indice == 0 or d == nullptr)
        return tirerCarteDessus(d);
    else
        return tirerCarteIndice(d->suivant, indice-1);
}

// Mélange une défausse de manière aléatoire.
void melanger(Defausse& d){
    for(int i = 71; i > 0; --i){
        ajoutFinDefausse(d, *tirerCarteIndice(d, rand()%i));
    };
}

// Remplie une défausse a partir d'un fichier
void lireFichierDefausse(std::string nomFic, Defausse& d){
    std::ifstream fic;
    fic.open(nomFic);
    if(fic.is_open()){
        while(fic.good()){
            Carte c;
            fic >> c.chiffre >> c.couleur  >> c.spirale;
            ajoutFinDefausse(d, c);
        }
    }
    else{
        std::cout << "Le fichier des Cartes n'a pas pu être lu.";
    }
}