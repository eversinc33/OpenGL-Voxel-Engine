//
// Created by sven on 15/09/2019.
//

#include <glad/glad.h>
#include "Ui.h"
#include "../resources/shaders/shader.h"
#include "../resources/resourcemanager.h"

Ui::Ui(unsigned int *VBO) {
    this->state = DEACTIVATED;
    this->VBO = VBO;
}

void Ui::renderLoadingBar(int percentage) {
    // TODO: draw loading bar
}

void Ui::setState(UiState state) {
    this->state = state;
}

UiState Ui::getState() {
    return this->state;
}


