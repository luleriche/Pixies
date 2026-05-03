#include <cstdlib>
#include <ctime>

#include "Partie.hpp"
#include "Console.hpp"
#include "Ordinateur.hpp"

void lancerUneNouvellePartie(){
    // Création de la partie
    Partie partie;

    // Création des cartes dans la mémoire à l'aide d'une boite de cartes
    BoiteCartes boite;
    unsigned int nbCartes;
    creerCartesAvecFichier("assets/cartes_pixies.txt", boite, nbCartes);

    // Chargement des cartes et mélange
    Defausse defausse;
    initDefausse(defausse);
    remplir(boite, nbCartes, defausse);
    melanger(defausse);
    partie.defausse = defausse;

    // Création de la pioche
    Pioche pioche;
    initPioche(pioche);
    partie.pioche = pioche;
    
    // On demande à l'utilisateur le nombre de joueurs à la partie.
    std::cout << "Le jeu se joue de 2 à 5 joueurs !" << std::endl << "Nombre de joueurs: ";
    std::cin >> partie.nombreJoueurs;
    while(partie.nombreJoueurs > 5 and partie.nombreJoueurs < 2){
        std::cout << "Impossible. Le jeu se joue de 2 à 5 joueurs ! Nombre de joueurs: ";
        std::cin >> partie.nombreJoueurs;
    }

    if(partie.nombreJoueurs == 2)
        partie.pioche.taille = 4;
    else
        partie.pioche.taille = partie.nombreJoueurs;
    
    // On demande à l'utilisateur d'entrer le nom des joueurs
    creerJoueurs(partie.joueurs, partie.nombreJoueurs, &partie.pioche);
    
    // Génération aléatoire du premier joueur.
    partie.prochainJoueur = rand()%partie.nombreJoueurs;
    std::cout << "Le premier joueur sera " << partie.joueurs[partie.prochainJoueur].surnom << std::endl;

    partie.numeroManche = 1;
    // Manche 1
    lancerManche(partie);

    toutRemettreDansDefausse(partie);
    melanger(partie.defausse);
    ++partie.numeroManche;

    // Manche 2
    lancerManche(partie);

    toutRemettreDansDefausse(partie);
    melanger(partie.defausse);
    ++partie.numeroManche;

    // Manche 3
    lancerManche(partie);

    std::cout << "La partie est terminée." << std::endl;
    toutRemettreDansDefausse(partie);
    viderDefausse(partie.defausse);
    supprimerBoite(boite);
}

void lancerManche(Partie& partie){
    std::cout << "DEBUT DE LA MANCHE " << partie.numeroManche << std::endl;
    // Si il n'y a que deux joueurs la manche peut se finir si il reste deux cartes dans la pioche
    if(partie.nombreJoueurs == 2){
        remplirPioche(partie.pioche, partie.defausse);
        // Tant qu'un joueur n'a pas rempli sa grille
        while(not unJoueurAFinit(partie)){
            // On remplit la pioche si elle est vide
            if(estVidePioche(partie.pioche)){
                remplirPioche(partie.pioche, partie.defausse);
                // Le prochain joueur devient le dernier
                finDeTour(partie.prochainJoueur, partie.nombreJoueurs);
            }
            // On fait jouer les deux joueurs
            tourDeJeu(partie);
            tourDeJeu(partie);
        }
        // On change le prochain joueur car on a finit une manche (équivalent à la fin d'un tour à plus de deux joueurs)
        finDeTour(partie.prochainJoueur, partie.nombreJoueurs);
    }
    // Si le nombre de joueur est supérieur à 2
    else{
        // Tant qu'un joueur n'a pas rempli sa grille
        while(not unJoueurAFinit(partie)){
            // On remplie la pioche et faisons un tour de table
            remplirPioche(partie.pioche, partie.defausse);
            for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
                tourDeJeu(partie);
            }
            finDeTour(partie.prochainJoueur, partie.nombreJoueurs);
        }
    }
    std::cout << "Manche Terminée. Voici les points désormais." << std::endl;
    // Comptage des points
    for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
        ajoutePointsDeGrille(partie.joueurs[i], partie.numeroManche);
        // Affiche les nouveaux points
        std::cout << partie.joueurs[i].surnom << " : " << partie.joueurs[i].nbPoints << " points." << std::endl;
    }
}

// Fais piocher un joueur dans la pioche et mets sa carte dans sa grille
void tourDeJeu(Partie& partie){
    unsigned int joueur = partie.prochainJoueur;
    
    // Affichage avant de choisir la carte a prendre
    std::cout << std::endl << std::endl << "Tour de " << partie.joueurs[joueur].surnom << ". Votre grille : " << std::endl;
    afficherGrille(partie.joueurs[joueur].grilleDeJeu);
    
    // Ya que le joueur 4 qui joue les autres c'est des ordis
    if(joueur == 4){
        // Choix de la carte et ajout de celle-ci à la grille
        Carte* carteChoisi = prendrePioche(partie.pioche);
        ajouterCarte(partie.joueurs[joueur].grilleDeJeu, carteChoisi);
        // On change le prochain joueur
        changerDeJoueur(partie.prochainJoueur, partie.nombreJoueurs);
    }else{
        afficher(partie.pioche);
        ListeDeCoups coupsPossibles = recupCoupsPossibles(partie);
        afficher(coupsPossibles);
        std::cout << "L'ordi joue le premier coup" << std::endl;
        jouerCoup(partie, coupsPossibles.coups[0]);
        if(coupsPossibles.nombre > 1){
            afficherGrille(partie.joueurs[joueur].grilleDeJeu);
            std::cout << "Annulation du premier coup" << std::endl;
            annulerCoup(partie, coupsPossibles.coups[0]);
            afficherGrille(partie.joueurs[joueur].grilleDeJeu);
            afficher(partie.pioche);
            std::cout << "L'ordi joue le second coup" << std::endl;
            jouerCoup(partie, coupsPossibles.coups[1]);
        }
    }


    // Affichage après avoir rajouter la carte à la grille
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

// Parcours chaque grille et la pioche et envoie les cartes dans la défausse
void toutRemettreDansDefausse(Partie& partie){
    // Parcours des grilles des joueurs
    for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
        // Parcours de chaque case de sa grille
        for(unsigned int j = 0; j < 9; ++j){
            // On met les cartes dans la defausse et on vide la grille
            ajoutDebutDefausse(partie.defausse, partie.joueurs[i].grilleDeJeu[j].faceCachee);
            partie.joueurs[i].grilleDeJeu[j].faceCachee = nullptr;
            ajoutDebutDefausse(partie.defausse, partie.joueurs[i].grilleDeJeu[j].faceVisible);
            partie.joueurs[i].grilleDeJeu[j].faceVisible = nullptr;
        }
    }

    // Parcours des cartes de la pioche
    for(unsigned int i = 0; i < partie.pioche.taille; ++i){
        ajoutDebutDefausse(partie.defausse, partie.pioche.cartes[i]);
        partie.pioche.cartes[i] = nullptr;
    }
}