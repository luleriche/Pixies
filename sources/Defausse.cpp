#include <iostream>
#include <fstream>
#include "Defausse.hpp"

void creerCartesAvecFichier(const std::string nomFic, BoiteCartes& boite){
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


void initDefausse(Defausse& d){
    d = nullptr; // Une défausse initialisée ne pointe vers aucun maillon
}

void remplir(const BoiteCartes& boite, Defausse& defausse){
    // On parcourt les cartes de la boîtes
    for(unsigned int i = 0; i < boite.nbCartes; ++i){
        ajoutFinDefausse(defausse, &boite.cartes[i]); // On les ajoute chacunes à la fin de la défausse
    }
}

void ajoutDebutDefausse(Defausse& d, Carte* ptrCarte){
    // Si le pointeur n'est pas nul
    if(ptrCarte != nullptr){
        maillon* nouv = new maillon; // On crée un nouveau maillon que l'on ajoute au début
        nouv->suivant = d;
        nouv->valeur = ptrCarte; // Dont la valeur est le pointeur vers la carte voulue
        d = nouv;
    }
}

void ajoutFinDefausse(Defausse& d, Carte* ptrCarte){
    // Si la défausse est vide
    if(d == nullptr) 
        ajoutDebutDefausse(d, ptrCarte); // Ajouter à la fin revient à ajouter au début
    // Sinon si le pointeur n'est pas nul
    else if(ptrCarte != nullptr) 
        ajoutFinDefausse(d->suivant, ptrCarte); // On ajoute à la fin de la défausse de manière récursive
}

unsigned int recupTaille(const Defausse& d){
    maillon* tmp = d; // On met un pouinteur vers le début de la défausse
    unsigned int taille = 0;
    while(tmp != nullptr){ // Tant que ce pointeur n'est pas nul
        ++taille; 
        tmp = tmp->suivant; // On le fait pointer vers le suite de la défausse et on ajoute 1 au nombre de cartes.
    }
    return taille;
}

void melanger(Defausse& d){
    unsigned int taille = recupTaille(d); // On récupère la taille de la défausse
    for(int i = taille; i > 0; --i){
        ajoutFinDefausse(d, tirerCarteIndice(d, rand()%i)); // On prend un carte parmi celle non mélangé, et on la met à la fin.
    };
}

Carte* tirerCarteDessus(Defausse& d){
    // Si la défausse n'est pas vide
    if(d != nullptr){
        maillon* premierMaillon = d;
        Carte* premiereCarte = premierMaillon->valeur; // On stock le pointeur vers la carte du premier maillon
        d = d->suivant; // On fait commencer la défausse au second maillon
        delete premierMaillon; // On désalloue le premier maillon
        return premiereCarte;
    }
    // Sinon alors il n'y a pas de première carte
    else{
        std::cout << "Défausse vide. Le pointeur retourné est nullptr." << std::endl;
        return nullptr;
    }
}

Carte* tirerCarteIndice(Defausse& d, const int indice){
    // Si on veut récupérer la première carte ou si la défausse est vide
    if(indice == 0 or d == nullptr)
        return tirerCarteDessus(d); // Rappel : tirer la première carte d'une défausse vide retourne nullptr, ce qui nous va
    // Sinon on va plus loin dans la défausse de manière récursive
    else
        return tirerCarteIndice(d->suivant, indice-1);
}

Carte* tirerCartePrecise(Defausse & d, Carte carte){
    // Si la défausse est vide, la carte demandé n'est forcément pas dedans.
    if (d == nullptr){
        std::cout << "La carte "; afficherEnCouleur(carte); std::cout << " n'a pas été trouvée dans la défausse." << std::endl;
        return nullptr;
    }
    // Sinon on regarde si la première carte est celle demandée
    else if(d->valeur->chiffre == carte.chiffre and d->valeur->couleur == carte.couleur and d->valeur->spirale == carte.spirale)
        return tirerCarteDessus(d);
    // Sinon on va plus loin dans la défausse de manière récursive
    else
        return tirerCartePrecise(d->suivant, carte);
}

void afficher(const Defausse d){
    // Si la défausse n'est pas vide
    if(d != nullptr){
        afficher(*(d->valeur));// On affiche la première carte
        afficher(d->suivant); // Puis le reste de la défausse
    }else{
        std::cout << std::endl; // Une fois arrivé à la fin on saute une ligne
    }
}

void afficherNCartes(const Defausse d, const unsigned int n){
    // Si la défausse n'est pas vide et que l'on afficher au moins une carte
    if(d != nullptr and n != 0){
        afficherEnCouleur(*(d->valeur)); // On affiche la première carte
        afficherNCartes(d->suivant, n-1); // On va plus loin dans la défausse de manière récursive
    }
    else
        std::cout << std::endl; // une fois à la fin on saute une ligne
}

void supprimerDefausse(Defausse& d){
    if(d != nullptr){
        supprimerDefausse(d->suivant); // On supprime d'abord la suite de la défausse
        delete d; // On désalloue le premier maillon
        d = nullptr; // Pour réinitialiser la défausse
    }
}
