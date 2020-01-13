//
// Created by sven on 05.05.19.
//

#include "Player.h"
#include "../../utilities/structs/coord3d.h"
#include "../../ChunkManager/ChunkManager.h"
#include <math.h>

Player::Player(Camera *camera, glm::vec3 position, ChunkManager *chunkManager) {

    this->camera = camera;
    this->position = position;
    this->chunkManager = chunkManager;

}

void Player::processInput(InputManager::Direction dir, float deltaTime) {

    // Move Player by Input
    // ------------------------
    float movVel = movementSpeed * deltaTime;

    if (dir == InputManager::Direction::FORWARD)
        posChange = camera->Front * movVel;
    if (dir == InputManager::Direction::BACKWARD)
        posChange = -(camera->Front * movVel);
    if (dir == InputManager::Direction::LEFT)
        posChange = -(camera->Right * movVel);
    if (dir == InputManager::Direction::RIGHT)
        posChange = camera->Right * movVel;

    position += posChange;

    // ------------------------

    if (state == Player::State::GROUND && !godModeOn)
    {

        // position.y = lastGround; // force stay on ground

    }

    // Jump
    if (dir == InputManager::Direction::JUMP && state == Player::State::GROUND)
    {

        state = Player::State::AIR;
        this->velocity = 42.0f;

    }

}

void Player::update(float deltaTime) {

    // chunk index
    int chunkX = (int) std::floor(position.x / Chunk::CHUNK_SIZE);
    int chunkY = (int) std::floor(position.y / Chunk::CHUNK_SIZE);
    int chunkZ = (int) std::floor(position.z / Chunk::CHUNK_SIZE);

    // Current ChunkManager
    Chunk* currentChunk = chunkManager->getChunkByCoords(chunkX, chunkZ);

    // Relative position on chunk
    this->coords = ChunkManager::worldToChunkCoordinates(position.x, position.y, position.z);

    if (!this->godModeOn)
    {

        // if jumping, substract gravity

        if (state == Player::State::AIR)
        {

            velocity += gravity;
            position.y += velocity * deltaTime;
            if (velocity < 0.0f) { velocity = 0.0f; }

        }

        // ------------------------------------------
        // Collision detection
        // ------------------------------------------

        // TODO: COLLISION DETECITON

    }

    // update position
    // ---------------------------
    camera->Position = position;

}

bool Player::checkCollisionWith(float x2, float y2, float z2) {

    coord3df coords = ChunkManager::worldToChunkCoordinates(position.x, position.y, position.z);

    // Block Size
    float size = 1.0f;

    return(coords.x > x2 &&
           coords.x < x2+size &&
           coords.y+size > y2 &&
           coords.y < y2+size &&
           coords.z+size > z2 &&
           coords.z < z2+size);

}

void Player::setGodMode(bool godModeOn) {
    this->godModeOn = godModeOn;
}
