# Comment jouer à Pixies !

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