#ifndef WORLDGENERATION_HPP
#define WORLDGENERATION_HPP
#include "chunk.hpp"
#include "linking/include/FastNoiseLite/FastNoiseLite.h"
class Chunk;
class WorldGeneration
{
public:
    WorldGeneration();
    void generateChunk(Chunk &chunk, int chunkX, int chunkZ);
    ~WorldGeneration();

private:
    FastNoiseLite noise;

};

#endif
