#pragma once

#include <string>
#include <array>

#include "Partie.hpp"

const int NbMaxEnfants = NbMaxCoups;

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
    unsigned int nbVisites;
    // Liste des coups non-visités à partir de ce Node
    ListeDeCoups coupsNonVisites;
};

// Récupérer le meilleur coup possible dans un certain état de la partie.
std::string recupMeilleurCoup(Partie& partie);

// Initialiser un noeud avec les valeurs passer en paramètres
void initNoeud(Noeud* const noeud, const Partie& partie, const std::string& coupCreateur, Noeud* const parent);

// Renvoyer une liste de coups contenant tous les coups possibles pour le prochain joueur d'une partie.
ListeDeCoups recupCoupsPossibles(const Partie& partie);

// Annuler le dernier coup joué dans une manche
void annulerDernierCoup(Partie& partie);

// Jouer un coup aléatoire à un certain moment de la partie.
void jouerCoupAlea(Partie& partie);

// Récupérer le joueur en tête après une simulation de fin de manche aléatoire
unsigned int recupLeaderFinMancheAleatoire(Partie& partie);

// Ajouter un enfant à un noeud et metttre jouer le coup le qui y amène, recupérer un pointeur vers le noeud crée
Noeud* ajouterEnfant(Noeud* const noeudActuel, Partie& partie);

// Choisir le meilleur enfant à explorer d'un noeud en utilisant le score UCT
Noeud* choisirEnfant(const Noeud* const n, unsigned int joueur);

// Désallouer un arbre en donnant le noeud Racine de l'arbre, désalloue également la racine.
void supprimerArbre(Noeud* const noeudRacine);

// Mettre un jour un arbre en supprimant tous les noeuds qui se trouvent après le premier tirage dans la pioche et en mettant à jour les coups non visité juste avant ce moment.
void metAJourArbre(Noeud* noeud, Partie& partie);

// Récupérer le ratio de victoire d'un joueur pour un noeud donné.
float calculerRatioVictoire(const Noeud* const n, const unsigned int joueur);

// Récupérer le score UCT en fonction des différents paramètres
float calculerUCT(const int nbVisitesParent, const int nbVistesEnfant, const float ratioVictoire, const float temperature);
