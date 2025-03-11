#ifndef WORLD_HPP
#define WORLD_HPP
#include "camera.hpp"
#include "chunk.hpp"
#include <memory>
#include <unordered_map>
#include "worldGeneration.hpp"

class Chunk;
class Camera;
class WorldGeneration;

class World
{
public:
    size_t hashCord(int x, int z);

    std::unordered_map<size_t, std::unique_ptr<Chunk>> worldMap;
    std::unordered_map<size_t, Chunk *> worldLoaded;
    void update(Shader &shader);
    void render(Shader &shader);
    World(TextureAtlas &atlas, Camera *cameraRef, WorldGeneration *worldGeneration);
    ~World();

private:
    Camera *cameraRef;
    TextureAtlas &atlas;
    WorldGeneration *worldGenerationRef;
};

#endif
