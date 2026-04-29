#include <iostream>
#include <fstream>
#include "Defausse.hpp"

void afficher(defausse d){
    if(d != nullptr){
        afficher(*(d->valeur));
        afficher(d->suivant);
    }else{
        std::cout << std::endl;
    }
}

// Initialise une défausse vide
void initDefausse(defausse& d){
    d = nullptr;
}

// Ajoute une carte à la fin de la défausse.
void ajoutFinDefausse(defausse& d, carte c){
    if(d == nullptr){
        maillon* nouv = new maillon;
        nouv->valeur = new carte(c);
        nouv->suivant = d;
        d = nouv;
    }else{
        ajoutFinDefausse(d->suivant, c);
    }
}

// Récupère un pointeur vers la première carte de la défausse et l'enlève de celle-ci
carte* tirerCarteDessus(defausse& d){
    if(d != nullptr){
        maillon* premier = d;
        d = d->suivant;
        return premier->valeur;
    }
    else{
        std::cout << "Défausse vide impossible de tirer la première carte." << std::endl;
        return nullptr;
    }
}

// Récupère un pointeur vers la n-ième carte de la défausse et l'enlève de celle-ci
carte* tirerCarteIndice(defausse& d, int indice){
    if(indice == 0 or d == nullptr)
        return tirerCarteDessus(d);
    else
        return tirerCarteIndice(d->suivant, indice-1);
}

// Mélange une défausse de manière aléatoire.
void melanger(defausse& d){
    for(int i = 71; i > 0; --i){
        ajoutFinDefausse(d, *tirerCarteIndice(d, rand()%i));
    };
}

// Remplie une défausse a partir d'un fichier
void lireFichierDefausse(std::string nomFic, defausse& d){
    std::ifstream fic;
    fic.open(nomFic);
    if(fic.is_open()){
        while(fic.good()){
            carte c;
            fic >> c.chiffre >> c.couleur  >> c.spirale;
            ajoutFinDefausse(d, c);
        }
    }
    else{
        std::cout << "Le fichier des cartes n'a pas pu être lu.";
    }
}