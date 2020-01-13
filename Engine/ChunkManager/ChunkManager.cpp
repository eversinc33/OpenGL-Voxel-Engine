//
// Created by sven on 08.05.19.
//

#include <iostream>
#include "ChunkManager.h"
#include "../game/Game.h"
#include "../utilities/structs/coord3d.h"

ChunkManager::ChunkManager(Camera *camera, Game *pGame) {

    this->camera = camera;
    this->game = pGame;

}

void ChunkManager::readMapData(std::vector<std::pair<coord, Chunk*>> map) {

    this->mapData = map;

}

Chunk* ChunkManager::getChunkByCoords(int x, int z) {

    for (std::pair<coord, Chunk*> chunk : mapData) {

        if (chunk.first.x == x && chunk.first.z)
        {

            return chunk.second;

        }

    }

}

coord3df ChunkManager::worldToChunkCoordinates(float x, float y, float z) {
    return coord3df{(x - ((int) std::floor(x / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE)),
                    (y - ((int) std::floor(y / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE)),
                    (z - ((int) std::floor(z / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE)) };
}

coord3d ChunkManager::worldToChunkCoordinatesInt(int x, int y, int z) {
    return coord3d{(x - ((int) std::floor(x / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE)),
                   (y - ((int) std::floor(y / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE)),
                   (z - ((int) std::floor(z / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE)) };
}

std::vector<std::pair<coord, Chunk*>> ChunkManager::getChunksToRender() {

    // if the camera is really high, only apply chunk position culling if camera pitch is really narrow

    int pitchThreshold = (camera->Position.y > 16) ? 20 : 60;

    // Find ID of current chunk player is located in

    coord playerCurrentChunk = { (int) std::floor(this->camera->Position.x/16),
                                 (int) std::floor(this->camera->Position.z/16) };

    // only get the chunks around him

    std::vector<std::pair<coord, Chunk*>> chunks = std::vector<std::pair<coord, Chunk*>>();

    for (std::pair<coord, Chunk*> chunk : mapData) {

        if ((chunk.first.x <= playerCurrentChunk.x+game->settings.RENDER_DISTANCE_CHUNKS && chunk.first.x >= playerCurrentChunk.x-game->settings.RENDER_DISTANCE_CHUNKS) &&
            (chunk.first.z <= playerCurrentChunk.z+game->settings.RENDER_DISTANCE_CHUNKS && chunk.first.z >= playerCurrentChunk.z-game->settings.RENDER_DISTANCE_CHUNKS))
        {

            // only render chunks that the camera can see
            double threshold = 0.45;

            if (fabs(camera->Pitch) <= pitchThreshold)
            {
                if (camera->Front.x < -threshold)
                {
                    if (chunk.first.x <= playerCurrentChunk.x)
                        chunks.push_back(chunk);
                }

                else if (camera->Front.x > threshold)
                {
                    if (chunk.first.x >= playerCurrentChunk.x)
                        chunks.push_back(chunk);
                }

                if (camera->Front.z < -threshold)
                {
                    if (chunk.first.z <= playerCurrentChunk.z)
                        chunks.push_back(chunk);
                }

                else if (camera->Front.z > threshold)
                {
                    if (chunk.first.z >= playerCurrentChunk.z)
                        chunks.push_back(chunk);
                }

            }
            else
            {

                chunks.push_back(chunk);

            }

        }

    }

    return chunks;

}
