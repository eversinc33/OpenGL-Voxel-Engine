//
// Created by sven on 02.05.19.
//

#ifndef OPENGL_ENGINE_SIMPLELIGHT_H
#define OPENGL_ENGINE_SIMPLELIGHT_H


class SimpleLight {
public:
    SimpleLight(glm::vec3 position);
    SimpleLight(float x, float y, float z);
    float x, y, z;
    glm::vec3 position;

    void render();

};


#endif //OPENGL_ENGINE_SIMPLELIGHT_H
