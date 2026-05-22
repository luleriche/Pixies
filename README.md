# COMMENT JOUER A PIXIES

## Règles du jeu
L'objectif premier de ce jeu est de finir la partie avec le plus de points. 
Pour cela placez intelligemment vos cartes dans votre grille de jeu. 
Celles-ci vous rapportent des points en fonction de leur chiffre, de leur couleur et des symboles présents dessus.

Les règles du jeu sont disponibles en détail ici -> [Règles du Pixies](https://www.nos-jeux-de-societe.fr/wp-content/uploads/2024/03/PIXIES-regles-du-jeu-FR-www.nos-jeux-de-societe.fr_.pdf).

__Déroulement d'une manche__

Une manche est divisée en plusieurs tours de jeu, et se termine à la fin d'un tour si l'un des joueurs n'a plus aucun emplacement vide dans sa zone.

__Lors d'un tour de jeu__ 

1. Le premier joueur pioche autant de cartes qu'il y a de joueurs, et les pose face visble sur la table.
2. Chaque personne prend alors une de ces cartes et la met dans sa zone selon des règles décrites plus bas. Cela en tournant dans le sens inverse des aiguilles d'une montre.
3. Le tour est terminé quand toutes les cartes ont été prises, c'est alors au dernier joueur de commencer le prochain tour.

_Attention, si vous jouez à deux, il faut tirer 4 cartes de la défausse et chaque personne joue alors deux fois._

__Règles de placement__

Votre grille de jeu se compose de 9 emplacements numérotés de 1 à 9 formant une grille de 3 par 3. 
Lorsque vous choisissez une carte, plusieurs cas sont possibles :
1. L'emplacement indiqué par le chiffre de la carte ne contient pas de carte visible, dans ce cas mettez la face visible à cet endroit
2. L'emplacement indiqué par le chiffre de la carte contient seulement une carte face visible, c'est à vous de choisir laquelle des deux reste visible. L'autre sera placée face cachée en dessous, l'emplacement sera désormais dit validé et plus aucune carte ne pourra y aller.
3. L'emplacement où doit aller la carte est déjà validé, c'est-à-dire qu'il contient déjà deux cartes, dans ce cas,placez la carte tirée face cachée à l'emplacement vide de votre choix. 

__Fin d'une manche__

Une manche se termine à la fin d'un tour si l'un des joueurs n'a plus aucun emplacement vide dans sa grille.
Si vous jouez à deux, la manche peut aussi se terminer s'il reste deux cartes à choisir.
Il ne vous reste plus qu'à compter les points sur votre grille et à les ajouter à votre compteur.

__Règles de décompte des points__
1. Comptez d'abord les spirales et les croix. Une spirale vous rajoute un point tandis qu'une croix vous en retire un. Les cartes spéciales qui affichent une spirale suivie d'une couleur vous rapportent un point par carte de la couleur indiquée.
2. Comptez ensuite les points de validation. Chaque emplacement validé vous rapporte autant de points que le chiffre indiqué dessus.
3. Enfin, ajoutez les points de zone. Une zone de couleur est formée par au moins deux cartes de la même couleur qui sont côte à côte (pas en diagonale). Chaque carte de votre plus grande zone vous rapporte 2 points à la première manche, 3 à la seconde et 4 à la dernière manche.

_Les cartes spéciales peuvent faire partie de n'importe quelle zone de couleur et comptent aussi pour les cartes à spirale spéciale._


# Comment compiler le projet

* __Le projet a été conçu pour se compiler sur Linux, vous pourriez faire face à des problèmes de compatibilité si vous êtes sur un autre système d'exploitation. Cela reste plus facile sur macOS que sur Windows.__

### Prérequis 
1. L'un des __compilateurs C++__ ci-dessous, pour être compatible avec SFML 3, sur Linux utilisez GCC 64 bits.

Compilateur | Version minimum
------------|-------------
MSVC        | 16 (VS 2019)
GCC         | 9
Clang       | 9
AppleClang  | 12

2. __Cmake__ dans sa version 3.18 ou plus récente.

3. Avoir téléchargé et décompressé SFML 3.0.2 pour Linux accessible via [ce lien](https://www.sfml-dev.org/download/sfml/3.0.2/#linux)

3. SFML peut aussi avoir besoin de certaines dépendences externes. Pour voir la liste de ce que vous devez avoir d'installé [cliquez ici](https://www.sfml-dev.org/faq/general/#dependencies).

### Guide de compilation
1. Cloner le dépôt git sur votre ordinateur.
   ```bash
    git clone https://github.com/luleriche/Pixies
    cd Pixies
    ```

2. Créer un dossier "Graphics" dans le dépôt. Mettez dedans le dossier SFML 3.0.2 téléchargé précédemment. Renommer le dossier de SFML "SFML-3.0.2" si vous êtes sur Linux, si vous êtes sur macOS "SFML-3.0.2-macos".

3. Créer un dossier "build" dans le dépôt. Aller dedans.
   ```bash
    mkdir build
    cd build
    ```
4. Générer les fichiers de configuration.
    ```bash
    cmake ..
    ```
5. Compiler le projet.
    ```bash
    make
    ```
6. Lancer votre exécutable.
    ```bash
    ./Pixies
    ```

### Prise en main
Au début de l'exécution, suivez les instructions. Vous n'aurez pas à utiliser votre souris.
Pour déplacer le sélecteur utilisez les flèches, et pour valider votre choix appuyez sur Entrée.

## Extras
Ajouter en argument le nom d'un fichier de partie (qui doit se trouver aux côtés de l'executable) pour y ajouter le meilleur coup à celui-ci.
__Attention l'exécutable doit toujours avoir à ses côtés le dossier assets pour lire les cartes du jeu.__
