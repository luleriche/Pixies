#include <iostream>

#include "Grille.hpp"
#include "Carte.hpp"

// Initialise un emplacement de grille comme étant vide.
void initEmplacement(emplacementGrille & emplGrille){
    emplGrille.faceCachee = nullptr;
    emplGrille.faceVisible = nullptr;
}

// Affiche un emplacement de grille sous la forme : c. cachée / c. visible.
void afficher(emplacementGrille emplGrille){
    if(emplGrille.faceCachee == nullptr)
        std::cout << "     ";
    else
        afficher(*emplGrille.faceCachee);
    std::cout<< " | ";
    if(emplGrille.faceVisible == nullptr)
        std::cout << "     ";
    else
        afficher(*emplGrille.faceVisible);
    std::cout<<std::endl;
}

// Initialise une grille vide
void initGrille(grille & g){
    for( int i = 0; i<9; ++i){
        initEmplacement(g[i]);
    }
}

// Affiche une grille
void afficherGrille(grille & g){
    std::cout << std::endl;
    std::cout << "  |Cachée | Visible" << std::endl;
    std::cout << "-------------------" << std::endl;
    for(int i = 0; i < 9; ++i){
        std::cout<< i+1 << " | ";
        afficher(g[i]);
    }
    std::cout << std::endl;
}

// Ajoute une carte à la grille 
void ajouterCarte(grille& g, carte* c){
    std::cout << "----- Ajout de la carte "; afficher(*c); std::cout << " -----" << std::endl;
    // Si il n'y a pas de carte visible à l'emplacement
    if(g[c->chiffre-1].faceVisible == nullptr){
        std::cout << "Emplacement de la carte disponible. Carte mise face visible." << std::endl;
        g[c->chiffre-1].faceVisible = c;

    // Si il y a une carte visible et pas de cachée
    }else if(g[c->chiffre-1].faceCachee == nullptr){
        unsigned int choix;
        std::cout << "Laissez visible 1 ou 2 ?" << std::endl;
        std::cout << "1 "; afficher(*g[c->chiffre-1].faceVisible);
        std::cout << " / 2 "; afficher(*c); std::cout << std::endl;
        std::cout << "Choix: "; std::cin >> choix;
        if(choix == 1){
            g[c->chiffre-1].faceCachee = c;
        }else{
            g[c->chiffre-1].faceCachee = g[c->chiffre-1].faceVisible;
            g[c->chiffre-1].faceVisible = c;
        }
    }
    // Si l'emplacement est validé (plein)
    else{
        unsigned int choix;
        std::cout << "Emplacement validé ! Où la mettre ? ";
        std::cin >> choix;
        while(g[choix-1].faceCachee != nullptr and g[choix-1].faceVisible){
            std::cout << "Erreur! Emplacement indisponible. Où la mettre ? ";
            std::cin >> choix;
        }
        g[choix-1].faceCachee = c;
    }
}

// Fonction qui regarde si la grille à min une carte par emplacement si oui renvoie true
bool finJeu(grille g){
    for (int i = 0; i < 9; ++i){
        if(g[i].faceCachee == nullptr and g[i].faceVisible == nullptr){
            return false;
        }
    }
    return true;
}