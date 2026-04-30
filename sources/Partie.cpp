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

    lancerManche(partie, 1);
}

void lancerManche(Partie& partie, unsigned int numeroManche){
    while(not unJoueurAFinit(partie)){
        remplirPioche(partie.pioche, partie.defausse, partie.nombreJoueurs);
        if(partie.nombreJoueurs == 2){
            for(unsigned int i = 0; i < 4; ++i){
                tourDeJeu(partie, i%2);
            }
        }else{
            for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
                tourDeJeu(partie, i);
            }
        }
    }
    std::cout << "Manche Terminée" << std::endl;
}

// Fais piocher un joueur dans la pioche et mets sa carte dans sa grille
void tourDeJeu(Partie& partie, unsigned int joueur){
    std::cout << std::endl;
    std::cout << "Au tour de " << partie.joueurs[joueur].surnom << ". Voici votre grille : " << std::endl;
    afficherGrille(partie.joueurs[joueur].grilleDeJeu);
    Carte* carteChoisi = prendrePioche(partie.pioche);
    ajouterCarte(partie.joueurs[joueur].grilleDeJeu, carteChoisi);
    std::cout << "Voici votre grille maintenant :" << std::endl;
    afficherGrille(partie.joueurs[joueur].grilleDeJeu);
    std::cout << std::endl;
}

// Renvoie un booléen qui indique si un des joueurs à rempli sa grille
bool unJoueurAFinit(Partie partie){
    unsigned int i = 0;
    while(i < partie.nombreJoueurs and not finJeu(partie.joueurs[i].grilleDeJeu))
        ++i;
    return i != partie.nombreJoueurs;
}
