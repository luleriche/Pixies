# COMMENT JOUER A PIXIES

## Règles du jeu.
L'objectif premier de ce jeu est de finir la partie avec le plus de points. 
Pour cela placer intelligemment vos cartes dans votre grille de jeu. 
Celles-ci vous rapportent des points en fonction de leur chiffre, de leur couleur et des symboles dessus (spirales ou croix).

Les règles du jeu sont disponibles en détail ici -> [Règles du Pixies](https://www.nos-jeux-de-societe.fr/wp-content/uploads/2024/03/PIXIES-regles-du-jeu-FR-www.nos-jeux-de-societe.fr_.pdf).

__Déroulement d'une manche__
Une manche est divisée en plusieurs tours de jeu, et se termine à la fin d'un tour si l'un des joueurs n'a plus aucun emplacement vide dans sa zone.
Lors d'un tour de jeu : 
1. Le premier joueur révèle autant de cartes de la défausse qu'il y a de joueurs.
2. Chaque personne prend alors une de ces cartes et la met dans sa zone selon des règles décrites plus bas. Cela en tournant dans le sens inverse des aiguilles d'une montre.
3. Le tour est terminé quand toutes les cartes ont été prises, c'est alors au dernier joueur de commencer le prochain tour.
Attention, si vous jouez à deux, il faut tirer 4 cartes de la défausse et chaque personne joue alors deux fois.

__Règle de placement__
Votre grille de jeu se compose de 9 emplacement numérotés de 1 à 9 formant une grille de 3 par 3. 
Lorsque vous choisissez une carte, plusieurs cas sont possibles :
1. L'emplacement indiqué par le chiffre de la carte ne contient pas de carte visible, dans ce cas mettez la face visible à cet endroit
2. Ce même emplacement ne contient que une carte face visible, c'est à vous de choisir laquelle des deux reste visible .L'autre sera placée face caché en dessous, l'emplacement sera désormais dit validé et plus aucune carte ne pourra y aller.
3. L'emplacement où doit aller la carte est déja validé, càd qu'il contient déjà deux cartes, dans ce cas placé la carte tirée face cachée à un emplacement vide. 

__Fin d'une manche__
Une manche se termine à la fin d'un tour si l'un des joueurs n'a aucun emplacement vide dans sa grille.
Si vous jouez à deux, la manche peut aussi se terminer si il reste deux cartes à choisir.
Il ne vous reste plus qu'à compter les points sur votre grille et à les ajouter à votre compteur.

__Règles de décompte des points__
1. Comptez d'abord les spirales et les croix. Une spirale vous rajoute un point tandis qu'une croix vous en retire un. Les cartes spéciales qui affichent une spirale suivie d'une couleur vous rapportent un point par carte de la couleur indiquée.
2. Comptez ensuite les points de validation. Chaque emplacement validé vous rapporte autant de points que le chiffre indiqué dessus.
3. Enfin, rajouter les points de zone. Une zone de couleurs est formée par au moins deux couleurs qui sont côte à côte (pas en diagonale). Chaque carte de votre plus grande zone vous rapporte 2 points à la première manche, 3 à la seconde et 4 à la dernière manche.

Les cartes spéciales peuvent faire partie de n'importe quelle zone de couleur et compte aussi pour les cartes à spirale spéciale.



## Comment compiler le projet
### Prérequis 
1. Un des __compilateur C++__ ci-dessous, pour être compatible avec SFML 3.0.2

Compilateur | Version
------------|-------------
MSVC        | 16 (VS 2019)
GCC         | 9
Clang       | 9
AppleClang  | 12

2. __Cmake__ dans sa version 3.18 ou plus récent.

### Guide de compilation
1. Cloner le dépot git sur votre ordinateur.
2. Créer un dossier build dans le dépot.
3. Si vous êtes sur Windows. Débrouillez vous pour compiler à l'aide de CMake
4. SI vous êtes sur Linux et MacOS, placer vous dans le dossier build et entrez ces commandes dans la console.
    ```bash
    cmake ..
    make
    ./Pixies.out
    ```

### Prise en main
Au début de l'exécution, suivez les instructions. Vous n'aurez pas à utiliser votre souris.