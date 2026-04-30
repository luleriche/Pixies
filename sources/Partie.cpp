#include <cstdlib>
#include <ctime>
#include "Partie.hpp"

void nouvellePartie(Partie& partie, unsigned int nbJoueurs){
    partie.nombreJoueurs = nbJoueurs;
    partie.numeroManche = 1;

    Defausse defausse;
    initDefausse(defausse);
    lireFichierDefausse("assets/cartes_pixies.txt", defausse);
    partie.defausse = defausse;

    Pioche pioche;
    initPioche(pioche);
    
    creerJoueurs(partie.joueurs, nbJoueurs, &pioche);

    partie.joueurActuel = rand()%nbJoueurs;

    bool unJoueurFinit = false;
    while(not unJoueurFinit){
        remplirPioche(pioche, defausse, nbJoueurs);
        for(unsigned int i = 0; i < nbJoueurs; ++i){
            afficherGrille(partie.joueurs[partie.joueurActuel].grilleDeJeu);
            Carte* cartePiochee = prendrePioche(pioche);
            ajouterCarte(partie.joueurs[partie.joueurActuel].grilleDeJeu, cartePiochee);
            afficherGrille(partie.joueurs[partie.joueurActuel].grilleDeJeu);
            if(finJeu(partie.joueurs[partie.joueurActuel].grilleDeJeu))
                unJoueurFinit = true;
            changerDeJoueur(partie.joueurActuel, partie.nombreJoueurs);
        }
    }
}