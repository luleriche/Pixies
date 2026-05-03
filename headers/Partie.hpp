#pragma once

#include "Defausse.hpp"
#include "Pioche.hpp"
#include "Joueur.hpp"

struct Partie{
    Defausse defausse;
    Pioche pioche;
    std::array<Joueur, 5> joueurs;
    unsigned int nombreJoueurs;
    unsigned int prochainJoueur;
    unsigned numeroManche;
};

// Lance une nouvelle partie a partir de zéro 
void lancerUneNouvellePartie();

// Renvoie un booléen qui indique si un des joueurs à rempli sa grille
bool unJoueurAFinit(Partie partie);

// Lance une manche du jeu
void lancerManche(Partie& partie);

// Fais jouer un joueur
void faireJouer(Partie& partie, unsigned int joueur);

// Parcours chaque grille et la pioche et envoie les cartes dans la défausse
void toutRemettreDansDefausse(Partie& partie);
