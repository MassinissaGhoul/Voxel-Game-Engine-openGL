#include "include/worldGeneration.hpp"

WorldGeneration::WorldGeneration() {
    FastNoiseLite noiseGen;
    noiseGen.SetSeed(this->seed);

    this->noise = noiseGen;

    std::cout << "worldgen\n";
}
void WorldGeneration::generateSeed() {
    std::random_device rd;
    this->seed =
        std::abs(static_cast<int>(rd() ^ static_cast<int>(time(nullptr))));
    this->noise.SetSeed(this->seed);
}
void WorldGeneration::generateChunk(Chunk &chunk, int chunkX, int chunkZ) {
    this->noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    this->noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    this->noise.SetFractalOctaves(this->fractalValue);
    this->noise.SetFrequency(this->frequencyValue);
    this->noise.SetFractalLacunarity(this->fractalLacunarityValue);
    this->noise.SetFractalGain(this->fractalGainValue);

    const int baseHeight = 20;

    for (int x = 0; x < chunk.CHUNK_SIZE; x++) {
        for (int z = 0; z < chunk.CHUNK_SIZE; z++) {
            int worldX = chunkX * chunk.CHUNK_SIZE + x;
            int worldZ = chunkZ * chunk.CHUNK_SIZE + z;

            float heightOffset =
                this->noise.GetNoise((float)worldX, (float)worldZ) *
                this->amplitudeValue;
            int groundHeight = static_cast<int>(baseHeight + heightOffset);

            if (groundHeight < 0)
                groundHeight = 0;
            if (groundHeight >= chunk.CHUNK_HEIGHT) {
                groundHeight = chunk.CHUNK_HEIGHT - 1;
            }

            for (int y = 0; y < chunk.CHUNK_HEIGHT; y++) {
                if (y > groundHeight) {
                    chunk.setBlock(x, y, z, AIR);
                } else if (y == groundHeight) {
                    chunk.setBlock(x, y, z, GRASS);
                } else if (y >= groundHeight - 3) {
                    chunk.setBlock(x, y, z, DIRT);
                } else {
                    chunk.setBlock(x, y, z, STONE);
                }
            }
        }
    }

    chunk.rebuild();
}

WorldGeneration::~WorldGeneration() { std::cout << "desc gen\n"; }
