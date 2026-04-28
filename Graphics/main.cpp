#include <iostream>
#include <array>

// Structure pour une carte de jeu.
struct carte{
    char couleur;
    unsigned int chiffre; // Chiffre sur la carte
    int spirale; // < 0 : nombre de croix, < 7 : nombre de spirales, 9 : carte spéciale
};

void afficher(carte c){
    std::cout << c.chiffre << " " << c.couleur << " " << c.spirale;
}

// Définition de la défausse/pile de cartes.
struct maillon{
    carte* valeur;
    maillon* suivant;
};
using defausse = maillon*;

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

// Structure pour un emplacement de la grille d'un joueur.
struct emplacementGrille{
    carte* faceVisible;
    carte* faceCachee;
};

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


// Définition de la grille d'un joueur
using grille = std::array<emplacementGrille, 9>;

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
        std::cout << "1 ("; afficher(*g[c->chiffre-1].faceVisible);
        std::cout << ") / 2 ("; afficher(*c); std::cout << ")" << std::endl;
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

// Structure pour la pioche
struct pioche{
    std::array<carte*,5> cartes;
    unsigned int taille;
};


int main()
{
    grille g;
    initGrille(g);
    afficherGrille(g);

    defausse d;
    initDefausse(d);
    ajoutFinDefausse(d, {'r', 4, 1});
    ajoutFinDefausse(d, {'b', 4, 1});
    ajoutFinDefausse(d, {'j', 4, 1});
    ajoutFinDefausse(d, {'v', 7, 1});
    
    ajouterCarte(g, tirerCarteDessus(d));
    afficherGrille(g);
    ajouterCarte(g, tirerCarteDessus(d));
    afficherGrille(g);
    ajouterCarte(g, tirerCarteDessus(d));
    afficherGrille(g);
    return 0;
}