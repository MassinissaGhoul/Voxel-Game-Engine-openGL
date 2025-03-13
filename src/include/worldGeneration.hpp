#ifndef WORLDGENERATION_HPP
#define WORLDGENERATION_HPP
#include "../linking/include/FastNoiseLite/FastNoiseLite.h"
#include "chunk.hpp"
class Chunk;
class WorldGeneration {
    public:
        int fractalValue = 4;
        float frequencyValue = 0.01f;
        WorldGeneration();
        void generateChunk(Chunk &chunk, int chunkX, int chunkZ);
        ~WorldGeneration();

    private:
        FastNoiseLite noise;
};

#endif
