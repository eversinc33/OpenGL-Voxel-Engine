#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "resources/shaders/shader.h"
#include "inputmanager/InputManager.h"
#include "resources/resourcemanager.h"
#include "renderer/Renderer.h"
#include "gameobjects/drawables/Skybox.h"
#include "gameobjects/player/Player.h"
#include "terrain-generation/TerrainGenerator.h"
#include "ChunkManager/ChunkManager.h"
#include "utilities/enums/ui_states.h"
#include "ui/Ui.h"
#include <glm/glm.hpp>

// function prototypes;
void centerWindow(GLFWwindow *window, GLFWmonitor *monitor);
int init();
void cleanUp(std::vector<std::pair<coord, Chunk*>> chunks);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 700;

// engine vars
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
unsigned int VBO;

GLFWwindow* window;

Game game                   = Game();
Ui ui                       = Ui(&VBO);
Camera camera               = Camera(glm::vec3(300.0f, 15.0f, 300.0f));
Renderer renderer           = Renderer(&VBO, &camera, SCR_WIDTH, SCR_HEIGHT);
ChunkManager chunkManager   = ChunkManager(&camera, &game);
Player player               = Player(&camera, camera.Position, &chunkManager);
InputManager inputManager   = InputManager(&camera, &player, &renderer, &game);

// mouse
bool mouseEnteredFirstTime = false;
float lastX = SCR_WIDTH/2, lastY = SCR_HEIGHT/2;

int main()
{
    // initialize
    if (init() != 0)
        return -1;

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // load resources
    ResourceManager::getInstance().loadResources();
    renderer.setupGL();
    Skybox skybox = Skybox();

    // generate terrain
    const std::vector<std::pair<coord, Chunk *>> &mapData = TerrainGenerator::generateTerrain(ui);
    chunkManager.readMapData(mapData);

    // render loop
    // -----------
    int nbFrames = 0; double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {

        // Calc time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame; lastFrame = currentFrame;

        // Measure speed
        nbFrames++;
        if (currentFrame - lastTime >= 1.0){
            std::cout << "Ms per frame: " << 1000.0/double(nbFrames) << " Maximum (targeting 60fps): " << 16.6666 << std::endl;
            nbFrames = 0;
            lastTime += 1.0;
        }

        // get current chunks
        std::vector<std::pair<coord, Chunk*>> chunks = chunkManager.getChunksToRender();

        // input & update
        inputManager.processKeyInput(window, deltaTime);
        player.update(deltaTime);

        // render
        // ------

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto chunk : chunks)
        {
            renderer.renderChunk(chunk.second, chunk.first.x, chunk.first.z);
        }
        renderer.render(&skybox);

        // ------

        ui.renderLoadingBar(100);

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanUp(mapData);
    return 0;
}

// init
int init() {

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Engine Demo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
    GLFWmonitor *monitor = monitors[0];
    centerWindow(window, monitor);

    // capture mouse and hide it
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback); // set hook

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    return 0;

}

void cleanUp(std::vector<std::pair<coord, Chunk*>> chunks) {

    // clean before exiting application

    for (auto chunk : chunks) {
        delete chunk.second;
    }

    glfwTerminate();

}

// resize window callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// mouse listener
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if(mouseEnteredFirstTime) {
        lastX = xpos;
        lastY = ypos;
        mouseEnteredFirstTime = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    inputManager.processMouseMovement(xoffset, yoffset);

}

void centerWindow(GLFWwindow *window, GLFWmonitor *monitor)
{
    if (!monitor)
        return;

    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    if (!mode)
        return;

    int monitorX, monitorY;
    glfwGetMonitorPos(monitor, &monitorX, &monitorY);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    glfwSetWindowPos(window,
                     monitorX + (mode->width - windowWidth) / 2,
                     monitorY + (mode->height - windowHeight) / 2);
}
