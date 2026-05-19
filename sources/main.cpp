#include "Ordinateur.hpp"
#include "Partie.hpp"

int main(int argc, char **argv)
{
    srand(time(nullptr));
    //lancerUneNouvellePartie();
    //lancerJeu();
    if(argc == 2){
        Partie partie;

        //Initialisation de la partie avant la lecture dans le fichier
        partie.nombreJoueurs = 5;

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
        partie.pioche.taille = 5;

        // Creation des joueurs
        for(unsigned int i = 0; i < 5; ++i){
            partie.joueurs[i].surnom = std::to_string(i);
            initGrille(partie.joueurs[i].grilleDeJeu);
            partie.joueurs[i].piocheCommune = &partie.pioche;
            partie.joueurs[i].nbPoints = 0;
            partie.joueurs[i].estOrdi = false;
        }

        lireFichierPartie(argv[1], partie);
    }
    else{

    }
    return 0;
}