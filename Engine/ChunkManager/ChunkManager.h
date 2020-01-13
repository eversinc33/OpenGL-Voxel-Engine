//
// Created by sven on 08.05.19.
//

#ifndef OPENGL_ENGINE_CHUNKMANAGER_H
#define OPENGL_ENGINE_CHUNKMANAGER_H


#include "../camera/camera.h"
#include "Chunk.h"
#include "../utilities/structs/coord.h"
#include "../game/Game.h"
#include "../utilities/structs/coord3d.h"

class ChunkManager {

public:
    ChunkManager(Camera *camera, Game *pGame);

private:
    Camera* camera;

public:
    std::vector<std::pair<coord, Chunk*>> getChunksToRender();

    void readMapData(std::vector<std::pair<coord, Chunk *>> map);

    std::vector<std::pair<coord, Chunk *>> mapData;
    Game *game;

    Chunk* getChunkByCoords(int x, int z);

    static coord3df worldToChunkCoordinates(float x, float y, float z);
    static coord3d worldToChunkCoordinatesInt(int x, int y, int z);
};


#endif //OPENGL_ENGINE_CHUNKMANAGER_H
