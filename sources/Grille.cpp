#include <iostream>

#include "Grille.hpp"
#include "Carte.hpp"

// Initialise un emplacement de Grille comme étant vide.
void initEmplacement(emplacementGrille & emplGrille){
    emplGrille.faceCachee = nullptr;
    emplGrille.faceVisible = nullptr;
}

// Affiche un emplacement de Grille sous la forme : c. cachée / c. visible.
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

// Initialise une Grille vide
void initGrille(Grille & g){
    for( int i = 0; i<9; ++i){
        initEmplacement(g[i]);
    }
}

// Affiche une Grille
void afficherGrille(Grille & g){

    // \033[90m : Gris foncé
    // \033[0m  : Réinitialise la couleur

    std::cout << std::endl;
    std::cout << "------------------------------------" << std::endl;

    for(int ligne = 0; ligne < 3; ++ligne)
    {
        for(int col = 0; col < 3; ++col){
            if(col == 0)
                std::cout << " |";
            std::cout << ligne*3 + col + 1;
            if(g[ligne*3+col].faceVisible == nullptr)
                std::cout <<"         |";
            else{
                afficherEnCouleur(*g[ligne*3+col].faceVisible); std::cout << " |";
            }
        }
        std::cout << std::endl;
        for(int col = 0; col < 3; ++col){
            if(col == 0)
                std::cout << " | ";
            if(g[ligne*3+col].faceCachee == nullptr)
                std::cout <<"         | ";
            else{
                std::cout << "\033[90m";afficher(*g[ligne*3+col].faceCachee); std::cout << "\033[0m | ";
            }
        }
        std::cout << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
    
    
    std::cout << std::endl;
}

// Ajoute une Carte à la Grille 
void ajouterCarte(Grille& g, Carte* c){
    std::cout << "----- Ajout de la Carte "; afficher(*c); std::cout << " -----" << std::endl;
    // Si il n'y a pas de Carte visible à l'emplacement
    if(g[c->chiffre-1].faceVisible == nullptr){
        std::cout << "Emplacement de la Carte disponible. Carte mise face visible." << std::endl;
        g[c->chiffre-1].faceVisible = c;

    // Si il y a une Carte visible et pas de cachée
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

// Fonction qui regarde si la Grille à min une Carte par emplacement si oui renvoie true
bool finJeu(Grille g){
    for (int i = 0; i < 9; ++i){
        if(g[i].faceCachee == nullptr and g[i].faceVisible == nullptr){
            return false;
        }
    }
    return true;
}

// Retourne le nombre de points sur la grille actuelle selon la manche
int comptePoints(Grille g, unsigned int numeroManche){
    int points = 0;

    // Addition des spriales et des croix
    points += compteSpirales(g);
    // Addition des points obtenus grâce aux cartes validés
    points += comptePointsAvecValidation(g);
    // Addition des points obtenus grâce à la plus grande zone de même couleur
    points += comptePointsZone(g, numeroManche);

    return points;
}

// Retourne le nombre de points sur une grille obtenus avec les spirales et les croix
int compteSpirales(Grille g){
    int points = 0;
    for(int i = 0; i < 9; ++i){
        // Si il y a une carte face visible
        if(g[i].faceVisible != nullptr){
            int nbSpirales = g[i].faceVisible->spirale;
            if(nbSpirales != 9)
                points += nbSpirales;
            else
                points += compteCarteCouleur(g, g[i].faceVisible->couleur);
        }
    }
    return points;
}

unsigned int compteCarteCouleur(Grille g, char couleur){
    unsigned int nombre = 0;
    for(int i = 0; i<9; ++i){
        if(g[i].faceVisible != nullptr and g[i].faceVisible->couleur == couleur)
            ++nombre;
    }
    return nombre;
}

unsigned int comptePointsAvecValidation(Grille g){
    unsigned int points = 0;
    for(int i = 0; i<9; ++i){
        if(g[i].faceVisible != nullptr and g[i].faceCachee != nullptr)
            points += (i+1);
    }
    return points;
}

unsigned int comptePointsZone(Grille g, unsigned int numManche){
    // Création d'un tableau de caractères qui représente les couleurs à la grille
    std::array<std::array<char, 3>, 3> tCouleurs;
    for(int i = 0; i < 9; ++i){
        if(g[i].faceVisible == nullptr)
            tCouleurs[i%3][i/3] = 'x';
        else
            tCouleurs[i%3][i/3] = g[i].faceVisible->couleur;
    }

    // Création d'un tableau de booléens qui va garder l'information des cases déja visitées
    std::array<std::array<bool, 3>, 3> tVisites;
    for(int i = 0; i < 9; ++i){
        tVisites[i%3][i/3] = false;
    }

    int taille_max = 0;
    
    // On va parcourir chaque case du tableaux des visites
    for (int col = 0; col < 3; ++col){
        for (int ligne = 0; ligne < 3; ++ligne){
            // Si la case n'a pas encore été visitée alors on débute une recherche à partir de celle-ci
            if (!tVisites[col][ligne]){
                int taille = tailleExpansionCouleur(tCouleurs, tVisites, col, ligne, tCouleurs[col][ligne]);
                // On met a jour la taille maximum si la zone parcourue était plus grande
                if(taille > taille_max)
                    taille_max = taille;
            }
        }
    }
    
    // Des points sont données seulement si il y a un endroit où deux cartes adjacentes sont de la même couleur
    if(taille_max > 1)
        // On applique le multiplicateur selon la manche
        return taille_max*(numManche+1);
    else
        return 0;
}

unsigned int tailleExpansionCouleur(std::array<std::array<char, 3>, 3> tabCouleurs, std::array<std::array<bool, 3>, 3>& dejaVisite, int col, int ligne, char couleur){
    // Si la case de départ est hors grille, 
    // OU Si la case a deja été visitée
    // OU Si la couleur de la case de départ n'est pas celle demandée et qu'elle n'est pas multicolore
    // Alors on arrête la recherche
    if(col < 0 or col > 2 or ligne < 0 or ligne > 2 or dejaVisite[col][ligne] or (tabCouleurs[col][ligne] != couleur and tabCouleurs[col][ligne] != 'n'))
       return 0;

    // Sinon la case actuelle devient déja visitée
    dejaVisite[col][ligne] = true;

    // Et on viste les cases à côtés et on ajoute 1 à la taille de la zone
    return 1 + tailleExpansionCouleur(tabCouleurs, dejaVisite, col+1, ligne, couleur) // A droite
             + tailleExpansionCouleur(tabCouleurs, dejaVisite, col-1, ligne, couleur) // A gauche
             + tailleExpansionCouleur(tabCouleurs, dejaVisite, col, ligne+1, couleur) // En bas
             + tailleExpansionCouleur(tabCouleurs, dejaVisite, col-1, ligne-1, couleur); // Au dessus
   
}