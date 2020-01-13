//
// Created by sven on 06.05.19.
//

#ifndef OPENGL_ENGINE_CHUNK_H
#define OPENGL_ENGINE_CHUNK_H


#include "../utilities/enums/block.h"

class Chunk {
public:
    // A 16x16x16 ChunkManager
    Chunk(BlockType blocks[16][16][16]);
    static const int CHUNK_SIZE = 16;
    BlockType blocks[16][16][16];

    std::vector<float> cachedVertices;
    bool hasChanged = false;
    bool firstRender = true;

    void muteNonVisibleBlocks();
};


#endif //OPENGL_ENGINE_CHUNK_H
