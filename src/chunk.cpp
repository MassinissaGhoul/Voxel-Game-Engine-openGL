#include "include/chunk.hpp"
#include "include/block.hpp"
#include "include/blockRegistry.hpp"
#include <cstdint>

Chunk::Chunk(TextureAtlas &atlas) {

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {

            int baseHeight = CHUNK_SIZE / 2;
            int variation = static_cast<int>(std::sin(x * 0.3f) *
                                             std::cos(z * 0.3f) * 10.0f);
            int height = baseHeight + variation;

            if (height < 0)
                height = 0;
            if (height >= CHUNK_SIZE)
                height = CHUNK_SIZE - 1;

            for (int y = 0; y < CHUNK_SIZE; y++) {
                if (y <= height) {

                    if (y == height)
                        blocks[x][y][z] = GRASS;
                    else if (y >= height - 3)
                        blocks[x][y][z] = DIRT;
                    else
                        blocks[x][y][z] = STONE;
                } else {
                    blocks[x][y][z] = AIR;
                }
            }
        }
    }

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    setupMesh(atlas);
}
bool Chunk::isFaceVisible(int x, int y, int z, Direction direction) {
    auto [dx, dy, dz] = directionOffsets[static_cast<int>(direction)];
    int nx = x + dx;
    int ny = y + dy;
    int nz = z + dz;
    if (nx < 0 || nx >= this->CHUNK_SIZE || ny < 0 || ny >= this->CHUNK_SIZE ||
        nz < 0 || nz >= this->CHUNK_SIZE) {
        return true;
    }
    blockType neighbor = blocks[nx][ny][nz];
    if (neighbor == AIR) {
        // std::cout << "AUIEZHEZRHUEZRUREHUÇERHU\n";
        return true;
    }
    return false;
}

