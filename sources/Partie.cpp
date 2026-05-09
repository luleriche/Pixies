#include <cstdlib>
#include <ctime>

#include "Partie.hpp"
#include "Console.hpp"
#include "Ordinateur.hpp"

void ajouterCoup(ListeDeCoups& lc, std::string coup){
    lc.coups[lc.nombre] = coup;
    ++lc.nombre;
}

void afficher(ListeDeCoups lc){
    for(unsigned int i = 0; i < lc.nombre; ++i)
        std::cout << lc.coups[i] << std::endl;
}

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
    partie.pioche = pioche;
    
    // On demande à l'utilisateur le nombre de joueurs à la partie.
    std::cout << "Le jeu se joue de 2 à 5 joueurs !" << std::endl << "Nombre de joueurs: ";
    std::cin >> partie.nombreJoueurs;
    while(partie.nombreJoueurs > 5 or partie.nombreJoueurs < 2){
        std::cout << "Impossible. Le jeu se joue de 2 à 5 joueurs ! Nombre de joueurs: ";
        std::cin >> partie.nombreJoueurs;
    }
    // On demande à l'utilisateur d'entrer le nom des joueurs
    std::cout << "Si le nom d'un joueur commence par $ alors il sera considéré comme un ordi et jouera automatiquement." << std::endl;
    creerJoueurs(partie.joueurs, partie.nombreJoueurs, &partie.pioche);

    // On initialise la pioche
    if(partie.nombreJoueurs == 2)
        initPioche(partie.pioche, 4);
    else
        initPioche(partie.pioche, partie.nombreJoueurs);
    
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
    partie.estMancheFinie = false;
    partie.coupsManche.nombre = 0;

    remplirPioche(partie.pioche, partie.defausse);
    std::cout << "DEBUT DE LA MANCHE " << partie.numeroManche << std::endl;
    
    while(not partie.estMancheFinie){
        faireJouerProchain(partie);
    }

    std::cout << "Manche Terminée. Voici les points désormais." << std::endl;
    // Affichage des points
    for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
        std::cout << partie.joueurs[i].surnom << " : " << partie.joueurs[i].nbPoints << " points." << std::endl;
    }
}

void faireJouerProchain(Partie& partie){
    std::cout << "Tour de " << partie.joueurs[partie.prochainJoueur].surnom << std::endl;
    std::string coup;
    if(partie.joueurs[partie.prochainJoueur].surnom[0]=='$'){
        std::cout << "L'ordinateur choisi son coup." << std::endl;
        coup = recupMeilleurCoup(partie, 40, 600);
    }else{
        coup = demanderCoupJoueur(partie);
    }
    jouerCoup(partie, coup);
    //Affichage après avoir rajouter la carte à la grille
    afficher(partie);
}

