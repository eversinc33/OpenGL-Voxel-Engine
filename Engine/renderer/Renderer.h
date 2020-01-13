//
// Created by sven on 04.05.19.
//

#ifndef OPENGL_ENGINE_RENDERER_H
#define OPENGL_ENGINE_RENDERER_H


#include <glm/ext.hpp>
#include <queue>
#include "../camera/camera.h"
#include "../gameobjects/drawables/drawable.h"
#include "../gameobjects/light/SimpleLight.h"
#include "../ChunkManager/Chunk.h"

class Renderer {
public:
    Renderer(unsigned int* VBO, Camera* camera, const unsigned int& SCR_WIDTH, const unsigned int& SCR_HEIGHT);
    glm::mat4 projection;
    Camera* camera;
    std::string lastDrawn;
    std::string lastTexture;
    unsigned int* VBO;

    // std::priority_queue<Drawable*, std::vector<Drawable*>, Drawable> renderBuffer = std::priority_queue<Drawable*, std::vector<Drawable*>, Drawable>();

    void setLight();
    bool torchLit = false;
    void renderAll();
    void render(Drawable* d);

    void setupGL();

    void renderChunk(Chunk* c, int xOffset, int zOffset);

    void addCube(int posX, int posY, int posZ, BlockType type, std::vector<float> *vertices, int xOffset, int zOffset);
};


#endif //OPENGL_ENGINE_RENDERER_H
