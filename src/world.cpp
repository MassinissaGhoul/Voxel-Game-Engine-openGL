// undored map a utiliser
#include "include/world.hpp"

World::World(TextureAtlas &atlas, Camera *cameraRef)
    : cameraRef(cameraRef),
      atlas(atlas) {
    std::cout << "objet world \n";
}

void World::update(Shader &shader) {

    float renderDistance = 10;
    glm::vec3 cameraPos = this->cameraRef->getPosition();
    int cameraChunkX = floor(cameraPos.x / 32);
    int cameraChunkZ = floor(cameraPos.z / 32);
    int minChunkX = cameraChunkX - renderDistance;
    int maxChunkX = cameraChunkX + renderDistance;
    int minChunkZ = cameraChunkZ - renderDistance;
    int maxChunkZ = cameraChunkZ + renderDistance;

    for (int x = minChunkX; x < maxChunkX; x++) {
        for (int z = minChunkZ; z < maxChunkZ; z++) {
            glm::vec3 chunkPosition(x * 32 + 16, 0.0f, z * 32 + 16);
            // std::cout << x << "puiis " << z << std::endl;
            float distance = glm::distance(chunkPosition, cameraPos);
            size_t key = hashCord(x, z);
            if (distance < renderDistance * 32 &&
                worldMap.find(key) == worldMap.end()) {
                std::cout << x << "puiis " << z << std::endl;

                // Chunk chunk(atlas);
                worldMap[key] = std::make_unique<Chunk>(this->atlas);
                std::cout << worldMap[key] << std::endl;
            }
        }
    }
    render(shader);
}

void World::render(Shader &shader) {

    float renderDistance = 10;

    glm::vec3 cameraPos = this->cameraRef->getPosition();
    int cameraChunkX = floor(cameraPos.x / 32);
    int cameraChunkZ = floor(cameraPos.z / 32);
    int minChunkX = cameraChunkX - renderDistance;
    int maxChunkX = cameraChunkX + renderDistance;
    int minChunkZ = cameraChunkZ - renderDistance;
    int maxChunkZ = cameraChunkZ + renderDistance;

    for (int x = minChunkX; x < maxChunkX; x++) {
        for (int z = minChunkZ; z < maxChunkZ; z++) {
            glm::vec3 chunkPosition(x * 32 + 16, 0.0f, z * 32 + 16);
            // std::cout << minChunkX << "puiis " << minChunkZ << std::endl;
            float distance = glm::distance(chunkPosition, cameraPos);
            if (distance < renderDistance * 32) {
                size_t key = hashCord(x, z);
                if (this->worldMap.find(key) != this->worldMap.end()) {
                    glm::mat4 model2 =
                        glm::translate(glm::mat4(1.0f), chunkPosition);
                    worldMap[key]->draw(shader, model2);
                    // std::cout << "ca render les chunks" << std::endl;
                }
            }
        }
    }
}

size_t World::hashCord(int x, int z) {
    size_t hx = std::hash<int>()(x);

    size_t hz = std::hash<int>()(z);

    return ((hx << 1) ^ hz);
}

World::~World() { std::cout << "destr" << std::endl; }
