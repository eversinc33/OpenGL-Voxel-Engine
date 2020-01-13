//
// Created by sven on 26.04.19.
//

#include "shader.h"
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <iostream>
#include <glm/ext.hpp>
#include <fstream>
#include <sstream>

/*
 * Creates a new Shader.
 * name = name of the shader
 * vShader = vertex shader
 * fShader = fragment shader
 */
Shader::Shader(std::string name, std::string vShaderPath, std::string fShaderPath) {

    this->name = name;
    this->vShaderPath = vShaderPath;
    this->fShaderPath = fShaderPath;
    compileAndLink();

}

Shader::Shader(const Shader& s) {

    this->name = s.name;
    this->vShaderPath = s.vShaderPath;
    this->fShaderPath = s.fShaderPath;
    compileAndLink();

}

void Shader::compileAndLink() {

    std::string vShader, fShader;
    std::ifstream v_inFile, f_inFile;
    std::stringstream v_strStream, f_strStream;

    // Read File for vertexShader
    v_inFile.open(vShaderPath);

    if (!v_inFile.is_open())
    {
        std::cout << "ERROR LOADING VERTEX SHADER FILE: " << vShaderPath << std::endl;
    }
    else
    {
        v_strStream << v_inFile.rdbuf();
        vShader = v_strStream.str();
    }

    // Read File for fragmentShader
    f_inFile.open(fShaderPath);
    if (!f_inFile.is_open())
    {
        std::cout << "ERROR LOADING FRAGMENT SHADER FILE: " << fShaderPath << std::endl;
    }
    else
    {
        f_strStream << f_inFile.rdbuf();
        fShader = f_strStream.str();
    }

    // Compile and link
    const char* vertex_shader = vShader.c_str();
    const char* fragment_shader = fShader.c_str();

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // store reference in ID
    ID = glCreateProgram();

    // link shaders
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // check for linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

/*
 * Tell OpenGL to use this shader-program.
 */
void Shader::use() {

    glUseProgram(ID);

}

/*
 * Utility functions to set uniform variables
 */
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}



