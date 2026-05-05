#pragma once

#include "Defausse.hpp"
#include "Pioche.hpp"
#include "Joueur.hpp"

// Structure pour une liste de de coups.
const int NbMaxCoups = 32;
struct ListeDeCoups{
    /* Un coup se construit ainsi :
    - indice du joueur qui a joué le coup
    - indice de la carte prise dans la pioche
    - type du coup (d/v/c/m)
    - indice dans la grille ou a finit la carte
    */ 
    std::array<std::string, NbMaxCoups> coups;
    unsigned int nombre;
};

// Ajouter un coup à une liste de coups.
void ajouterCoup(ListeDeCoups& lc, std::string coup);

// Afficher une liste de coup, autant de lignes que de coups.
void afficher(ListeDeCoups lc);

// Supprimer le dernier coup d'une liste de coups.
void supprimeDernierCoup(ListeDeCoups& lc);


// Structure pour une partie de Pixies
struct Partie{
    // Défausse de la partie
    Defausse defausse;
    // Pioche de la partie
    Pioche pioche;
    // Joueurs de la partie
    std::array<Joueur, 5> joueurs;
    unsigned int nombreJoueurs;
    // Indice du prochain joueurs dans la liste des joueurs de la partie
    unsigned int prochainJoueur;
    // Numéro de la manche actuelle
    unsigned int numeroManche;
    // Si la manche actuelle est terminée ou non
    bool estMancheFinie;
    // Liste des coups joués depuis le début de la manche actuelle
    ListeDeCoups coupsManche;
};

// Lance une nouvelle partie à partir de zéro.
void lancerUneNouvellePartie();

// Savoir si un joueur d'une partie a sa grille de remplie.
bool unJoueurAFinit(Partie partie);

// Lancer une manche d'une partie en utilisant la valeur actuelle de numeroManche.
void lancerManche(Partie& partie);

// Faire jouer le prochain joueur de la partie.
void faireJouerProchain(Partie& partie);

// Demander à l'utilisateur le coup qu'il jouerait si il était le prochain joueur 
std::string demanderCoupJoueur(const Partie& partie);

// Demander à l'ordinateur le coup qu'il jouerait si il était le prochain joueur
std::string demanderCoupOrdi(Partie& partie);

// Remettre les cartes de la pioche et des grilles des joueurs dans la défausse d'une partie.
void toutRemettreDansDefausse(Partie& partie);

// Changer la valeur du joueur suivant en la mettant au joueur après lui dans la liste des joueurs.
void changerDeJoueur(Partie& partie);

// Changer la valeur du joueur suivant en la mettant au joueur après lui dans la liste des joueurs.
void remettreDernierJoueurCommeSuivant(Partie& partie);

// Jouer un coup sur une partie, le coup est supposé possibl et valide.
void jouerCoup(Partie& partie, std::string coup);

// Renvoyer le dernier coup joué dans une manche. "NULL" si aucun coup.
std::string recupDernierCoup(Partie partie);

// Renvoyer l'indice du joueur du dernier coup de la manche. 10 si aucun.
unsigned int joueurDuDernierCoup(Partie partie);

//  Récupérer le joueur qui a le plus de points d'une partie.
unsigned int recupLeader(const Partie& partie);

// Afficher la pioche , les 5 premières cartes de la défausse et les grilles des joueurs d'une partie.
void afficher(const Partie& partie);