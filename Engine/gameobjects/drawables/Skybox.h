//
// Created by sven on 04.05.19.
//

#ifndef OPENGL_ENGINE_SKYBOX_H
#define OPENGL_ENGINE_SKYBOX_H


#include "drawable.h"

class Skybox : public Drawable {
public:
    Skybox();

    void setupBuffers() override ;

    void draw() override ;
};



#endif //OPENGL_ENGINE_SKYBOX_H
