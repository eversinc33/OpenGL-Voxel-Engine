//
// Created by sven on 04.05.19.
//

#include <iostream>
#include <vector>
#include "resourcemanager.h"
#include "../utilities/structs/coord.h"
#include "../utilities/enums/block.h"

void ResourceManager::loadResources() {

    SHADER_DIR = "/home/sven/Documents/repos/modern-opengl-engine/Engine/resources/shaders/";
    TEXTURE_DIR = "/home/sven/Documents/repos/modern-opengl-engine/Engine/resources/textures/";

    shaders  = std::map<std::string, Shader>();
    textures = std::map<std::string, Texture>();

    setupShaders();
    setupTextures();

}

void ResourceManager::setupShaders() {

    // Load shaders

    addShader(Shader("SKYBOX",      SHADER_DIR+"vertex/skybox.vs",          SHADER_DIR+"fragment/skybox.fs"));
    addShader(Shader("TEX_LIGHTED", SHADER_DIR+"vertex/texture_lighted.vs", SHADER_DIR+"fragment/texture_lighted.fs"));
    addShader(Shader("WHITE",       SHADER_DIR+"vertex/basic.vs",           SHADER_DIR+"fragment/white.fs"));
    addShader(Shader("RED",         SHADER_DIR+"vertex/basic.vs",           SHADER_DIR+"fragment/red.fs"));

}

void ResourceManager::setupTextures() {

    // load skymap

    std::string skybox = "stormydays";
    std::string faces[] =
    {
            TEXTURE_DIR+"skyboxes/"+skybox+"_ft.jpg", // front
            TEXTURE_DIR+"skyboxes/"+skybox+"_bk.jpg", // back
            TEXTURE_DIR+"skyboxes/"+skybox+"_up.jpg", // up
            TEXTURE_DIR+"skyboxes/"+skybox+"_dn.jpg", // down
            TEXTURE_DIR+"skyboxes/"+skybox+"_ri.jpg", // right
            TEXTURE_DIR+"skyboxes/"+skybox+"_lf.jpg"  // left
    };
    addTexture(Texture("SKYBOX", faces));

    // Load textures

    addTexture(Texture("ATLAS",              TEXTURE_DIR+"png/atlas.png"));
    // !! N OF ROWS OF ATLAS SPECIFIED IN HEADER !
    addTexture(Texture("NONE",               TEXTURE_DIR+"png/specular_none.png"));

}

void ResourceManager::addShader(const Shader& s) {

    shaders.insert(std::pair<std::string, Shader>(s.name, s));

}

void ResourceManager::addTexture(const Texture& t) {

    textures.insert(std::pair<std::string, Texture>(t.name, t));

}

Shader* ResourceManager::getShaderByName(const std::string& name) {

    auto it = shaders.find(name);

    if (it == shaders.end()) { // if no shader found
        std::cout << "Searched for shader: " << name << " found null" << std::endl;
        return nullptr;
    }

    return &it->second;

}

Texture* ResourceManager::getTextureByName(const std::string& name) {

    auto it = textures.find(name);

    if (it == textures.end()) { // if no texture found
        return nullptr;
    }

    return &it->second;

}

coordf ResourceManager::getTextureOffset(BlockType blockType) {

    // Return x and y offset for texture-atlas

    if (blockType == BlockType::GRASS)
        return {0, 0};
    if (blockType == BlockType::LEAFS)
        return {0.25f, 0};
    if (blockType == BlockType::GROUND)
        return {0.5f, 0};
    if (blockType == BlockType::SAND)
        return {0.75f, 0};
    if (blockType == BlockType::WOOD)
        return {0, 0.25f};
    if (blockType == BlockType::SANDSTONE)
        return {0.25f, 0.25f};
    else
        return {0, 0};

}
