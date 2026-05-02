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

// Ajoute une carte au début d'une défausse.
void ajoutDebutDefausse(Defausse& d, Carte* c){
    if(c != nullptr){
        maillon* nouv = new maillon;
        nouv->suivant = d;
        nouv->valeur = c;
        d = nouv;
    }
}

// Ajoute une carte à la fin d'une défausse.
void ajoutFinDefausse(Defausse& d, Carte* c){
    if(d == nullptr){
        maillon* nouv = new maillon;
        nouv->valeur = c;
        nouv->suivant = d;
        d = nouv;
    }else if(c != nullptr){
        ajoutFinDefausse(d->suivant, c);
    }
}

// Récupère un pointeur vers la première carte d'une défausse et l'enlève de celle-ci
// Renvoie nullptr si il n'y a pas de carte
Carte* tirerCarteDessus(Defausse& d){
    if(d != nullptr){
        maillon* premierMaillon = d;
        Carte* premiereCarte = premierMaillon->valeur;
        d = d->suivant;
        delete premierMaillon;
        return premiereCarte;
    }
    else{
        std::cout << "Défausse vide. Le pointeur retourné est nullptr." << std::endl;
        return nullptr;
    }
}

// Récupère un pointeur vers la n-ième carte d'une défausse et l'enlève de celle-ci
Carte* tirerCarteIndice(Defausse& d, int indice){
    if(indice == 0 or d == nullptr)
        return tirerCarteDessus(d);
    else
        return tirerCarteIndice(d->suivant, indice-1);
}

// Mélange une défausse de manière aléatoire.
void melanger(Defausse& d){
    for(int i = 71; i > 0; --i){
        ajoutFinDefausse(d, tirerCarteIndice(d, rand()%i));
    };
}

// Mets toutes les cartes d'une boite dans une défausse
void remplir(BoiteCartes boite, unsigned int nbCartes, Defausse& defausse){
    for(unsigned int i = 0; i < nbCartes; ++i){
        ajoutFinDefausse(defausse, &boite[i]);
    }
}

// Créer les emplacements pour les cartes dans la mémoire à partir d'un fichier texte
// Et mets les pointeurs vers ces emplacements dans la boite
void creerCartesAvecFichier(std::string nomFic, BoiteCartes& boite, unsigned int &nbCartes){
    std::ifstream fic;
    fic.open(nomFic);
    if(fic.is_open()){
        fic >> nbCartes;
        boite = new Carte[nbCartes];
        for(unsigned int i = 0; i < nbCartes; ++i){
            fic >> boite[i].chiffre >> boite[i].couleur  >> boite[i].spirale;
        }
        std::cout << "Toutes les cartes du fichier ont bien été lues." << std::endl;
    }
    else{
        std::cout << "Le fichier des Cartes n'a pas pu être lu." << std::endl;
    }
}

// Désalloue toute la mémoire prise par une boite
void supprimerBoite(BoiteCartes& boite){
    delete[] boite;
    boite = nullptr;
}