cmake_minimum_required(VERSION 3.18)
 
# Nom du projet
project(Pixies)

# On change les chemins de SFML selon la plateforme de compilation
if(APPLE)
    message(STATUS "Compilation sur MacOs")
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Graphics/SFML-3.0.2-macos/lib/cmake/SFML")
    set(SFML_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Graphics/SFML-3.0.2-macos/lib")
elseif(UNIX AND NOT APPLE)
    message(STATUS "Compilation sur Linux")
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Graphics/SFML-3.0.2/lib/cmake/SFML")
    set(SFML_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Graphics/SFML-3.0.2/lib")
elseif(WIN32)
    message(STATUS "Compilation sur Windows")
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Graphics/SFML-3.0.2-Win/lib/cmake/SFML")
    set(SFML_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Graphics/SFML-3.0.2-Win/lib")
endif()

# On va chercher le fichier SFMLConfig.cmake 
find_package(SFML 3.0.2 REQUIRED COMPONENTS Graphics)

add_compile_options(-Wall)

set(SOURCES 
    sources/main.cpp 
    sources/Carte.cpp
    sources/Grille.cpp
    sources/Defausse.cpp
    sources/Pioche.cpp
    sources/Joueur.cpp
    sources/Partie.cpp
    sources/Console.cpp
    sources/Ordinateur.cpp
    sources/Graphics.cpp
    sources/EtatJeu.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System)

# Si on est sur Windows
if(WIN32)
    # Copie des DLLs SFML à côté de l'exécutable
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SFML_LIB_PATH}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# Pour que l'exécutable trouve les .so ou .dylib au lancement
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${SFML_LIB_PATH}")

# Copie des fichiers images pour que l'executable y est accès
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})