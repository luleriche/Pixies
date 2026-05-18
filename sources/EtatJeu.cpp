#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "EtatJeu.hpp"
#include "Partie.hpp"
#include "Carte.hpp"
#include "Defausse.hpp"
#include "Pioche.hpp"
#include "Joueur.hpp"
#include "Ordinateur.hpp"
#include "Console.hpp"

static std::ofstream ficIA;
static bool ficIAOuverte = false;

// Écriture d'une carte dans le fichier de sauvegarde (format : chiffre couleur points estSpeciale)
static void ecrireCarte(const Carte* c) {
    if (c == nullptr) return;
    int estSpeciale = (c->spirale == 9) ? 1 : 0;
    int points = estSpeciale ? 0 : c->spirale;
    ficIA << c->chiffre << " " << c->couleur << " " << points << " " << estSpeciale;
}

// Écriture d'une carte dans un flux quelconque
static void ecrireCarteStream(std::ostream& out, const Carte* c) {
    if (c == nullptr) return;
    int estSpeciale = (c->spirale == 9) ? 1 : 0;
    int points = estSpeciale ? 0 : c->spirale;
    out << c->chiffre << " " << c->couleur << " " << points << " " << estSpeciale;
}

// Ouverture du fichier de sauvegarde
void ouvrirFichierIA(const std::string& nomFichier, const Partie& partie) {
    ficIA.open(nomFichier);
    if (!ficIA.is_open()) {
        std::cout << "Erreur : impossible d'ouvrir " << nomFichier << std::endl;
        return;
    }
    ficIAOuverte = true;
}

// Écriture du numéro de manche et des scores des 5 joueurs
void ecrireDebutManche(const Partie& partie) {
    if (!ficIAOuverte) return;
    ficIA << partie.numeroManche << std::endl;
    for (unsigned int i = 0; i < 5; ++i) {
        if (i < partie.nombreJoueurs)
            ficIA << partie.joueurs[i].nbPoints;
        else
            ficIA << 0;
        ficIA << std::endl;
    }
}

// Écriture des cartes disponibles dans la pioche
void ecrireCartesDisponibles(const Partie& partie) {
    if (!ficIAOuverte) return;
    for (unsigned int i = 0; i < partie.pioche.taille; ++i) {
        if (partie.pioche.cartes[i] != nullptr) {
            ecrireCarte(partie.pioche.cartes[i]);
            ficIA << std::endl;
        }
    }
}

// Écriture d'un coup joué (format : numero_joueur chiffre couleur points estSpeciale emplacement c/v)
void ecrireCoupJoue(const Partie& partie, const std::string& coup) {
    if (!ficIAOuverte) return;

    unsigned int joueur = coup[0] - '0';
    unsigned int idxGrille = coup[3] - '0';
    char type = coup[2];

    const Carte* c = nullptr;
    if (type == 'd' || type == 'v')
        c = partie.joueurs[joueur].grilleDeJeu[idxGrille].faceVisible;
    else
        c = partie.joueurs[joueur].grilleDeJeu[idxGrille].faceCachee;

    ficIA << (joueur + 1) << " ";
    ecrireCarte(c);
    ficIA << " " << (idxGrille + 1) << " ";
    ficIA << ((type == 'c' || type == 'm') ? 'c' : 'v');
    ficIA << std::endl;
}

// Fermeture du fichier de sauvegarde
void fermerFichierIA() {
    if (!ficIAOuverte) return;
    ficIAOuverte = false;
    ficIA.close();
}

// Recherche d'une carte dans la boite selon ses caractéristiques (ignore les cartes déjà utilisées)
static Carte* trouverCarte(BoiteCartes& boite, unsigned int nbCartes,
                            unsigned int chiffre, char couleur, int points, int estSpeciale) {
    int spirale = estSpeciale ? 9 : points;
    for (unsigned int i = 0; i < nbCartes; ++i) {
        if (boite[i].chiffre != 0 &&
            boite[i].chiffre == chiffre &&
            boite[i].couleur == couleur &&
            boite[i].spirale == spirale)
            return &boite[i];
    }
    return nullptr;
}

