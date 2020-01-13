//
// Created by sven on 04.05.19.
//

#ifndef OPENGL_ENGINE_RESOURCEMANAGER_H
#define OPENGL_ENGINE_RESOURCEMANAGER_H


#include <map>
#include "shaders/shader.h"
#include "textures/texture.h"
#include "../utilities/enums/block.h"
#include "../utilities/structs/coord.h"

class ResourceManager {
public:
    // Singleton Class
    ResourceManager() = default;
    static ResourceManager& getInstance()
    {
        static ResourceManager instance;
        return instance;
    }
    // Dont allow copies
    ResourceManager(ResourceManager const&) = delete;
    void operator=(ResourceManager const&)  = delete;

    inline static std::string SHADER_DIR;
    inline static std::string TEXTURE_DIR;
    inline static std::map<std::string, Shader> shaders;
    inline static std::map<std::string, Texture> textures;

    static coordf getTextureOffset(BlockType blockType);
    static void loadResources();
    static Shader*  getShaderByName(const std::string& name);
    static Texture* getTextureByName(const std::string& name);

    static constexpr float atlasRows = 4.0f;

private:
    static void setupShaders();
    static void setupTextures();
    static void addShader(const Shader& s);
    static void addTexture(const Texture& t);

};


#endif //OPENGL_ENGINE_RESOURCEMANAGER_H
