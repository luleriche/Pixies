# Comment jouer à Pixies !

## Prérequis
* Windows
    + GCC 14.2.0 MinGW (DW2) (UCRT) - [Téléchargeable ici](https://github.com/brechtsanders/winlibs_mingw/releases/download/14.2.0posix-19.1.1-12.0.0-ucrt-r2/winlibs-i686-posix-dwarf-gcc-14.2.0-mingw-w64ucrt-12.0.0-r2.7z)

## Guide d'installation
1. Cloner le dépot sur votre ordinateur.
2. Créer un dossier build dans le dépot
3. Lancer le jeu avec les commandes suivantes :
    * Sur Windows
        ```cmd 
        cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER="chemin\vers\gcc.exe" -DCMAKE_CXX_COMPILER="chemin\vers\gcc.exe\g++.exe" ..
        mingw32-make
        Pixies.Exe
        ```
   * Sur Linux et MacOS
        ```bash
        cmake ..
        make
        ./Pixies.out
        ```