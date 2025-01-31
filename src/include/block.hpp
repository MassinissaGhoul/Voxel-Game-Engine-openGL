
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "../glad/include/glad/glad.h"
#include "../linking/include/glm/glm.hpp"
#include "shader.h"
#include "textureAtlas.hpp"

enum blockType : uint8_t {
    DIRT,
    GRASS,
    STONE,
    WOOD,
};

class Block {
    public:
        glm::ivec3 blockPos;
        blockType type;
        uint8_t textureIndex;
        GLuint VAO, VBO; // OpenGL VAO/VBO pour le rendu

        Block(glm::ivec3 blockPos, blockType type, uint8_t textureIndex,
              TextureAtlas &atlas);
        ~Block();

        void render(Shader &shader, TextureAtlas &atlas);

    private:
        void setupMesh(TextureAtlas &atlas);
};

#endif // BLOCK_HPP
