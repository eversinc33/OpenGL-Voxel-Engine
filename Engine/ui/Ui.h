//
// Created by sven on 15/09/2019.
//

#ifndef OPENGL_ENGINE_UI_H
#define OPENGL_ENGINE_UI_H

#include <glfw-3.3/include/GLFW/glfw3.h>
#include "../utilities/enums/ui_states.h"

class Ui {
public:
    Ui(unsigned int *VBO);

    UiState getState();
    void setState(UiState state);
    void renderLoadingBar(int percentage);

private:
    UiState state;
    unsigned int *VBO;
};


#endif //OPENGL_ENGINE_UI_H
