// undored map a utiliser
#include "include/world.hpp"

World::World(TextureAtlas &atlas, Camera* cameraRef) : cameraRef(cameraRef), atlas(atlas)
{
    float renderDistance = 3; 
    glm::vec3 cameraPos = cameraRef->getPosition();
    int cameraChunkX = floor(cameraPos.x / 32);
    int cameraChunkZ = floor(cameraPos.z / 32);
    int minChunkX = cameraChunkX - renderDistance;
    int maxChunkX = cameraChunkX + renderDistance;
    int minChunkZ = cameraChunkZ - renderDistance;
    int maxChunkZ = cameraChunkZ + renderDistance;

    for (int x = minChunkX; x < maxChunkX; x++) {
        for (int z = minChunkZ; z < maxChunkZ; z++) {
            Chunk chunk(atlas);
            glm::vec3 chunkPosition(x * 32 + 16, 0.0f, z * 32 + 16);
            // std::cout << minChunkX << "puiis " << minChunkZ << std::endl;
            float distance = glm::distance(chunkPosition, cameraPos);
            if (distance < renderDistance * 32) {
                size_t key = hashCord(x, z);
                worldMap[key] = std::make_unique<chunk>(chunk);
                glm::mat4 model2 =
                    glm::translate(glm::mat4(1.0f), chunkPosition);
                chunk.draw(triShader, model2);
            }
        }
}
}

size_t World::hashCord(int x, int z)
{
    size_t hx = std::hash<int>()(x);

    size_t hz = std::hash<int>()(z);

    return ((hx << 1) ^ hz);
}

World::~World()
{
    
    std::cout << "destr" << std::endl;

}