std::string demanderCoupJoueur(const Partie& partie){
    const unsigned int& joueur = partie.prochainJoueur;
    const Grille& griJoueur = partie.joueurs[joueur].grilleDeJeu;
    
    // Affichage de la partie avant de choisir
    afficher(partie);
    
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
    const unsigned int& chiffreCarte = carteChoisi->chiffre;
    
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

void jouerCoup(Partie& partie, std::string coup){
    // Joueur qui joue le coup
    unsigned int joueur = coup[0] - '0';
    // Indice de la carte à prendre dans la pioche
    unsigned int indiceCartePioche = coup[1] - '0';
    // Type du coup voulant être joué
    char typeCoup = coup[2];
    // Indice où la carte sera mise dans la grille
    unsigned int indiceDestGrille = coup[3] - '0';
    
    // Référence vers la grille du joueur qui va jouer
    Grille& griJoueur = partie.joueurs[joueur].grilleDeJeu;
    
    // On tire la carte choisi par le coup
    Carte* cartePiochee = tierCartePioche(partie.pioche, indiceCartePioche);

    // Si le coup est de la mettre directement dans sa case face visible.
    if(typeCoup == 'd'){
        griJoueur[indiceDestGrille].faceVisible = cartePiochee;
    }
    // Si le coup est de choisir la carte choisie comme cachée face à celle qui était la avant
    else if(typeCoup == 'c'){
        // La carte face visible reste la même (celle qui était là avant)
        // On met la carte face cachée
        griJoueur[indiceDestGrille].faceCachee = cartePiochee;
    }
    // Si le coup est de choisir la carte choisie comme visible face à celle qui était la avant
    else if(typeCoup == 'v'){
        // La carte qui était visible avant devient cachée et on mets celle choisis en tant que visible
        griJoueur[indiceDestGrille].faceCachee = griJoueur[indiceDestGrille].faceVisible;
        griJoueur[indiceDestGrille].faceVisible = cartePiochee;
    }
    // Si le coup est de la mettre dans une autre case vide
    else{
        griJoueur[indiceDestGrille].faceCachee = cartePiochee;
    }

    // Si il n'y a pas que deux joueurs
    if(partie.nombreJoueurs != 2){
        if(estVidePioche(partie.pioche)){
            if(unJoueurAFinit(partie)){
                // La manche est finit si la pioche est vide et qu'un joueur a rempli sa grille
                partie.estMancheFinie = true;
                // On ajoute les points aux joueurs
                for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
                    ajoutePointsDeGrille(partie.joueurs[i], partie.numeroManche);
                }
            }
            else
                // On remplit la pioche si elle est vide et que personne n'a terminé
                remplirPioche(partie.pioche, partie.defausse);
        }
        else
            // Si la pioche n'est pas vide on passe au joueur suivant
            changerDeJoueur(partie);
    }
    // Si il y a que 2 joueurs
    else{
        // On regarde si un jouer à finit si il reste 2 ou 0 cartes
        if((partie.pioche.nombreCartesRestantes == 2 or partie.pioche.nombreCartesRestantes == 0) and unJoueurAFinit(partie)){
            partie.estMancheFinie = true;
            // On ajoute les points aux joueurs
            for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
                ajoutePointsDeGrille(partie.joueurs[i], partie.numeroManche);
            }
        } 
        // Sinon si il n'y a plus de cartes dans la pioche et que personne n'a finit
        else if(partie.pioche.nombreCartesRestantes == 0){
            remplirPioche(partie.pioche, partie.defausse);
        }
        // Si il reste des cartes dans la pioche et que personne n'a finit on passe au prochain joueur
        else
            changerDeJoueur(partie);
    }
    // On ajoute le coup au coups joués depuis le début de la manche
    ajouterCoup(partie.coupsManche, coup);
}

bool unJoueurAFinit(Partie partie){
    unsigned int i = 0;
    while(i < partie.nombreJoueurs and not finJeu(partie.joueurs[i].grilleDeJeu))
        ++i;
    return i != partie.nombreJoueurs;
}

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
    
    mettrePiocheDansDefausse(partie.pioche, partie.defausse);
}

void changerDeJoueur(Partie& partie){
    partie.prochainJoueur = (partie.prochainJoueur + 1)%partie.nombreJoueurs;
}

void remettreDernierJoueurCommeSuivant(Partie& partie){
    partie.prochainJoueur = (partie.prochainJoueur - 1)%partie.nombreJoueurs;
}

std::string recupDernierCoup(Partie partie){
    if(partie.coupsManche.nombre==0)
        return "NULL";
    else
        return partie.coupsManche.coups[partie.coupsManche.nombre-1];
}

void supprimeDernierCoup(ListeDeCoups& lc){
    --lc.nombre;
}

unsigned int joueurDuDernierCoup(Partie partie){
    std::string dernierCoup = recupDernierCoup(partie);
    if(dernierCoup == "NULL")
        return 10;
    else
        return dernierCoup[0] - '0';
}

unsigned int recupLeader(const Partie& partie){
    int leader = 0;
    for(unsigned int i = 1; i < partie.nombreJoueurs; ++i){
        if(partie.joueurs[i].nbPoints > partie.joueurs[leader].nbPoints)
            leader = i;
    }
    return leader;
}

void afficher(const Partie& partie){
    
    for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
        std::cout << "Grille de " << partie.joueurs[i].surnom << std::endl;
        afficherGrille(partie.joueurs[i].grilleDeJeu);
    }
    std::cout <<" Pioche :" << std::endl;
    afficher(partie.pioche);
}
