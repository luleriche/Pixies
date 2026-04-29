#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <optional>

// Structure pour une carte de jeu.
struct carte{
    char couleur; 
    unsigned int chiffre; // Chiffre sur la carte
    int spirale; // < 0 : nombre de croix, < 7 : nombre de spirales, 9 : carte spéciale
};

void afficher(carte c){
    std::cout << "(" << c.chiffre << " " << c.couleur << " " << c.spirale << ")";
}

// Définition de la défausse/pile de cartes.
struct maillon{
    carte* valeur;
    maillon* suivant;
};
using defausse = maillon*;

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

// Structure pour la pioche
struct pioche{
    std::array<carte*,5> cartes;
    unsigned int taille;
};

// Fonction qui regarde si la grille à min une carte par emplacement si oui renvoie true
bool finJeu(grille g){
    for (int i = 0; i < 9; ++i){
        if(g[i].faceCachee == nullptr and g[i].faceVisible == nullptr){
            return false;
        }
    }
    return true;
}

int main()
{
    srand(time(nullptr));
    grille ma_grille;
    initGrille(ma_grille);
    afficherGrille(ma_grille);

    defausse ma_def;
    initDefausse(ma_def);
    lireFichierDefausse("assets/cartes_pixies.txt", ma_def);
    melanger(ma_def);
    afficher(ma_def);

    sf::RenderWindow window(sf::VideoMode({1000, 800}), "SFML 3 Image");

    // Chemin relatif vers l'image
    sf::Texture texture;
    if (!texture.loadFromFile("assets/images_pixies/1_b_6.jpg")) {
        std::cerr << "Erreur : impossible de charger l'image !" << std::endl;
        return -1;
    }

    // Créer le sprite à partir de la texture
    sf::Sprite sprite(texture);

    sf::Vector2u textureSize = texture.getSize();  // largeur et hauteur de l'image originale
    float targetWidth = 400.f;
    float targetHeight = 600.f;

    // Calcul du facteur de redimensionnement pour garder les proportions
    float scaleX = targetWidth / textureSize.x;
    float scaleY = targetHeight / textureSize.y;
    float scale = std::min(scaleX, scaleY);  // prend le plus petit pour que l'image tienne dans le cadre
    sprite.setScale({scale, scale});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    return 0;
}