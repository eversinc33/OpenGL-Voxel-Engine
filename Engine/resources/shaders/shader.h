//
// Created by sven on 26.04.19.
//

#ifndef OPENGLPRACTICE_SHADER_H
#define OPENGLPRACTICE_SHADER_H

#include <string>
#include <glm/ext.hpp>

class Shader {
public:
    unsigned int ID;
    std::string vShaderPath;
    std::string fShaderPath;
    std::string name, vShader, fShader;

    Shader(std::string name, std::string vShader, std::string fShader);
    Shader(const Shader& s); // Copy cons

    void compileAndLink();
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, float x, float y, float z, float w);
    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setVec4(const std::string &name, const glm::vec4 &value) const;
};


#endif //OPENGLPRACTICE_SHADER_H
