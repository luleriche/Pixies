#include <cstdlib>
#include <ctime>

#include "Partie.hpp"
#include "Console.hpp"

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
    std::cout << "Le jeu se joue de 2 à 5 joueurs !" << std::endl << "Nombre de joueurs: ";
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
    unsigned int joueurActuel = premierJoueur;
    // Si il n'y a que deux joueurs la manche peut se finir si il reste deux cartes dans la pioche
    if(partie.nombreJoueurs == 2){
        // Tant qu'un joueur n'a pas rempli sa grille
        while(not unJoueurAFinit(partie)){
            // On remplit la pioche si elle est vide
            if(estVidePioche(partie.pioche)){
                remplirPioche(partie.pioche, partie.defausse, 4);
                joueurActuel = partie.dernierJoueur;
            }
            // On fait jouer les deux joueurs
            tourDeJeu(partie, joueurActuel);
            tourDeJeu(partie, (joueurActuel + 1)%2);
        }
    // Si le nombre de joueur est supérieur à 2
    }else{
        // Tant qu'un joueur n'a pas rempli sa grille
        while(not unJoueurAFinit(partie)){
            // On remplie la pioche et faisons un tour de table
            remplirPioche(partie.pioche, partie.defausse, partie.nombreJoueurs);
            for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
                tourDeJeu(partie, (joueurActuel + i)%partie.nombreJoueurs);
            }
            joueurActuel = partie.dernierJoueur;
        }
    }
    std::cout << "Manche Terminée. Voici les points désormais." << std::endl;
    // Comptage des points
    for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
        ajoutePointsDeGrille(partie.joueurs[i], numeroManche);
        // Affiche les nouveaux points
        std::cout << partie.joueurs[i].surnom << " : " << partie.joueurs[i].nbPoints << " points." << std::endl;
    }
}

// Fais piocher un joueur dans la pioche et mets sa carte dans sa grille
void tourDeJeu(Partie& partie, unsigned int joueur){
    // Affichage avant de choisir la carte a prendre
    std::cout << std::endl; // Si le efface console ne marche pas au moins on n'est pas collé à avant
    //effaceConsole();
    std::cout << "Tour de " << partie.joueurs[joueur].surnom << ". Votre grille : " << std::endl;
    afficherGrille(partie.joueurs[joueur].grilleDeJeu);
    
    // Choix de la carte et ajout de celle-ci à la grille
    unsigned int taillePioche = partie.nombreJoueurs;
    if(partie.nombreJoueurs == 2)
        taillePioche = 4;
    Carte* carteChoisi = prendrePioche(partie.pioche, taillePioche);
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
