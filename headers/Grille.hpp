#pragma once

#include <array>
#include "Carte.hpp"

// Structure pour un emplacement de la Grille d'un joueur.
struct emplacementGrille{
    Carte* faceVisible;
    Carte* faceCachee;
};

// Initialise un emplacement de Grille comme étant vide.
void initEmplacement(emplacementGrille & emplGrille);

// Affiche un emplacement de Grille sous la forme : c. cachée / c. visible.
void afficher(const emplacementGrille& emplGrille);

// Définition de la Grille d'un joueur
using Grille = std::array<emplacementGrille, 9>;

// Initialise une Grille vide
void initGrille(Grille & g);

// Affiche une Grille
void afficherGrille(const Grille& g);

// Ajoute une Carte à la Grille 
void ajouterCarte(Grille& g, Carte* c);

// Fonction qui regarde si la Grille à min une Carte par emplacement si oui renvoie true
bool finJeu(const Grille& g);

// Retourne le nombre de points sur la grille actuelle selon la manche
int comptePoints(const Grille& g, const unsigned int numeroManche);

// Retourne les points obtenus avec les spirales (simples et spéciales)
int compteSpirales(const Grille& g);

// Retourne le nombre de cartes d'une certaine couleur
unsigned int compteCarteCouleur(const Grille& g, const char couleur);

// Retourne le nombre de points obtenus grâce aux cartes qui sont validées
unsigned int comptePointsAvecValidation(const Grille& g);

// Retourne le nombre de points obtenus grâce à la plus grande zone de couleur
unsigned int comptePointsZone(const Grille& g, const unsigned int numManche);

// Fonction récursive qui explore le tableaux de couleurs en restant sur une meme couleur, renvoie la taille de la zone en question
unsigned int tailleExpansionCouleur(const std::array<std::array<char, 3>, 3>& tabCouleurs, std::array<std::array<bool, 3>, 3>& dejaVisite, const int col, const int ligne, const char couleur);

// Renvoie vrai si l'emplacement est validé
bool emplacementEstValide(const Grille& g, const unsigned int numeroEmpl);

// Renvoie vrai si l'emplacement ne contient aucune carte
bool estVideEmplacement(const Grille& g, const unsigned int indice);