#include <cstdlib>
#include <ctime>

#include "Partie.hpp"
#include "Console.hpp"
#include "Ordinateur.hpp"

void ajouterCoup(ListeDeCoupsManche& lc, std::string coup){
    lc.coups[lc.nombre] = coup;
    ++lc.nombre;
}

void ajouterCoup(ListeDeCoupsPossibles& lc, std::string coup){
    lc.coups[lc.nombre] = coup;
    ++lc.nombre;
}

void afficher(const ListeDeCoupsManche& lc){
    for(unsigned int i = 0; i < lc.nombre; ++i)
        std::cout << lc.coups[i] << std::endl;
}

void afficher(const ListeDeCoupsPossibles& lc){
    for(unsigned int i = 0; i < lc.nombre; ++i)
        std::cout << lc.coups[i] << std::endl;
}

void supprimeDernierCoup(ListeDeCoupsManche& lc){
    --lc.nombre;
}

void supprimeDernierCoup(ListeDeCoupsPossibles& lc){
    --lc.nombre;
}


void lancerPartieConsole(){
    // Création de la partie
    Partie partie;

    // Création des cartes de la boite
    creerCartesAvecFichier("assets/cartes_pixies.txt", partie.boite);

    // Initialisation, remplissage et mélange de la défausse
    initDefausse(partie.defausse);
    remplir(partie.boite, partie.defausse);
    melanger(partie.defausse);

    // On demande à l'utilisateur le nombre de joueurs à la partie.
    std::cout << "Le jeu se joue de 2 à 5 joueurs !" << std::endl << "Nombre de joueurs: ";
    std::cin >> partie.nombreJoueurs;
    while(partie.nombreJoueurs > 5 or partie.nombreJoueurs < 2){
        std::cout << "Impossible. Le jeu se joue de 2 à 5 joueurs ! Nombre de joueurs: ";
        std::cin >> partie.nombreJoueurs;
    }
    creerJoueurs(partie.joueurs, partie.nombreJoueurs, &partie.pioche);
    
    // On initialise la pioche
    if(partie.nombreJoueurs == 2)
        initPioche(partie.pioche, 4);
    else
        initPioche(partie.pioche, partie.nombreJoueurs);
    
    // Génération aléatoire du premier joueur.
    partie.prochainJoueur = rand()%partie.nombreJoueurs;
    std::cout << "Le premier joueur sera " << partie.joueurs[partie.prochainJoueur].surnom << std::endl;

    // Manche 1
    lancerManche(partie, 1);

    toutRemettreDansDefausse(partie);
    melanger(partie.defausse);
    
    // Manche 2
    lancerManche(partie, 2);

    toutRemettreDansDefausse(partie);
    melanger(partie.defausse);
    ++partie.numeroManche;

    // Manche 3
    lancerManche(partie, 3);

    std::cout << "La partie est terminée." << std::endl;
    toutRemettreDansDefausse(partie);
    supprimerDefausse(partie.defausse);
    supprimerBoite(partie.boite);
}

void lancerManche(Partie& partie, const unsigned int numeroManche){
    partie.numeroManche = numeroManche;
    partie.estMancheFinie = false;
    partie.coupsManche.nombre = 0;
    remplirPioche(partie.pioche, partie.defausse);

    std::cout << "DEBUT DE LA MANCHE " << partie.numeroManche << std::endl;
    
    while(not partie.estMancheFinie){
        faireJouerProchain(partie); 
    }

    std::cout << "Manche Terminée. Voici les points désormais.:" << std::endl;
    for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
        std::cout << partie.joueurs[i].surnom << " : " << partie.joueurs[i].nbPoints << " points." << std::endl;
    }
}

void faireJouerProchain(Partie& partie){
    std::cout << "Tour de " << partie.joueurs[partie.prochainJoueur].surnom << std::endl;
    std::string coup;
    // Si le prochain joueur est un ordinateur
    if(partie.joueurs[partie.prochainJoueur].estOrdi){
        std::cout << "L'ordinateur choisi son coup..." << std::endl;
        coup = recupMeilleurCoup(partie, 20, 300); // On trouve son coup grâce à un arbre de recherche
    }else{
        coup = demanderCoupJoueur(partie); // Sinon on demande dans la console à l'utilisateur d'entrer son coup
    }
    // On joue le coup et on affiche les grilles
    jouerCoup(partie, coup);
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
        while(choix < 1 or choix > 9 or griJoueur[choix-1].faceCachee != nullptr or griJoueur[choix-1].faceVisible != nullptr){
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
            // Si un joueur a finit ou si la défausse est vide, ce qui peut arriver à 5 joueurs, la partie s'arrête
            if(unJoueurAFinit(partie) or partie.defausse == nullptr){
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

bool unJoueurAFinit(const Partie& partie){
    // On parcout les joueurs et on s'arrête si il y en un qui a aucun emplacement vide
    unsigned int i = 0;
    while(i < partie.nombreJoueurs and not finJeu(partie.joueurs[i].grilleDeJeu))
        ++i;
    return i != partie.nombreJoueurs;
}

void toutRemettreDansDefausse(Partie& partie){
    // Parcours des grilles des joueurs
    for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
        // Parcours de chaque emplacement de sa grille
        for(unsigned int j = 0; j < 9; ++j){
            // On met les cartes dans la defausse et on vide la grille
            ajoutDebutDefausse(partie.defausse, partie.joueurs[i].grilleDeJeu[j].faceCachee);
            partie.joueurs[i].grilleDeJeu[j].faceCachee = nullptr;
            ajoutDebutDefausse(partie.defausse, partie.joueurs[i].grilleDeJeu[j].faceVisible);
            partie.joueurs[i].grilleDeJeu[j].faceVisible = nullptr;
        }
    }
    // On met aussi les cartes de la pioche dans la défausse
    mettrePiocheDansDefausse(partie.pioche, partie.defausse);
}

void changerDeJoueur(Partie& partie){
    partie.prochainJoueur = (partie.prochainJoueur + 1)%partie.nombreJoueurs;
}

std::string recupDernierCoup(const Partie& partie){
    if(partie.coupsManche.nombre > 0) // Si il y a au moins un coup dans la liste de ceux joué
        return partie.coupsManche.coups[partie.coupsManche.nombre-1]; // On revoie le dernier
    else 
        return "NULL";
}

unsigned int joueurDuDernierCoup(const Partie& partie){
    std::string dernierCoup = recupDernierCoup(partie); // On récupère le dernier coups joué dans une manche
    if(dernierCoup == "NULL")
        return 10;
    else
        return dernierCoup[0] - '0'; // Si il y en a un, le premier caractère est le joueur qui l'a joué.
}

unsigned int recupLeader(const Partie& partie){
    int leader = 0; // On parcours les joueurs
    for(unsigned int i = 1; i < partie.nombreJoueurs; ++i){
        if(partie.joueurs[i].nbPoints > partie.joueurs[leader].nbPoints)
            leader = i; // On garde l'indice de celui qui a le plus de points
    }
    return leader;
}

void afficher(const Partie& partie){
    // On affiche les grilles de tous les joueurs
    for(unsigned int i = 0; i < partie.nombreJoueurs; ++i){
        std::cout << "Grille de " << partie.joueurs[i].surnom << std::endl;
        afficherGrille(partie.joueurs[i].grilleDeJeu);
    }
    // Et on affiche la pioche
    std::cout <<" Pioche :" << std::endl;
    afficher(partie.pioche);
}


