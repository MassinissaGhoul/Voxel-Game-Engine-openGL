#ifndef WORLDGENERATION_HPP
#define WORLDGENERATION_HPP
#include "chunk.hpp"
#include "linking/include/FastNoiseLite/FastNoiseLite.h"
class Chunk;
class WorldGeneration
{
public:
    int fractalValue = 4;
    WorldGeneration();
    void generateChunk(Chunk &chunk, int chunkX, int chunkZ);
    ~WorldGeneration();

private:
    FastNoiseLite noise;

};

#endif
