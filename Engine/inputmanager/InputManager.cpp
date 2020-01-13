//
// Created by sven on 05.05.19.
//

#include "InputManager.h"

InputManager::InputManager(Camera* camera, Player* player, Renderer* renderer, Game* game) {

    this->camera   = camera;
    this->player   = player;
    this->game     = game;
    this->renderer = renderer;

}

void InputManager::processMouseMovement(float xoffset, float yoffset) {

    if (game->currentState == Game::RUNNING)
    {
        camera->processMouseMovement(xoffset, yoffset);
    }
    if (game->currentState == Game::PAUSED)
    {
        //TODO: Process GUI
    }

}

void InputManager::processKeyInput(GLFWwindow *window, float deltaTime) {

    // ESC = Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // SPACE = Jump
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        player->processInput(JUMP, deltaTime);

    // E activates Godmode
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        (player->godModeOn) ? player->setGodMode(false) : player->setGodMode(true);

    // WASD
    if (game->currentState == Game::RUNNING)
    {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            player->processInput(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            player->processInput(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            player->processInput(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            player->processInput(RIGHT, deltaTime);

    }

}