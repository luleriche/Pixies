#pragma once

#include "Defausse.hpp"
#include "Pioche.hpp"
#include "Joueur.hpp"

struct Partie{
    Defausse defausse;
    Pioche pioche;
    std::array<Joueur, 5> joueurs;
    unsigned int nombreJoueurs;
    unsigned int dernierJoueur;
    unsigned numeroManche;
};

// Lance une nouvelle partie a partir de zéro 
void lancerUneNouvellePartie();

// Renvoie un booléen qui indique si un des joueurs à rempli sa grille
bool unJoueurAFinit(Partie partie);

// Lance une manche précise du jeu
void lancerManche(Partie& partie, unsigned int numeroManche, unsigned int premierJoueur);

// Fais jouer un joueur
void tourDeJeu(Partie& partie, unsigned int joueur);
