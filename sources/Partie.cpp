#include <cstdlib>
#include <ctime>
#include "Partie.hpp"

void lancerUneNouvellePartie(){
    // Création de la partie
    Partie partie;

    // Chargement des cartes et mélange
    Defausse defausse;
    initDefausse(defausse);
    lireFichierDefausse("assets/cartes_pixies.txt", defausse);
    melanger(defausse);
    partie.defausse = defausse;

    // Création de la pioche
    Pioche pioche;
    initPioche(pioche);
    partie.pioche = pioche;
    
    // On demande à l'utilisateur le nombre de joueurs à la partie.
    unsigned int nbJoueurs;
    std::cout << "Le jeu se joue de 2 à 5 joueurs ! Nombre de joueurs: ";
    std::cin >> nbJoueurs;
    while(nbJoueurs > 5){
        std::cout << "Impossible. Le jeu se joue de 2 à 5 joueurs ! Nombre de joueurs: ";
        std::cin >> nbJoueurs;
    }
    partie.nombreJoueurs = nbJoueurs;
    // On demande à l'utilisateur d'entrer le nom des joueurs
    creerJoueurs(partie.joueurs, partie.nombreJoueurs, &partie.pioche);
    
    // Génération aléatoire du premier joueur.
    partie.joueurActuel = rand()%partie.nombreJoueurs;
    std::cout << "Le premier joueur sera " << partie.joueurs[partie.joueurActuel].surnom << std::endl;

    partie.numeroManche = 1;

    std::cout << "Le jeu est prêt à être lancé !" << std::endl;
}
