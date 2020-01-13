//
// Created by sven on 04.05.19.
//

#include <iostream>
#include <climits>
#include "TerrainGenerator.h"
#include "../game/Game.h"

unsigned long int TerrainGenerator::getRandom(const unsigned long int &begin = 0, const unsigned long int &end = 100) {

    // Get random seed

    std::srand(time(NULL));
    unsigned long int seed = begin >= end ? 0 : begin + (unsigned long int) ((end - begin)*std::rand());
    std::cout << seed << std::endl;
    return seed;

}


std::vector<std::pair<coord, Chunk *>> TerrainGenerator::generateTerrain(Ui ui) {

    // TODO: nx, ny = number of chunks

    ui.setState(LOADING);
    ui.renderLoadingBar(0);

    siv::PerlinNoise perlin = siv::PerlinNoise(getRandom(0, LLONG_MAX)); //seed

    const int OCTAVES = 100;
    const int HEIGHT = Chunk::CHUNK_SIZE*2;
    int nx = 25; int nz = 25; // Number of Chunks
    const int WIDTH_X = 25*Chunk::CHUNK_SIZE;
    const int WIDTH_Z = 25*Chunk::CHUNK_SIZE;

    // ------------------------
    // Map Generation
    // ------------------------

    float heightMap[WIDTH_X][WIDTH_Z] = {{ 0.0f }};
    float humidityMap[WIDTH_X][WIDTH_Z] = {{ 0.0f }};

    auto*** blockMap = new BlockType**[WIDTH_Z];
    for(int i = 0; i < WIDTH_Z; ++i) {
        blockMap[i] = new BlockType*[HEIGHT*2];
        for(int j = 0; j < HEIGHT; ++j) {
            blockMap[i][j] = new BlockType[WIDTH_X];
        }
    }

    ui.renderLoadingBar(25);

    // height map

    for (int z = 0; z < nz*Chunk::CHUNK_SIZE; z++) {

        for (int x = 0; x < nx*Chunk::CHUNK_SIZE; x++) {

            // generate noise for height
            double noise = perlin.octaveNoise0_1(x / (double) (nx*Chunk::CHUNK_SIZE), z / (double) (nz*Chunk::CHUNK_SIZE), OCTAVES);
            heightMap[x][z] = pow(noise, 5.1);

        }

    }

    ui.renderLoadingBar(40);

    // humidity map (seed again)

    perlin = siv::PerlinNoise(getRandom(0, LLONG_MAX)); //seed

    for (int z = 0; z < nz*Chunk::CHUNK_SIZE; z++) {

        for (int x = 0; x < nx*Chunk::CHUNK_SIZE; x++) {

            // generate noise for humidity
            float noise = perlin.octaveNoise0_1(x / (float) (nx*Chunk::CHUNK_SIZE), z / (float) (nz*Chunk::CHUNK_SIZE), OCTAVES);
            humidityMap[x][z] = noise;

        }

    }

    ui.renderLoadingBar(60);

    // ------------------------------
    // Generate biomes
    // ------------------------------

    for (int z = 0; z < nz*Chunk::CHUNK_SIZE; z++) {

        for (int x = 0; x < nx*Chunk::CHUNK_SIZE; x++) {

            // get height and humidity value from previously generated maps
            float noise = heightMap[x][z];
            float humidity = humidityMap[x][z];

            for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {

                // Biomes
                // ------------------

                // For arid terrain

                if (humidity <= 0.3)
                {

                    if (y <= noise*Chunk::CHUNK_SIZE && y <= 0.7*Chunk::CHUNK_SIZE)
                    {

                        blockMap[x][y][z] = BlockType::SAND;

                    }
                    else if (y <= noise*Chunk::CHUNK_SIZE)
                    {

                        blockMap[x][y][z] = BlockType::SANDSTONE;

                    }
                    else
                    {

                        blockMap[x][y][z] = BlockType::AIR;

                    }

                }

                    // humid terrain

                else if (humidity > 0.3)
                {

                    if (y <= noise*Chunk::CHUNK_SIZE && y <= 0.7*Chunk::CHUNK_SIZE)
                    {

                        blockMap[x][y][z] = BlockType::GRASS;

                    }
                    else if (y <= noise*Chunk::CHUNK_SIZE)
                    {

                        blockMap[x][y][z] = BlockType::GROUND;

                    }
                    else
                    {

                        blockMap[x][y][z] = BlockType::AIR;

                    }

                }

            }

        }

    }

    ui.renderLoadingBar(80);

    // -----------------------------
    // Generate Trees
    // -----------------------------

    int treeSpawnRate = 0.4; // percent chance to spawn a tree
    int maxTreeHeight = 3;

    for (int z = 1; z < nz*Chunk::CHUNK_SIZE-1; z++) {  // 1 - (-1) -> do not place on edge to avoid segfault

        for (int x = 1; x < nx * Chunk::CHUNK_SIZE-1; x++) {

            for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {

                // Place tree on top of terrain
                if (blockMap[x][y][z] == BlockType::AIR)
                {

                    // if tree spawns, check if map is high enough
                    if ((rand() % 1000 + 1) >= (1000-treeSpawnRate) &&
                        y+maxTreeHeight+1 < Chunk::CHUNK_SIZE)
                    {

                        // generate random height
                        int treeHeight = ((rand() % (maxTreeHeight-1)) + 2);

                        // on arid terrain, trees do not grow that often or high
                        if (humidityMap[x][z] <= 0.3)
                        {
                            treeHeight = 2;
                            // therefore, occasionaly do not place tree (50 percent chance)
                            if ((rand() % 100 + 1) >= (50))
                            {
                                break;
                            }
                        }

                        int y2;

                        for (y2=y; y2<y+(treeHeight); y2++)
                        {
                            blockMap[x][y2][z] = BlockType::WOOD;
                        }

                        // place leafs

                        blockMap[x][y2][z] = BlockType::LEAFS;
                        if ((rand() % 100 + 1) >= (90))
                            blockMap[x+1][y2][z] = BlockType::LEAFS;
                        else
                            blockMap[x-1][y2][z] = BlockType::LEAFS;
                        if ((rand() % 100 + 1) >= (90))
                            blockMap[x][y2][z+1] = BlockType::LEAFS;
                        else
                            blockMap[x][y2][z-1] = BlockType::LEAFS;
                        if ((rand() % 100 + 1) >= (90)) {
                            blockMap[x+1][y2][z-1] = BlockType::LEAFS;
                            if ((rand() % 100 + 1) >= (90)) {
                                blockMap[x+1][y2+1][z-1] = BlockType::LEAFS;
                            }
                        }
                        if ((rand() % 100 + 1) >= (90)) {
                            blockMap[x-1][y2][z-1] = BlockType::LEAFS;
                            if ((rand() % 100 + 1) >= (90)) {
                                blockMap[x-1][y2+1][z-1] = BlockType::LEAFS;
                            }
                        }
                        if ((rand() % 100 + 1) >= (90)) {
                            blockMap[x+1][y2][z+1] = BlockType::LEAFS;
                            if ((rand() % 100 + 1) >= (90)) {
                                blockMap[x+1][y2+1][z+1] = BlockType::LEAFS;
                            }
                        }
                        if ((rand() % 100 + 1) >= (90)) {
                            blockMap[x-1][y2][z+1] = BlockType::LEAFS;
                            if ((rand() % 100 + 1) >= (90)) {
                                blockMap[x-1][y2+1][z+1] = BlockType::LEAFS;
                            }
                        }
                        // y+1
                        if ((rand() % 100 + 1) >= (90))
                            blockMap[x][y2+1][z] = BlockType::LEAFS;

                        break;

                    }

                    break;

                }

            }

        }

    }

    ui.renderLoadingBar(95);

    // ------------------------------
    // Chunkize Map Data
    // ------------------------------

    std::vector<std::pair<coord, Chunk*>> chunks = std::vector<std::pair<coord, Chunk*>>();

    for (int i=0; i<nx; ++i) {

        for (int j=0; j<nz; ++j) {

            BlockType blocksInChunk[Chunk::CHUNK_SIZE][Chunk::CHUNK_SIZE][Chunk::CHUNK_SIZE];

            // FOR EACH BLOCK
            for (int x = 0; x < Chunk::CHUNK_SIZE; ++x) {

                for (int z = 0; z < Chunk::CHUNK_SIZE; ++z) {

                    for (int y = 0; y < Chunk::CHUNK_SIZE; ++y) {

                        blocksInChunk[x][y][z] = blockMap[x+i*Chunk::CHUNK_SIZE][y][z+j*Chunk::CHUNK_SIZE];

                    }

                }

            }

            // Add chunk and corresponding coords to chunkmap

            coord c;
            c.x = i;
            c.z = j;

            std::pair<coord, Chunk*> pair = std::pair<coord, Chunk*>(c, new Chunk(blocksInChunk));

            chunks.push_back(pair);

        }

    }

    // Free memory
    for(int i = 0; i < WIDTH_Z; ++i)
        delete[] blockMap[i];
    delete[] blockMap;

    ui.setState(DEACTIVATED);

    return chunks;

}