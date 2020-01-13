//
// Created by sven on 04.05.19.
//

#ifndef OPENGL_ENGINE_TERRAINGENERATOR_H
#define OPENGL_ENGINE_TERRAINGENERATOR_H

#include "PerlinNoise-master/PerlinNoise.hpp"
#include "../ChunkManager/Chunk.h"
#include "../game/Game.h"
#include "../utilities/structs/coord.h"
#include "../ui/Ui.h"
#include <map>

class TerrainGenerator {
public:
    static unsigned long getRandom(const unsigned long &begin, const unsigned long &end);

    static std::vector<std::pair<coord, Chunk *>> generateTerrain(Ui ui);

};


#endif //OPENGL_ENGINE_TERRAINGENERATOR_H
