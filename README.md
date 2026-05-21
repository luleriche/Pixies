# Comment jouer à Pixies

## Règles du jeu
L'objectif premier de ce jeu est de finir la partie avec le plus de points. 
Pour cela placer intelligemment vos cartes dans votre grille de jeu. 
Celles-ci vous rapportent des points en fonction de leur chiffre, de leur couleur et des symboles dessus (spirales ou croix).

### Déroulement d'une manche
Une manche est divisée en plusieurs tours de jeu, et se termine à la fin d'un tour si l'un des joueurs n'a plus aucun emplacement vide dans sa zone.
Lors d'un tour de jeu : 
1. Le premier joueur révèle autant de cartes de la défausse qu'il y a de joueurs.
2. Chaque personne prend alors une de ces cartes et la met dans sa zone selon des règles décrites plus bas. Cela en tournant dans le sens inverse des aiguilles d'une montre.
3. Le tour est terminé quand toutes les cartes ont été prises, c'est alors au dernier joueur de commencer le prochain tour.
Attention, si vous jouez à deux, il faut tirer 4 cartes de la défausse et chaque personne joue alors deux fois.

### Règle de placement
Votre grille de jeu se compose de 9 emplacement numérotés de 1 à 9 formant une grille de 3 par 3. 
Lorsque vous choisissez une carte, plusieurs cas sont possibles :
1. L'emplacement indiqué par le chiffre de la carte ne contient pas de carte visible, dans ce cas mettez la face visible à cet endroit
2. Ce même emplacement ne contient que une carte face visible, c'est à vous de choisir laquelle des deux reste visible .L'autre sera placée face caché en dessous, l'emplacement sera désormais dit validé et plus aucune carte ne pourra y aller.
3. L'emplacement où doit aller la carte est déja validé, càd qu'il contient déjà deux cartes, dans ce cas placé la carte tirée face cachée à un emplacement vide. 

### Fin d'une manche
Une manche se termine à la fin d'un tour si l'un des joueurs n'a aucun emplacement vide dans sa grille.
Si vous jouez à deux, la manche peut aussi se terminer si il reste deux cartes à choisir.
Il ne vous reste plus qu'à compter les points sur votre grille et à les ajouter à votre compteur.

### Règles de décompte des points
1. Comptez d'abord les spirales et les croix. Une spirale vous rajoute un point tandis qu'une croix vous en retire un. Les cartes spéciales qui affichent une spirale suivie d'une couleur vous rapportent un point par carte de la couleur indiquée.
2. Comptez ensuite les points de validation. Chaque emplacement validé vous rapporte le nombre de points

## Prérequis
* Windows
    + GCC 14.2.0 MinGW (DW2) (UCRT) - [Téléchargeable ici](https://github.com/brechtsanders/winlibs_mingw/releases/download/14.2.0posix-19.1.1-12.0.0-ucrt-r2/winlibs-i686-posix-dwarf-gcc-14.2.0-mingw-w64ucrt-12.0.0-r2.7z)

## Guide d'installation
1. Cloner le dépot sur votre ordinateur.
2. Créer un dossier build dans le dépot
3. Pour lancer le jeu :
    * Sur Windows
        Copier les fichiers suivants qui se trouvent au même endroit que votre compilateur dans le dossier build :
            - libgcc_s_dw2-1.dll
            - libstdc++-6.dll
            - libwinpthread-1.dll
        Entrez les commande suivande dans le cmd en étant dans le dossier build
        ```cmd 
        cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER="chemin\vers\gcc.exe" -DCMAKE_CXX_COMPILER="chemin\vers\gcc.exe\g++.exe" -DCMAKE_BUILD_TYPE=Release ..
        mingw32-make
        Pixies.Exe
        ```
   * Sur Linux et MacOS
        ```bash
        cmake ..
        make
        ./Pixies.out
        ```