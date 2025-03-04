// undored map a utiliser
#include "include/world.hpp"

World::World(TextureAtlas &atlas, Camera *cameraRef)
    : cameraRef(cameraRef),
      atlas(atlas) {
    std::cout << "objet world \n";
}

void World::update(Shader &shader) {
    float renderDistance = 10;
    float chunkSize = 32.0f;
    glm::vec3 cameraPos = this->cameraRef->getPosition();
    
    // Calcul précis des coordonnées de chunk sans décalage
    int cameraChunkX = static_cast<int>(std::floor(cameraPos.x / chunkSize));
    int cameraChunkZ = static_cast<int>(std::floor(cameraPos.z / chunkSize));
    
    int minChunkX = cameraChunkX - renderDistance;
    int maxChunkX = cameraChunkX + renderDistance;
    int minChunkZ = cameraChunkZ - renderDistance;
    int maxChunkZ = cameraChunkZ + renderDistance;

    std::cout << "Camera Chunk Position: " 
              << "x=" << cameraChunkX 
              << ", z=" << cameraChunkZ << std::endl;

    std::cout << "Chunk Calculation: " 
              << "chunkX=" << cameraChunkX 
              << ", chunkZ=" << cameraChunkZ 
              << ", chunkSize=" << 32 << std::endl;

    
    for (int x = minChunkX; x < maxChunkX; x++) {
        for (int z = minChunkZ; z < maxChunkZ; z++) {
            glm::vec3 chunkPosition(x * chunkSize, 0.0f, z * chunkSize);
            // std::cout << x << "puiis " << z << std::endl;
            float distance = glm::distance(chunkPosition, cameraPos);
            size_t key = hashCord(x, z);
            if (distance < renderDistance * 32 &&
                worldMap.find(key) == worldMap.end()) {
                //std::cout << x << "puiis " << z << std::endl;

                // Chunk chunk(atlas);
                worldMap[key] = std::make_unique<Chunk>(this->atlas);
                //std::cout << worldMap[key] << std::endl;
            }
        }
    }
    render(shader);
}

void World::render(Shader &shader) {

    float renderDistance = 10;
    float chunkSize = 32.0f;
    glm::vec3 cameraPos = this->cameraRef->getPosition();
    
    int cameraChunkX = static_cast<int>(std::floor(cameraPos.x / chunkSize));
    int cameraChunkZ = static_cast<int>(std::floor(cameraPos.z / chunkSize));
    
    int minChunkX = cameraChunkX - renderDistance;
    int maxChunkX = cameraChunkX + renderDistance;
    int minChunkZ = cameraChunkZ - renderDistance;
    int maxChunkZ = cameraChunkZ + renderDistance;

    for (int x = minChunkX; x < maxChunkX; x++) {
        for (int z = minChunkZ; z < maxChunkZ; z++) {
            glm::vec3 chunkPosition(x * chunkSize , 0.0f, z * chunkSize);
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
    size_t seed = 0;
    seed ^= std::hash<int>()(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= std::hash<int>()(z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}

World::~World() { std::cout << "destr" << std::endl; }
