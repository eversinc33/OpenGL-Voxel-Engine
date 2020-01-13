//
// Created by sven on 05.05.19.
//

#ifndef OPENGL_ENGINE_GAME_H
#define OPENGL_ENGINE_GAME_H


class Game {

public:
    Game() {};

    static const int WORLD_BOUNDS_XZ = 64;
    static const int WORLD_BOUNDS_Y = 16;

    struct game_settings {
        int RENDER_DISTANCE_CHUNKS = 5;
    } settings;

    enum State {
        RUNNING,
        PAUSED
    };

    State currentState = RUNNING;

};


#endif //OPENGL_ENGINE_GAME_H
