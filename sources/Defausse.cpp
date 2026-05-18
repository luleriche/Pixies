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

void remplir(BoiteCartes boite, unsigned int nbCartes, Defausse& defausse){
    for(unsigned int i = 0; i < nbCartes; ++i){
        ajoutFinDefausse(defausse, &boite[i]);
    }
}

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

void supprimerBoite(BoiteCartes& boite){
    delete[] boite;
    boite = nullptr;
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

Carte* TirerCartePrecise(Defausse & d, Carte c){
    if (d = nullptr){
        return nullptr;
    }
    if((d->valeur->chiffre == c.chiffre)  and (d->valeur->couleur == c.couleur) and (d->valeur->spirale == c.spirale)){
        return tirerCarteDessus(d);
    }
    maillon* courant = d;

    while(courant->suivant != nullptr){
        if((courant->valeur->chiffre == c.chiffre)  and (courant->valeur->couleur == c.couleur) and (courant->valeur->spirale == c.spirale)){
            maillon* tmp = courant->suivant;
            Carte* carte = tmp->valeur;
            courant->suivant = tmp->suivant;
            delete tmp;
            return carte;
        }
        courant = courant->suivant;
    }
}