// Reconstruction du coup interne depuis les données lues dans le fichier
static std::string construireCoup(unsigned int joueur, Carte* carte,
                                   unsigned int emplacement, char cv,
                                   const Partie& partie) {
    unsigned int idxGrille = emplacement - 1;
    unsigned int idxPioche = 0;

    // Recherche de l'indice de la carte dans la pioche
    for (unsigned int i = 0; i < partie.pioche.taille; ++i) {
        if (partie.pioche.cartes[i] == carte) {
            idxPioche = i;
            break;
        }
    }

    const Grille& g = partie.joueurs[joueur].grilleDeJeu;
    char type;

    // Détermination du type de coup selon l'état de la case destination
    if (g[idxGrille].faceVisible == nullptr) {
        type = 'd';
    } else if (g[idxGrille].faceCachee == nullptr) {
        type = (cv == 'v') ? 'v' : 'c';
    } else {
        type = 'm';
    }

    return std::to_string(joueur) + std::to_string(idxPioche) + type + std::to_string(idxGrille);
}

// Lecture du fichier d'état, reconstruction de la partie et calcul du meilleur coup
void jouerCoupDepuisFichier(const std::string& nomFichier) {
    std::ifstream fic(nomFichier);
    if (!fic.is_open()) {
        std::cout << "Impossible d'ouvrir " << nomFichier << std::endl;
        return;
    }

    // Chargement des cartes depuis le fichier de configuration
    BoiteCartes boite;
    unsigned int nbCartes;
    creerCartesAvecFichier("assets/cartes_pixies.txt", boite, nbCartes);

    // Initialisation de la partie
    Partie partie;
    partie.nombreJoueurs = 5;
    initPioche(partie.pioche, 5);
    initDefausse(partie.defausse);
    partie.estMancheFinie = false;
    partie.coupsManche.nombre = 0;
    partie.pioche.taille = 5;

    for (unsigned int i = 0; i < partie.nombreJoueurs; ++i) {
        initJoueur(partie.joueurs[i], &partie.pioche, "Joueur" + std::to_string(i+1), false);
        partie.joueurs[i].nbPoints = 0;
        initGrille(partie.joueurs[i].grilleDeJeu);
    }

    std::string ligne;
    unsigned int prochainJoueur = 0;

    // Lecture ligne par ligne du fichier
    while (std::getline(fic, ligne)) {
        if (ligne.empty()) continue;

        // Détection du numéro de manche (entier seul entre 1 et 3)
        std::istringstream test(ligne);
        unsigned int val;
        std::string reste;
        if (test >> val && !(test >> reste) && val >= 1 && val <= 3) {
            partie.numeroManche = val;
            // Lecture des 5 scores
            for (unsigned int i = 0; i < 5; ++i) {
                std::string scoreLigne;
                if (std::getline(fic, scoreLigne)) {
                    std::istringstream ss(scoreLigne);
                    int score;
                    if (ss >> score && i < partie.nombreJoueurs)
                        partie.joueurs[i].nbPoints = score;
                }
            }
            // Réinitialisation des grilles pour la nouvelle manche
            for (unsigned int i = 0; i < partie.nombreJoueurs; ++i)
                initGrille(partie.joueurs[i].grilleDeJeu);
            prochainJoueur = 0;
            continue;
        }

        std::istringstream ssLigne(ligne);
        unsigned int chiffre;
        char couleur;
        int points;
        int estSpeciale;

        if (ssLigne >> chiffre >> couleur >> points >> estSpeciale) {
            unsigned int emplacement;
            char cv;
            std::istringstream ssCoup(ligne);
            unsigned int joueurFichier;
            unsigned int chiffre2; char couleur2; int points2; int estSpeciale2;

            // Détection d'un coup joué
            if (ssCoup >> joueurFichier >> chiffre2 >> couleur2 >> points2 >> estSpeciale2 >> emplacement >> cv
                && joueurFichier >= 1 && joueurFichier <= 5) {
                unsigned int joueur = joueurFichier - 1;
                Carte* carte = trouverCarte(boite, nbCartes, chiffre2, couleur2, points2, estSpeciale2);
                if (carte != nullptr) {
                    std::string coup = construireCoup(joueur, carte, emplacement, cv, partie);
                    jouerCoup(partie, coup);

                    // Marquage de la carte comme utilisée
                    for (unsigned int k = 0; k < nbCartes; ++k) {
                        if (&boite[k] == carte) {
                            boite[k].chiffre = 0;
                            break;
                        }
                    }

                    prochainJoueur = (joueur + 1) % partie.nombreJoueurs;
                }
            } else {
                // Détection d'un nouveau chargement de pioche (5 cartes)
                for (unsigned int i = 0; i < partie.pioche.taille; ++i)
                    partie.pioche.cartes[i] = nullptr;

                // Première carte déjà lue
                Carte* c = trouverCarte(boite, nbCartes, chiffre, couleur, points, estSpeciale);
                if (c != nullptr) partie.pioche.cartes[0] = c;

                // Lecture des 4 cartes suivantes
                for (unsigned int i = 1; i < partie.pioche.taille; ++i) {
                    std::string carteLigne;
                    if (std::getline(fic, carteLigne) && !carteLigne.empty()) {
                        std::istringstream sc(carteLigne);
                        unsigned int ch; char col; int pts; int spec;
                        if (sc >> ch >> col >> pts >> spec) {
                            Carte* ci = trouverCarte(boite, nbCartes, ch, col, pts, spec);
                            partie.pioche.cartes[i] = ci;
                        }
                    }
                }

                // Mise à jour du nombre de cartes restantes
                partie.pioche.nombreCartesRestantes = 0;
                for (unsigned int i = 0; i < partie.pioche.taille; ++i) {
                    if (partie.pioche.cartes[i] != nullptr)
                        ++partie.pioche.nombreCartesRestantes;
                }
            }
        }
    }

    fic.close();

    // Mise à jour du joueur courant et réinitialisation de l'état
    prochainJoueur = partie.prochainJoueur;
    partie.coupsManche.nombre = 0;
    partie.estMancheFinie = false;
    std::cout << "prochainJoueur = " << prochainJoueur << std::endl;
    std::cout << "C'est notre tour (joueur " << prochainJoueur + 1 << "). Calcul du meilleur coup..." << std::endl;

    // Activation du mode IA pour le joueur courant
    partie.joueurs[prochainJoueur].estOrdi = true;

    // Calcul du meilleur coup possible
    std::string meilleurCoup = recupMeilleurCoup(partie, 20, 300);
    std::cout << "Meilleur coup : " << meilleurCoup << std::endl;

    // Décodage du coup
    unsigned int joueur = meilleurCoup[0] - '0';
    char type = meilleurCoup[2];
    unsigned int idxGrille = meilleurCoup[3] - '0';

    // Application du coup dans la partie
    jouerCoup(partie, meilleurCoup);

    // Récupération de la carte jouée depuis la grille
    const Carte* c = nullptr;
    if (type == 'd' || type == 'v')
        c = partie.joueurs[joueur].grilleDeJeu[idxGrille].faceVisible;
    else
        c = partie.joueurs[joueur].grilleDeJeu[idxGrille].faceCachee;

    // Écriture du coup à la fin du fichier
    std::ofstream fout(nomFichier, std::ios::app);
    fout << (joueur + 1) << " ";
    ecrireCarteStream(fout, c);
    fout << " " << (idxGrille + 1) << " ";
    fout << ((type == 'c' || type == 'm') ? 'c' : 'v');
    fout << std::endl;
    fout.close();

    // Libération de la mémoire
    supprimerBoite(boite);
    std::cout << "Coup écrit dans le fichier." << std::endl;
}