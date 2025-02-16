
# Définir les fichiers sources
set(SOURCES
    main.cpp
    glad/src/glad.c  # Chemin correct vers glad.c
    shader.cpp
    camera.cpp
    block.cpp
    textureAtlas.cpp
    chunk.cpp
    loadSky.cpp
    blockRegistry.cpp
    linking/lib/stb.cpp
)

# Créer l'exécutable
add_executable(OpenGLWindow ${SOURCES})

# Inclure les headers
target_include_directories(OpenGLWindow PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/linking/include
    ${CMAKE_CURRENT_SOURCE_DIR}/linking/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Copier les fichiers shaders GLSL dans le dossier de build
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader ${CMAKE_CURRENT_SOURCE_DIR}/texture
     DESTINATION ${CMAKE_BINARY_DIR})

# Lier les bibliothèques nécessaires
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(OpenGLWindow PRIVATE
    OpenGL::GL
    glfw
)

