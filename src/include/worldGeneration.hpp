#ifndef WORLDGENERATION_HPP
#define WORLDGENERATION_HPP
#include "chunk.hpp"
#include "include/FastNoiseLite/FastNoiseLite.h"
class Chunk;
class WorldGeneration
{
public:
    WorldGeneration();
    void generateChunk(Chunk &chunk);

private:
    ~WorldGeneration();
};

#endif
