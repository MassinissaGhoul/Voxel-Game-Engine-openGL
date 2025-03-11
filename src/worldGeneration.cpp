#include "include/worldGeneration.hpp"



void WorldGeneration::generateChunk(Chunk& chunk){
    FastNoiseLite noiseOS; // opensimplex2
    noiseOS.SetSeed(1333);
    noiseOS.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseOS.SetFrequency(0.2f);
    


}