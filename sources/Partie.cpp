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
    std::cout << "Si le nom d'un joueur commence par $ alors il sera considéré comme un ordi et jouera automatiquement." << std::endl;
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
                remettreDernierJoueurCommeSuivant(partie);
            }
            // On fait jouer les deux joueurs
            faireJouer(partie, partie.prochainJoueur);
            changerDeJoueur(partie);
            faireJouer(partie, partie.prochainJoueur);
            changerDeJoueur(partie);
        }
        // On change le prochain joueur car on a finit une manche (équivalent à la fin d'un tour à plus de deux joueurs)
        remettreDernierJoueurCommeSuivant(partie);
    }
    // Si le nombre de joueur est supérieur à 2
    else{
        // Tant qu'un joueur n'a pas rempli sa grille
        while(not unJoueurAFinit(partie)){
            // On remplie la pioche et faisons un tour de table
            remplirPioche(partie.pioche, partie.defausse);
            for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
                faireJouer(partie, partie.prochainJoueur);
                changerDeJoueur(partie);
            }
            remettreDernierJoueurCommeSuivant(partie);
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

void faireJouer(Partie& partie, unsigned int joueur){
    std::string coup;
    if(partie.joueurs[joueur].surnom[0]=='$')
        coup = demanderCoupOrdi(partie);
    else{
        coup = demanderCoupJoueur(partie);
    }
    jouerCoup(partie, coup);
    
    // Affichage après avoir rajouter la carte à la grille
    std::cout << "Voici votre grille maintenant :" << std::endl;
    afficherGrille(partie.joueurs[joueur].grilleDeJeu);
    std::cout << std::endl;
}

std::string demanderCoupOrdi(Partie& partie){
    ListeDeCoups coupsPossibles = recupCoupsPossibles(partie);
    afficher(coupsPossibles);
    return coupsPossibles.coups[0];
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

// Change la valeur du joueur suivant en la mettant au joueur après l'actuel dans la liste
void changerDeJoueur(Partie& partie){
    partie.prochainJoueur = (partie.prochainJoueur + 1)%partie.nombreJoueurs;
}

// Change la valeur du joueur suivant en la mettant au joueur avant l'actuel dans la liste
void remettreDernierJoueurCommeSuivant(Partie& partie){
    partie.prochainJoueur = (partie.prochainJoueur - 1)%partie.nombreJoueurs;
}

std::string demanderCoupJoueur(const Partie& partie){

    const unsigned int& joueur = partie.prochainJoueur;
    const Grille& griJoueur = partie.joueurs[joueur].grilleDeJeu;

    // Affichage de la grille et de la pioche avant de choisir
    std::cout << std::endl << std::endl << "Tour de " << partie.joueurs[joueur].surnom << ". Votre grille : " << std::endl;
    afficherGrille(griJoueur);
    std::cout << "Pioche :" << std::endl;
    afficher(partie.pioche);

    //  -------- Choix de la carte parmi celle de la pioche --------
    unsigned int indiceCartePioche;
    std::cout << "Votre choix : ";
    std::cin >> indiceCartePioche;
    --indiceCartePioche;
    while (indiceCartePioche >= partie.pioche.taille or indiceCartePioche < 0 or partie.pioche.cartes[indiceCartePioche] == nullptr){
        std::cout<< "Erreur! Il n'y a pas de carte ici, réessayer : ";
        std::cin>> indiceCartePioche;
        --indiceCartePioche;
    }


    //  -------- Choix de la destination de la carte dans la grille --------
    
    // Pointeur vers la carte choisie juste avnt
    const Carte* carteChoisi = partie.pioche.cartes[indiceCartePioche];
    // Chiffre de la carte
    const unsigned int& chiffreCarte = carteChoisi->chiffre-1;
    

    // L'emplacement de la carte dans la grille est disponible, le coup est direct
    if(griJoueur[chiffreCarte-1].faceVisible == nullptr){
        std::cout << "L'emplacement de la carte est disponible." << std::endl;
        std::cout << "La carte y a est sera visible." << std::endl;
        return std::to_string(joueur)+std::to_string(indiceCartePioche)+'d'+std::to_string(chiffreCarte-1);
    
    
    // Si il y a une carte visible et pas de carte cachée, il faut faire le choix de laquelle on garde visible
    }else if(griJoueur[chiffreCarte-1].faceCachee == nullptr){
        unsigned int choix;
        std::cout << "Laissez visible 1 ou 2 ?" << std::endl;
        std::cout << "1 "; afficherEnCouleur(*griJoueur[chiffreCarte-1].faceVisible);; std::cout << std::endl;
        std::cout << "2 "; afficherEnCouleur(*carteChoisi); std::cout << std::endl;
        std::cout << "Choix: "; std::cin >> choix;
        while(choix != 1 and choix != 2){
            std::cout << "On a dit 1 ou 2 ! Réessayer : "; std::cin >> choix;
        }
        if(choix == 1){
            return std::to_string(joueur)+std::to_string(indiceCartePioche)+'c'+std::to_string(chiffreCarte-1);
        }else{
            return std::to_string(joueur)+std::to_string(indiceCartePioche)+'v'+std::to_string(chiffreCarte-1);
        }
    }



    // Si l'emplacement est validé (plein), il faut faire le choix d'où mettre la carte
    else{
        unsigned int choix;
        std::cout << "Emplacement validé !" << std::endl;
        std::cout <<  "Choisissez dans quelle case la mettre : ";
        std::cin >> choix;
        while(griJoueur[choix-1].faceCachee != nullptr or griJoueur[choix-1].faceVisible != nullptr){
            std::cout << "Erreur! Cet emplacement n'est pas vide. Réessayez : ";
            std::cin >> choix;
        }
        return std::to_string(joueur)+std::to_string(indiceCartePioche)+'m'+std::to_string(choix-1);
    }
}