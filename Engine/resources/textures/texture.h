//
// Created by sven on 28.04.19.
//

#ifndef OPENGL_ENGINE_TEXTURE_H
#define OPENGL_ENGINE_TEXTURE_H


class Texture {
public:
    Texture(std::string name, std::string path);
    Texture(std::string name, std::string faces[]); // skymap constructor

    unsigned int ID;
    std::string name, path;

    void createTexture();
    void use();

    void createSkymap(std::string faces[]);
};


#endif //OPENGL_ENGINE_TEXTURE_H
