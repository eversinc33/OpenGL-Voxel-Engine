//
// Created by sven on 06.05.19.
//

#include <vector>
#include "Chunk.h"
#include "../utilities/structs/coord3d.h"

Chunk::Chunk(BlockType blocks[16][16][16]) {

    // Init Array // TODO: optimize
    for (int i=0; i<16; ++i) {
        for (int j=0; j<16; ++j) {
            for (int k=0; k<16; ++k) {
                this->blocks[i][j][k] = blocks[i][j][k];
            }
        }
    }

    muteNonVisibleBlocks();

}

void Chunk::muteNonVisibleBlocks() {

    std::vector<coord3d> nonVisible = std::vector<coord3d>();

    for (int x=0; x<16; ++x) {

        for (int y = 0; y < 16; ++y) {

            for (int z = 0; z < 16; ++z) {

                // If block hidden between solid blocks (not air)

                if (x >= 1 && z >= 1 && y >= 1 && x < Chunk::CHUNK_SIZE-1 && y < Chunk::CHUNK_SIZE-1 && z < Chunk::CHUNK_SIZE-1 &&
                    blocks[x][y + 1][z] != BlockType::AIR &&
                    blocks[x][y][z + 1] != BlockType::AIR &&
                    blocks[x][y][z - 1] != BlockType::AIR &&
                    blocks[x + 1][y][z - 1] != BlockType::AIR &&
                    blocks[x + 1][y][z + 1] != BlockType::AIR &&
                    blocks[x + 1][y][z] != BlockType::AIR &&
                    blocks[x - 1][y][z - 1] != BlockType::AIR &&
                    blocks[x - 1][y][z] != BlockType::AIR &&
                    blocks[x - 1][y][z + 1] != BlockType::AIR)
                {

                    // mark for deletion so that calculation is not influenced during this loop
                    coord3d b = { x, y, z };
                    nonVisible.push_back(b);

                }

            }

        }

    }

    // mute all
    for (auto b : nonVisible) {

        blocks[b.x][b.y][b.z] = BlockType::AIR;

    }

}