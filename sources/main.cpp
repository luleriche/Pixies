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
    else if(argc == 1){
        char choix;
        std::cout << "Lancer le jeu dans une fenêtre graphique (y/n) : ";
        std::cin >> choix;
        if(choix == 'y')
            lancerJeu();
        else
            lancerPartieConsole();
    }
    // Si il y a trois agruments
    else{
        // On répète 50 fois
        for(unsigned int i = 0; i < 50; ++i){
            // On écrit une partie aléatoire dans le fichier
            ecrirePartieAlea("test"+std::to_string(i)+".txt");
            // On simule une lecture de cette partie et l'écriture d'un coup
            Partie partie;
            lireFichierPartie("test"+std::to_string(i)+".txt", partie);
            afficher(partie);
            jouerCoupOrdiEtEcrire(partie, "test"+std::to_string(i)+".txt");
            supprimerDefausse(partie.defausse);
            supprimerBoite(partie.boite);
        }
        
    }
    return 0;
}