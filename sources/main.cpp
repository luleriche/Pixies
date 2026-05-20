#include "Ordinateur.hpp"
#include "Partie.hpp"
#include "Graphics.hpp"

int main(int argc, char **argv)
{
    srand(time(nullptr));
    if(argc == 2){
        Partie partie;
        lireFichierPartie(argv[1], partie);
        jouerCoupOrdiEtEcrire(partie, argv[1]);
        supprimerDefausse(partie.defausse);
        supprimerBoite(partie.boite);
    }
    else{
        char choix;
        std::cout << "Lancer le jeu dans une fenêtre graphique (y/n) : ";
        std::cin >> choix;
        if(choix == 'y')
            lancerJeu();
        else
            lancerUneNouvellePartie();
    }
    return 0;
}