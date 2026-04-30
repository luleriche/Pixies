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
    while(nbJoueurs > 5 and nbJoueurs < 2){
        std::cout << "Impossible. Le jeu se joue de 2 à 5 joueurs ! Nombre de joueurs: ";
        std::cin >> nbJoueurs;
    }
    partie.nombreJoueurs = nbJoueurs;
    // On demande à l'utilisateur d'entrer le nom des joueurs
    creerJoueurs(partie.joueurs, partie.nombreJoueurs, &partie.pioche);
    
    // Génération aléatoire du premier joueur.
    partie.dernierJoueur = rand()%partie.nombreJoueurs;
    std::cout << "Le premier joueur sera " << partie.joueurs[partie.dernierJoueur].surnom << std::endl;

    partie.numeroManche = 1;

    lancerManche(partie, 1, partie.dernierJoueur);
}

void lancerManche(Partie& partie, unsigned int numeroManche, unsigned int premierJoueur){
    // Si il n'y a que deux joueurs la manche peut se finir si il reste deux cartes dans la pioche
    if(partie.nombreJoueurs == 2){
        // Tant qu'un joueur n'a pas rempli sa grille
        while(not unJoueurAFinit(partie)){
            // On remplit la pioche si elle est vide
            if(estVidePioche(partie.pioche))
                remplirPioche(partie.pioche, partie.defausse, 4);
            // On fait jouer les deux joueurs
            tourDeJeu(partie, premierJoueur);
            tourDeJeu(partie, (premierJoueur + 1)%2);
        }
    // Si le nombre de joueur est supérieur à 2
    }else{
        // Tant qu'un joueur n'a pas rempli sa grille
        while(not unJoueurAFinit(partie)){
            // On remplie la pioche et faisons un tour de table
            remplirPioche(partie.pioche, partie.defausse, partie.nombreJoueurs);
            for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
                tourDeJeu(partie, (premierJoueur + i)%partie.nombreJoueurs);
            }
        }
    }
    std::cout << "Manche Terminée" << std::endl;
}

// Fais piocher un joueur dans la pioche et mets sa carte dans sa grille
void tourDeJeu(Partie& partie, unsigned int joueur){
    // Affichage avant de choisir la carte a prendre
    std::cout << std::endl;
    std::cout << "Au tour de " << partie.joueurs[joueur].surnom << ". Voici votre grille : " << std::endl;
    afficherGrille(partie.joueurs[joueur].grilleDeJeu);
    
    // Choix de la carte et ajout de celle-ci à la grille
    Carte* carteChoisi = prendrePioche(partie.pioche);
    ajouterCarte(partie.joueurs[joueur].grilleDeJeu, carteChoisi);
    
    // Affichage après avoir rajouter la carte à la grille
    std::cout << "Voici votre grille maintenant :" << std::endl;
    afficherGrille(partie.joueurs[joueur].grilleDeJeu);
    std::cout << std::endl;

    partie.dernierJoueur = joueur;
}

// Renvoie un booléen qui indique si un des joueurs à rempli sa grille
bool unJoueurAFinit(Partie partie){
    unsigned int i = 0;
    while(i < partie.nombreJoueurs and not finJeu(partie.joueurs[i].grilleDeJeu))
        ++i;
    return i != partie.nombreJoueurs;
}
