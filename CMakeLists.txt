
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(OpenGLWindow)

# Configurer la politique CMP0072 pour utiliser GLVND
if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

# Inclure le sous-dossier src
add_subdirectory(src)

