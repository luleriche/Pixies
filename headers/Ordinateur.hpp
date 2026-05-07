#pragma once

#include <string>
#include <array>

#include "Partie.hpp"

const int NbMaxEnfants = 32;

// Structure qui représente un noeud dans l'arbre de recherche
struct Noeud{
    // Dernier coup qui a été joué pour arriver ici
    std::string coupCreateur;
    // Pointeur vers le Noeud Parent
    Noeud* parent;
    // Pointeurs vers les Noeuds Enfants
    std::array<Noeud*, NbMaxEnfants> enfants;
    unsigned int nbEnfants;
    // Statistiques de ce noeud
    std::array<double, 5> nbVictoires;
    int nbVisites;
    // Liste des coups non-visités à partir de ce Node
    ListeDeCoups coupsNonVisites;
};

// Annuler le dernier coup joué dans une manche
void annulerDernierCoup(Partie& partie);

// Renvoyer une liste de coups contenant tous les coups possibles pour le prochain joueur d'une partie.
ListeDeCoups recupCoupsPossibles(const Partie& partie);

// Fais jouer un coup aléatoire pour le prochain joueur
void jouerCoupAlea(Partie& partie);

// Finir la manche actuelle avec uniquement des coups aléatoire
void finirMancheAleatoirement(Partie& partie);

// Récupérer le joueur en tête si l'on simule une fin de manche aléatoire
unsigned int recupLeaderFinMancheAleatoire(Partie& partie);

void initNoeud(Noeud& noeud, Partie& partie, std::string coup, Noeud* parent);

void ajouterEnfant(Noeud& noeud, Partie& partie);

bool ajoutEnfantPossible(const Noeud& n);

std::string recupMeilleurCoup(Partie& partie);

void annulerDernierCoup(Partie& partie);

ListeDeCoups recupCoupsPossibles(const Partie& partie);

void jouerCoupAlea(Partie& partie);

void finirMancheAleatoirement(Partie& partie);

unsigned int recupLeaderFinMancheAleatoire(Partie& partie);

Noeud* choisirEnfant(Noeud* n, unsigned int joueur);

float calculerRatioVictoire(Noeud n, unsigned int joueur);

float calculerUCT(int nbVisitesParent, int nbVistesEnfant, float ratioVictoire, float temperature);

void supprimerArbre(Noeud* ptrRacine);

void reinitRacine(Noeud& racine, ListeDeCoups coupsNonVisites);
