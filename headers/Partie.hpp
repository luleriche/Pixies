#pragma once

#include "Defausse.hpp"
#include "Pioche.hpp"
#include "Joueur.hpp"

/* 
Un coup se construit ainsi :
- indice du joueur qui a joué le coup (0 à 4)
- indice de la carte prise dans la pioche (0 à 4)
- type du coup (d/v/c/m)
- indice dans la grille ou a finit la carte (0 à 8)
*/

// Structure pour une liste de de coups d'une manche.
const int NbMaxCoupsManche = 200;
struct ListeDeCoupsManche{
    std::array<std::string, NbMaxCoupsManche> coups; // Liste qui contient les coups
    unsigned int nombre; // Nombre de coups dans la liste
};

// Structure pour une liste de coups possible lors d'un tour de jeu.
const int NbMaxCoupsPossibles = 80;
struct ListeDeCoupsPossibles{
    std::array<std::string, NbMaxCoupsPossibles> coups; // Liste qui contient les coups
    unsigned int nombre; // Nombre de coups dans la liste
};

// Ajouter un coup à une liste de coups.
void ajouterCoup(ListeDeCoupsPossibles& lc, std::string coup);
// Ajouter un coup à une liste de coups
void ajouterCoup(ListeDeCoupsManche& lc, std::string coup);

// Afficher une liste de coup avec un coup par ligne.
void afficher(const ListeDeCoupsPossibles& lc);
// Afficher une liste de coup avec un coup par ligne.
void afficher(const ListeDeCoupsManche& lc);

// Supprimer le dernier coup d'une liste de coups.
void supprimeDernierCoup(ListeDeCoupsManche& lc);
// Supprimer le dernier coup d'une liste de coups.
void supprimeDernierCoup(ListeDeCoupsPossibles& lc);



// Structure pour une partie de Pixies
struct Partie{
    // Boite qui contient constamment toutes les cartes de la partie
    BoiteCartes boite;
    // Défausse de la partie
    Defausse defausse;
    // Pioche de la partie
    Pioche pioche;
    // Joueurs de la partie
    std::array<Joueur, 5> joueurs;
    unsigned int nombreJoueurs; // Nombre de joueurs dans la partie
    // Indice du prochain joueur (cad. celui qui s'apprête à jouer) dans la liste des joueurs de la partie
    unsigned int prochainJoueur;
    // Numéro de la manche actuelle
    unsigned int numeroManche;
    // Si la manche actuelle est terminée ou non
    bool estMancheFinie;
    // Liste des coups joués depuis le début de la manche actuelle
    ListeDeCoupsManche coupsManche;
};

// Lancer une nouvelle partie à partir de zéro dans la console.
void lancerPartieConsole();

// Lancer une manche d'une partie de Pixies. Toutes les cartes doivent se trouver dans la défausse
void lancerManche(Partie& partie, const unsigned int numeroManche);

// Faire jouer le prochain joueur de la partie.
void faireJouerProchain(Partie& partie);

// Demander à l'utilisateur le coup qu'il jouerait si il était le prochain joueur à l'aide de la console.
std::string demanderCoupJoueur(const Partie& partie);

// Jouer un coup sur une partie, le coup est supposé valide.
void jouerCoup(Partie& partie, std::string coup);

// Renvoyer le dernier coup joué dans une manche. Retourne "NULL" si aucun coup n'a encore été joué.
std::string recupDernierCoup(const Partie& partie);

// Renvoyer l'indice du joueur qui a joué le dernier coup de la manche. Renvoie 10 si aucun coup n'a encore été joué.
unsigned int joueurDuDernierCoup(const Partie& partie);

// Savoir si un joueur d'une partie rempli sa grille ou non.
bool unJoueurAFinit(const Partie& partie);

// Remettre les cartes de la pioche et des grilles des joueurs dans la défausse d'une partie.
void toutRemettreDansDefausse(Partie& partie);

// Changer la valeur du joueur suivant en la mettant au joueur après lui dans la liste des joueurs.
void changerDeJoueur(Partie& partie);

// Récupérer le joueur qui a le plus de points d'une partie.
unsigned int recupLeader(const Partie& partie);

// Afficher les grilles des joueurs et la pioche d'une partie.
void afficher(const Partie& partie);