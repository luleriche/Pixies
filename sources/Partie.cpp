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
    remplirPioche(pioche, defausse, nbJoueurs);

    creerJoueurs(partie.joueurs, nbJoueurs, &pioche);

    partie.joueurActuel = rand()%nbJoueurs;
}

void commencerPartie(Partie p);