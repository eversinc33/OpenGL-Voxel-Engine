//
// Created by sven on 02.05.19.
//

#include <glm/ext.hpp>
#include "SimpleLight.h"

SimpleLight::SimpleLight(glm::vec3 position) {

    this->x = position.x;
    this->y = position.y;
    this->z = position.z;

    this->position = glm::vec3(x, y, z);

}

SimpleLight::SimpleLight(float x, float y, float z) {

    this->x = x;
    this->y = y;
    this->z = z;

    this->position = glm::vec3(x, y, z);

}