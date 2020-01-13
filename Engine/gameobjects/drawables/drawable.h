//
// Created by sven on 03.05.19.
//

#ifndef OPENGL_ENGINE_DRAWABLE_H
#define OPENGL_ENGINE_DRAWABLE_H

#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include "../../resources/shaders/shader.h"
#include "../../resources/textures/texture.h"

// Interface 
class Drawable {
public:
    Drawable() {}

    enum Category {
        SKYBOX,
        BLOCK,
        NO_DRAW
    };

    Category cat;

    virtual void setupBuffers() = 0;
    virtual void draw() = 0;

    // Comparator for priorized rendering
    bool operator() (Drawable* a, Drawable* b)
    {
        // always render skybox last
        if (a->cat == Category::SKYBOX)
            return false;
        if (b->cat == Category::SKYBOX)
            return true;
    }

    unsigned int VAO;
    glm::vec3 position;

    Shader* shader;
    Texture* specularTexture;
    Texture* texture;
};

#endif //OPENGL_ENGINE_DRAWABLE_H
