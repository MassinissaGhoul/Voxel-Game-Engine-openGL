
# Définir les fichiers sources
set(SOURCES
    main.cpp
    glad/src/glad.c  # Chemin correct vers glad.c
)

# Créer l'exécutable
add_executable(OpenGLWindow ${SOURCES})

# Inclure les headers
target_include_directories(OpenGLWindow PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

# Lier les bibliothèques nécessaires
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(OpenGLWindow PRIVATE
    OpenGL::GL
    glfw
)

