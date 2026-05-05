#pragma once

#include "Defausse.hpp"
#include "Pioche.hpp"
#include "Joueur.hpp"

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

// Ajoute un coup à une liste de coups.
void ajouterCoup(ListeDeCoups& lc, std::string coup);

// Affiche une liste de coup, une ligne par coup.
void afficher(ListeDeCoups lc);

struct Partie{
    Defausse defausse;
    Pioche pioche;
    std::array<Joueur, 5> joueurs;
    unsigned int nombreJoueurs;
    unsigned int prochainJoueur;
    unsigned int numeroManche;
    bool estMancheFinie;
    ListeDeCoups coupsManche;
};

// Lance une nouvelle partie a partir de zéro 
void lancerUneNouvellePartie();

// Renvoie un booléen qui indique si un des joueurs à rempli sa grille
bool unJoueurAFinit(Partie partie);

// Lance une manche du jeu
void lancerManche(Partie& partie);

// Fais jouer le prochain joueur de la partie
void faireJouerProchain(Partie& partie);

std::string demanderCoupJoueur(const Partie& partie);

std::string demanderCoupOrdi(Partie& partie);

// Parcours chaque grille et la pioche et envoie les cartes dans la défausse
void toutRemettreDansDefausse(Partie& partie);

// Change la valeur du joueur suivant en la mettant au joueur après l'actuel dans la liste
void changerDeJoueur(Partie& partie);

// Change la valeur du joueur suivant en la mettant au joueur avant l'actuel dans la liste
void remettreDernierJoueurCommeSuivant(Partie& partie);

void jouerCoup(Partie& partie, std::string coup);

// Renvoie le dernier coup joué dans une manche. "NULL" si aucun coup.
std::string recupDernierCoup(Partie partie);

// Renvoie l'indice du joueur du dernier coup de la manche. 10 si aucun.
unsigned int joueurDuDernierCoup(Partie partie);