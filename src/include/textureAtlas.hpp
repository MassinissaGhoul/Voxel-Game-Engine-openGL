
#ifndef TEXTUREATLAS_HPP
#define TEXTUREATLAS_HPP

#include "../glad/include/glad/glad.h"
#include "../linking/include/glm/ext/vector_float4.hpp"
#include "../linking/include/stb/stb_image.h"
#include <cstdint>
#include <iostream>
#include <string>
class TextureAtlas {
    public:
        GLuint ID;                      // Identifiant OpenGL de la texture
        int width, height, numTextures; // Dimensions et nombre de textures
        int tileSize;                   // Taille d'une texture dans l'atlas

        TextureAtlas(const std::string &filePath, int tileSize);
        void bind() const;
        glm::vec4 getUVs(uint8_t textureIndex) const;

    private:
        void load(const std::string &filePath);
};

#endif // TEXTUREATLAS_HPP
