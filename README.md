Commande pour compiler sous Linux :
g++ main.cpp -o main -std=c++17 -I SFML-3.0.2/include -L SFML-3.0.2/lib -Wl,-rpath,$(pwd)/SFML-3.0.2/lib -lsfml-graphics -lsfml-window -lsfml-system && ./main

Commande pour compiler sous MAcOS :
g++ main.cpp -o main -std=c++17 -I SFML-3.0.2-macos/include -L SFML-3.0.2-macos/lib -Wl,-rpath,$(pwd)/SFML-3.0.2-macos/lib -lsfml-graphics -lsfml-window -lsfml-system && ./main
Autoriser les fichier dans confidentialiter 