#ifndef WORLDGENERATION_HPP
#define WORLDGENERATION_HPP
#include "../linking/include/FastNoiseLite/FastNoiseLite.h"
#include "chunk.hpp"
#include <ctime>
#include <random>
class Chunk;
class WorldGeneration {
    public:
        int seed = 7893850;
        int fractalValue = 4;
        float frequencyValue = 0.01f;
        float fractalLacunarityValue = 1.5f;
        float fractalGainValue = 0.3f;
        float amplitudeValue = 50.0f;
        WorldGeneration();
        void generateSeed();
        void generateChunk(Chunk &chunk, int chunkX, int chunkZ);
        ~WorldGeneration();

    private:
        FastNoiseLite noise;
};

#endif
