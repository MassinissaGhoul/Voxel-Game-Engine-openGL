
#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "../glad/include/glad/glad.h"
#include "shader.h"
#include "textureAtlas.hpp"
#include <vector>

class Chunk {
    public:
        static const int CHUNK_SIZE = 32;

        Chunk(TextureAtlas &atlas);
        ~Chunk();

        void draw(Shader &shader);

    private:
        GLuint VAO, VBO, EBO, instanceVBO;
        void setupMesh();
};

#endif // CHUNK_HPP
