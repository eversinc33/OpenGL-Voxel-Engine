//
// Created by sven on 05.05.19.
//

#ifndef OPENGL_ENGINE_INPUTMANAGER_H
#define OPENGL_ENGINE_INPUTMANAGER_H

#include "../camera/camera.h"
#include "../game/Game.h"
#include "../renderer/Renderer.h"
#include <GLFW/glfw3.h>
class Player;

class InputManager {
public:

    InputManager(Camera *camera, Player *player, Renderer* renderer, Game *game);
    InputManager() {};

    enum Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        JUMP
    };

    void processKeyInput(GLFWwindow *window, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);

    Camera *camera;
    Player *player;
    Game *game;
    Renderer *renderer;
};

#include "../gameobjects/player/Player.h"


#endif //OPENGL_ENGINE_INPUTMANAGER_H