inline void Chunk::addVertex(std::vector<float> &vertexData, float px, float py,
                             float pz, float u, float v) {
    vertexData.push_back(px);
    vertexData.push_back(py);
    vertexData.push_back(pz);
    vertexData.push_back(u);
    vertexData.push_back(v);
}
void Chunk::addFaceVertices(std::vector<float> &vertexData, int x, int y, int z,
                            Direction dir, float uMin, float vMin, float uMax,
                            float vMax) {
    // On calcule les 4 points de la face en fonction de la direction
    // P1, P2, P3, P4 (x,y,z)
    // +2 triangles (6 vertices au total).
    float X = float(x);
    float Y = float(y);
    float Z = float(z);

    // Les 4 coins (P1,P2,P3,P4) selon la face
    // => On place le bloc dans [x, x+1], [y, y+1], [z, z+1]
    glm::vec3 P1, P2, P3, P4;

    switch (dir) {
    case TOP:
        // plan y+1
        // P1 = (x,   y+1, z)
        // P2 = (x+1, y+1, z)
        // P3 = (x+1, y+1, z+1)
        // P4 = (x,   y+1, z+1)
        P1 = {X, Y + 1, Z};
        P2 = {X + 1, Y + 1, Z};
        P3 = {X + 1, Y + 1, Z + 1};
        P4 = {X, Y + 1, Z + 1};
        break;

    case BOTTOM:
        // plan y
        P1 = {X, Y, Z};
        P2 = {X + 1, Y, Z};
        P3 = {X + 1, Y, Z + 1};
        P4 = {X, Y, Z + 1};
        break;

    case DIR_LEFT:
        // plan x
        P1 = {X, Y, Z};
        P2 = {X, Y, Z + 1};
        P3 = {X, Y + 1, Z + 1};
        P4 = {X, Y + 1, Z};
        break;

    case DIR_RIGHT:
        // plan x+1
        P1 = {X + 1, Y, Z};
        P2 = {X + 1, Y, Z + 1};
        P3 = {X + 1, Y + 1, Z + 1};
        P4 = {X + 1, Y + 1, Z};
        break;

    case FRONT:
        // plan z+1
        P1 = {X, Y, Z + 1};
        P2 = {X + 1, Y, Z + 1};
        P3 = {X + 1, Y + 1, Z + 1};
        P4 = {X, Y + 1, Z + 1};
        break;

    case BACK:
        // plan z
        P1 = {X, Y, Z};
        P2 = {X + 1, Y, Z};
        P3 = {X + 1, Y + 1, Z};
        P4 = {X, Y + 1, Z};
        break;
    }

    // Selon le winding (ordre), on peut choisir :
    // Tri1 : P1, P2, P3
    // Tri2 : P1, P3, P4
    // On associe uMin/vMin => P1, etc.
    // A adapter pour avoir un rendu cohérent (UV + face culling)

    // Triangle 1 (P1, P2, P3)
    //
    if (dir == BOTTOM) {
        // Triangle 1 (ordre inversé pour la face du bas)
        addVertex(vertexData, P1.x, P1.y, P1.z, uMin,
                  vMax); // Changé vMin à vMax
        addVertex(vertexData, P2.x, P2.y, P2.z, uMax,
                  vMax); // Changé vMin à vMax
        addVertex(vertexData, P3.x, P3.y, P3.z, uMax,
                  vMin); // Changé vMax à vMin

        // Triangle 2 (ordre inversé pour la face du bas)
        addVertex(vertexData, P1.x, P1.y, P1.z, uMin,
                  vMax); // Changé vMin à vMax
        addVertex(vertexData, P3.x, P3.y, P3.z, uMax,
                  vMin); // Changé vMax à vMin
        addVertex(vertexData, P4.x, P4.y, P4.z, uMin,
                  vMin); // Changé vMax à vMin
    } else {
        // Le code existant pour les autres faces...

        addVertex(vertexData, P1.x, P1.y, P1.z, uMin, vMin);
        addVertex(vertexData, P2.x, P2.y, P2.z, uMax, vMin);
        addVertex(vertexData, P3.x, P3.y, P3.z, uMax, vMax);

        // Triangle 2 (P1, P3, P4)
        addVertex(vertexData, P1.x, P1.y, P1.z, uMin, vMin);
        addVertex(vertexData, P3.x, P3.y, P3.z, uMax, vMax);
        addVertex(vertexData, P4.x, P4.y, P4.z, uMin, vMax);
    }
}
void Chunk::setupMesh(TextureAtlas &atlas) {
    std::vector<float> vertexData;
    vertexData.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6 * 5);
    // Réserve un maximum possible (optionnel)

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                blockType currentType = blocks[x][y][z];
                // On ne dessine pas l'AIR
                if (currentType == AIR) {
                    continue;
                }

                std::cout << "Position (" << x << "," << y << "," << z
                          << ") Block type: " << (int)currentType
                          << std::endl; // Récupère la "définition" du bloc
                                        // (opacité, uvTop, uvSide,
                // uvBottom)
                const blockData &bData = g_blockRegistry[currentType];
                std::cout << "Registry data - isOpaque: " << bData.isOpaque
                          << ", uvTop: " << bData.uvTop
                          << ", uvBottom: " << bData.uvBottom
                          << ", uvSide: " << bData.uvSide << std::endl;
                // Parcours les 6 directions
                for (int d = 0; d < 6; d++) {
                    Direction dir = static_cast<Direction>(d);

                    // Vérifie si la face est visible
                    if (!isFaceVisible(x, y, z, dir)) {
                        continue;
                    }

                    // Choisit quel index UV on utilise (top/bottom/side)
                    uint8_t uvIndex = 0;
                    switch (dir) {
                    case TOP:
                        uvIndex = bData.uvTop;
                        // std::cout << uvIndex << "\n";

                        break;
                    case BOTTOM:
                        uvIndex = bData.uvBottom;
                        // std::cout << uvIndex << "\n";
                        std::cout
                            << "Setting bottom uvIndex to: " << (int)uvIndex
                            << std::endl;
                        break;
                    default:
                        uvIndex = bData.uvSide;
                        break;
                    }

                    // Récupère (uMin, vMin, uMax, vMax) dans un glm::vec4
                    glm::vec4 uvCoords = atlas.getUVs(uvIndex);
                    float uMin = uvCoords.x;
                    float vMin = uvCoords.y;
                    float uMax = uvCoords.z;
                    float vMax = uvCoords.w;

                    // std::cout << x << y << z << std::endl;
                    //  Ajoute les 2 triangles (6 vertices) de cette face
                    addFaceVertices(vertexData, x, y, z, dir, uMin, vMin, uMax,
                                    vMax);
                }
            }
        }
    }

    // Upload sur GPU
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
                 vertexData.data(), GL_STATIC_DRAW);

    // Attributs : positions (3 floats) + UV (2 floats) = 5 floats/vertex
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));

    glBindVertexArray(0);

    // Garde en mémoire le nombre de vertices pour le draw d'ou le / 5
    totalVertices = static_cast<int>(vertexData.size() / 5);
}

void Chunk::draw(Shader &shader, glm::mat4 model) {
    // si atlas.bind() is not in main put it here
    shader.use();
    glBindVertexArray(VAO);
    /*
        glm::mat4 model1 = glm::mat4(1.0f);
        glm::mat4 model2 =
            glm::translate(glm::mat4(1.0f), glm::vec3(32.0f, 0.0f, 0.0f));*/
    shader.setMat4("model", model);
    // shader.setMat4("model", model2);
    glDrawArrays(GL_TRIANGLES, 0, totalVertices);
    glBindVertexArray(0);
}
Chunk::~Chunk() {

    glDeleteBuffers(1, &this->VBO);
    if (this->EBO != 0) {
        glDeleteBuffers(1, &this->EBO);
    }
    if (this->instanceVBO != 0) {
        glDeleteBuffers(1, &this->instanceVBO);
    }
    glDeleteVertexArrays(1, &this->VAO);
}
