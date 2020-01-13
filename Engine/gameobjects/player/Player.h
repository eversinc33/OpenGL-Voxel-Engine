//
// Created by sven on 05.05.19.
//

#ifndef OPENGL_ENGINE_PLAYER_H
#define OPENGL_ENGINE_PLAYER_H

#include "../../camera/camera.h"

class InputManager;

#include "../../inputmanager/InputManager.h"
#include "../../utilities/structs/coord.h"
#include "../../ChunkManager/ChunkManager.h"
#include "../../utilities/structs/coord3d.h"
#include <map>

class Player {
public:
    float movementSpeed =  5.0f;

    Player(Camera *camera, glm::vec3 position, ChunkManager *chunkManager);

    coord3df coords;
    Camera* camera;
    glm::vec3 position;

    enum State {
        GROUND,
        AIR
    };

    void processInput(InputManager::Direction dir, float deltaTime);

    void update(float deltaTime);

    bool checkCollisionWith(float x2, float y2, float z2);

    State state = Player::State::AIR;
    glm::vec3 posChange = glm::vec3(0.0f);
    float lastGround;
    float gravity = -4.81f;
    float velocity = 0.0f;;

    void setGodMode(bool godModeOn);

    bool godModeOn = true;
    ChunkManager *chunkManager;
};


#endif //OPENGL_ENGINE_PLAYER_H
