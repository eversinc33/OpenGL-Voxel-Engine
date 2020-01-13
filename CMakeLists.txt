cmake_minimum_required(VERSION 3.14)
project(OpenGL_Engine)

set(CMAKE_CXX_STANDARD 17)

add_executable(OpenGL_Engine
        Engine/main.cpp
        libs/glad/glad.c
        Engine/resources/shaders/shader.cpp
        Engine/resources/shaders/shader.h
        Engine/resources/textures/texture.cpp
        Engine/resources/textures/texture.h
        Engine/resources/image-loader/stb_image.h
        Engine/camera/camera.h
        Engine/gameobjects/light/SimpleLight.cpp
        Engine/gameobjects/light/SimpleLight.h
        Engine/resources/resourcemanager.cpp
        Engine/resources/resourcemanager.h
        Engine/renderer/Renderer.cpp
        Engine/renderer/Renderer.h
        Engine/gameobjects/drawables/Skybox.cpp
        Engine/gameobjects/drawables/Skybox.h
		Engine/terrain-generation/TerrainGenerator.cpp
		Engine/terrain-generation/TerrainGenerator.h
        Engine/gameobjects/player/Player.cpp
        Engine/gameobjects/player/Player.h
        Engine/inputmanager/InputManager.cpp
        Engine/inputmanager/InputManager.h 
	Engine/game/Game.h
		Engine/ChunkManager/Chunk.cpp
		Engine/ChunkManager/Chunk.h
	Engine/utilities/structs/coord.h 
	Engine/utilities/enums/block.h 
	Engine/utilities/structs/coord3d.h
		Engine/ChunkManager/ChunkManager.cpp
		Engine/ChunkManager/ChunkManager.h Engine/utilities/enums/ui_states.h Engine/ui/Ui.cpp Engine/ui/Ui.h)

include_directories(libs/)

# glfw3
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/glfw-3.3)

target_link_libraries(OpenGL_Engine
        glfw
        )