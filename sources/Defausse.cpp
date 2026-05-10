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

void initDefausse(Defausse& d){
    d = nullptr;
}

void viderDefausse(Defausse& d){
    if(d != nullptr){
        viderDefausse(d->suivant);
        delete d;
        d = nullptr;
    }
}

void ajoutDebutDefausse(Defausse& d, Carte* c){
    if(c != nullptr){
        maillon* nouv = new maillon;
        nouv->suivant = d;
        nouv->valeur = c;
        d = nouv;
    }
}

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

Carte* tirerCarteIndice(Defausse& d, int indice){
    if(indice == 0 or d == nullptr)
        return tirerCarteDessus(d);
    else
        return tirerCarteIndice(d->suivant, indice-1);
}

void melanger(Defausse& d){
    unsigned int taille = recupTaille(d);
    for(int i = taille; i > 0; --i){
        ajoutFinDefausse(d, tirerCarteIndice(d, rand()%i));
    };
}

void remplir(BoiteCartes boite, Defausse& defausse){
    for(unsigned int i = 0; i < boite.nbCartes; ++i){
        ajoutFinDefausse(defausse, &boite.cartes[i]);
    }
}

void creerCartesAvecFichier(std::string nomFic, BoiteCartes& boite){
    std::ifstream fic;
    fic.open(nomFic);
    // Si le fichier est bien ouvert
    if(fic.is_open()){
        // On alloue assez de mémoire pour les cartes dont le nombre est marqué en haut
        fic >> boite.nbCartes;
        boite.cartes = new Carte[boite.nbCartes];
        // Pour chaque carte
        for(unsigned int i = 0; i < boite.nbCartes; ++i){
            // On lit ses infos
            fic >> boite.cartes[i].chiffre >> boite.cartes[i].couleur  >> boite.cartes[i].spirale;
            // Et on met son indice dans la boite
            boite.cartes[i].boiteIndice = i;
        }
        std::cout << "Toutes les cartes du fichier ont bien été lues." << std::endl;
    }
    else{
        std::cout << "Le fichier des Cartes n'a pas pu être lu." << std::endl;
    }
}

void supprimerBoite(BoiteCartes& boite){
    delete[] boite.cartes;
    boite.cartes = nullptr;
    boite.nbCartes = 0;
}

unsigned int recupTaille(Defausse d){
    maillon* tmp = d;
    unsigned int taille = 0;
    while(tmp != nullptr){
        ++taille;
        tmp = tmp->suivant;
    }
    return taille;
}

void afficherNCartes(const Defausse d, unsigned int n){
    if( d!= nullptr and n != 0){
        afficherEnCouleur(*(d->valeur));
        afficherNCartes(d->suivant, n-1);
    }
}