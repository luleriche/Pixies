#pragma once

#include <string>
#include <array>

#include "Partie.hpp"

const int NbMaxEnfants = NbMaxCoupsPossibles;

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
    ListeDeCoupsPossibles coupsNonVisites;
};

// Récupérer le meilleur coup possible dans un certain état de la partie, les parmètre de recherche sont explicites pour une recherche MCTS.
// Paramètres conseillées 20, 300
std::string recupMeilleurCoup(Partie& partie, const unsigned int nbDefausse, const unsigned int nbDescentesParDefausses);

// Initialiser un noeud avec les valeurs passer en paramètres
void initNoeud(Noeud* const noeud, const Partie& partie, const std::string& coupCreateur, Noeud* const parent);

// Renvoyer une liste de coups contenant tous les coups possibles pour le prochain joueur d'une partie.
ListeDeCoupsPossibles recupCoupsPossibles(const Partie& partie);

// Annuler le dernier coup joué dans une manche
void annulerDernierCoup(Partie& partie);

// Jouer un coup aléatoire à un certain moment de la partie.
void jouerCoupAlea(Partie& partie);

// Jouer le coup qui rapporte le plus de points au prochain joueur. Cette fonction a été testé pour les simulation de fin de partie dans l'arbre de recherche mais elle les rends 10 fois plus longue
void jouerCoupMaximisePoints(Partie& partie);

// Récupérer le joueur en tête après une simulation de fin de manche
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


// Lire les coups d'un fichier et les jouer sur une partie de 5 joueurs vierge.
void lireFichierPartie(const std::string nomFichier, Partie& partie);

// Ecrire dans un fichier le meilleur prochain coup à jouer
void jouerCoupOrdiEtEcrire(Partie& partie, const std::string nomFichier);

// Traduire un coup dans le format de notre projet à celui d'un fichier d'une partie (ex 43d3 -> 5,5 v 0 0,5,v)
std::string traduireCoupNormalVersFichier(const std::string coup, const Partie& partie);

// Traduire un coup dans le format d'un fichier à celui de notre format (ex 5,5 v 0 0,5,v -> 43d3)
std::string traduireCoupFichierVersNormal(const std::string& ligneCoup, const Partie& partie);

// Récupérer une carte qui est écrit dans le format utilisé dans un fichier de partie
Carte lireCarteDeFichier(const std::string& txtCarte);

// Récupérer le texte qui décrit une carte pour le mettre dans une fichier de partie
std::string construireTexteCartePourFichier(const Carte carte);

// Ecrire une partie aléatoire dans un fichier.
void ecrirePartieAlea(const std::string nomFichier);

// Ecrire les cartes qu'il y a dans la pioche d'une partie à la find d'un fichier
void ecrirePiocheDansFichier(const std::string nomFichier, const Partie& partie);

// Ecire le dernier coupd 'une partie dans un fichier.
void ecrireCoupDansFichier(const std::string nomFichier, const std::string coup, const Partie& partie);