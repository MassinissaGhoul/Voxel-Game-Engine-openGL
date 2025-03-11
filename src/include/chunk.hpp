#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "../glad/include/glad/glad.h"
#include "block.hpp"
#include "shader.h"
#include "textureAtlas.hpp"
#include <array>
#include <tuple>


#include <vector>

enum Direction { FRONT, BACK, DIR_RIGHT, DIR_LEFT, TOP, BOTTOM };

// Tableau des offsets des directions
constexpr std::array<std::tuple<int, int, int>, 6> directionOffsets = {{
    {0, 0, 1},  // FRONT
    {0, 0, -1}, // BACK
    {1, 0, 0},  // RIGHT
    {-1, 0, 0}, // LEFT
    {0, 1, 0},  // TOP
    {0, -1, 0}  // BOTTOM
}};
struct blockData {
        bool isOpaque;
        int uvTop;
        int uvBottom;
        int uvSide;
};
class Camera;
class Chunk {
        friend class Camera;

    public:
        static const size_t CHUNK_SIZE = 32;
        static const size_t CHUNK_HEIGHT = 256; 
        TextureAtlas &atlasChunk;
        Chunk(TextureAtlas &atlas);
        ~Chunk();
        inline void addVertex(std::vector<float> &vertexData, float px,
                              float py, float pz, float u, float v);
        void addFaceVertices(std::vector<float> &vertexData, int x, int y,
                             int z, Direction dir, float uMin, float vMin,
                             float uMax, float vMax);
        bool isFaceVisible(int x, int y, int z, Direction direction);
        void draw(Shader &shader, glm::mat4 model);
        void action(int x, int y, int z, int action);
        void rebuild();
        void setBlock(int x, int y, int z, blockType block);

    private:
        GLuint VAO, VBO, EBO, instanceVBO, instanceVAO;
        void setupMesh(TextureAtlas &atlas);
        int totalVertices = 0;
        blockType blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
};

#endif
