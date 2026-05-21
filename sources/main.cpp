#include "Ordinateur.hpp"
#include "Partie.hpp"
#include "Graphics.hpp"

int main(int argc, char **argv)
{
    srand(time(nullptr));
    if(argc == 2){
        // On créer un partie
        Partie partie;
        lireFichierPartie(argv[1], partie); // On lit la partie du fichier
        jouerCoupOrdiEtEcrire(partie, argv[1]); // On écrit le coup dans le fichier
        supprimerDefausse(partie.defausse); // On désalloue les emplacements mémoire
        supprimerBoite(partie.boite);
    }
    else if(argc == 1){
        char choix;
        std::cout << "Lancer le jeu dans une fenêtre graphique (y/n) : ";
        std::cin >> choix;
        if(choix == 'y')
            lancerJeu();
        else
            lancerPartieConsole();
    }
    return 0;
}