//
// Created by sven on 04.05.19.
//

#include <iostream>
#include "Renderer.h"
#include "../gameobjects/drawables/Skybox.h"
#include "../resources/resourcemanager.h"
#include "../utilities/structs/coord.h"

Renderer::Renderer(unsigned int* VBO, Camera* camera, const unsigned int& SCR_WIDTH, const unsigned int& SCR_HEIGHT) {

    this->VBO = VBO;
    this->camera = camera;
    this->projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

}

void Renderer::setupGL() {

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // gen buffer
    glGenBuffers(1, VBO);

}

void Renderer::renderChunk(Chunk* c, int xOffset, int zOffset) {

    int numOfBlocks = Chunk::CHUNK_SIZE*Chunk::CHUNK_SIZE*Chunk::CHUNK_SIZE;
    std::vector<float> vertices = std::vector<float>();

    // use block shader
    Shader* s = ResourceManager::getInstance().getShaderByName("TEX_LIGHTED");
    s->use();

    // generate gl objects
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    // setup attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // add each cubes vertex data to VAO
    // ---------------------------------

    int nBlocks = 0;

    // only recalculate if it has changed, else use cached vertices
    if (c->hasChanged || c->firstRender)
    {
        for (int x=0; x<Chunk::CHUNK_SIZE; x++) {

            for (int y=0; y<Chunk::CHUNK_SIZE; y++) {

                for (int z=0; z<Chunk::CHUNK_SIZE; z++) {

                    if (c->blocks[x][y][z] != BlockType::AIR)
                    {

                        addCube(x, y, z, c->blocks[x][y][z], &vertices, xOffset, zOffset);
                        nBlocks++;

                    }
                }
            }
        }

        c->cachedVertices = vertices;
        c->firstRender = false;

    }
    else
    {
        vertices = c->cachedVertices;
    }

    // bind to buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ResourceManager::getTextureByName("ATLAS")->ID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ResourceManager::getTextureByName("NONE")->ID);

    // light properties
    s->setVec3("light.position", camera->Position); // torch
    s->setVec3("light.direction", -1.0f, -1.0f, -1.0f); // global light
    s->setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
    s->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    s->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    s->setFloat("material.shininess", 64.0f);

    // Bind Texture Maps
    s->setInt("material.diffuse", 0); // not IDs but texture unit numbers
    s->setInt("material.specular", 1);

    // view/projection transformations
    glm::mat4 view = camera->getViewMatrix();
    s->setMat4("projection", projection);
    s->setVec3("viewPos", camera->Position);
    s->setMat4("view", view);

    // world transformation (1.0 = ignore, data is stored in vertices)
    glm::mat4 model = glm::mat4(1.0f);
    s->setMat4("model", model);

    // ---------------------
    // Draw all
    // ---------------------

    glDrawArrays(GL_TRIANGLES, 0, (vertices.size()/288)*36);

}

void Renderer::addCube(int posX, int posY, int posZ, BlockType type, std::vector<float>* vertices, int xOffset, int zOffset) {

    // Add Cube Vertices to ChunkMesh

    float x = posX*1.0f;
    float y = posY*1.0f;
    float z = posZ*1.0f;

    xOffset *= Chunk::CHUNK_SIZE;
    zOffset *= Chunk::CHUNK_SIZE;

    coordf texCoords = ResourceManager::getTextureOffset(type);
    float tex_x = texCoords.x;
    float tex_y = texCoords.z;

    std::vector<float> newVertices = {
            // Back face
            -0.5f+x+xOffset, -0.5f+y, -0.5f+z+zOffset, 0.0f,  0.0f, -1.0f,  0.0f+tex_x, 0.0f+tex_y, // Bottom-left
            0.5f+x+xOffset,  0.5f+y, -0.5f+z+zOffset,  0.0f,  0.0f, -1.0f,  1/ResourceManager::atlasRows+tex_x, 1/ResourceManager::atlasRows+tex_y, // top-right
            0.5f+x+xOffset, -0.5f+y, -0.5f+z+zOffset,  0.0f, 0.0f, -1.0f,   1/ResourceManager::atlasRows+tex_x, 0.0f+tex_y,// bottom-right
            0.5f+x+xOffset,  0.5f+y, -0.5f+z+zOffset,  0.0f, 0.0f, -1.0f,  1/ResourceManager::atlasRows+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-right
            -0.5f+x+xOffset, -0.5f+y, -0.5f+z+zOffset, 0.0f, 0.0f, -1.0f,  0.0f+tex_x, 0.0f+tex_y, // bottom-left
            -0.5f+x+xOffset,  0.5f+y, -0.5f+z+zOffset, 0.0f, 0.0f, -1.0f,  0.0f+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-left
            // Front face
            -0.5f+x+xOffset, -0.5f+y,  0.5f+z+zOffset, 0.0f,  0.0f, 1.0f, 0.0f+tex_x, 0.0f+tex_y,// bottom-left
            0.5f+x+xOffset, -0.5f+y,  0.5f+z+zOffset,  0.0f,  0.0f, 1.0f, 1/ResourceManager::atlasRows+tex_x, 0.0f+tex_y,// bottom-right
            0.5f+x+xOffset,  0.5f+y,  0.5f+z+zOffset,  0.0f,  0.0f, 1.0f, 1/ResourceManager::atlasRows+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-right
            0.5f+x+xOffset,  0.5f+y,  0.5f+z+zOffset,  0.0f,  0.0f, 1.0f, 1/ResourceManager::atlasRows+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-right
            -0.5f+x+xOffset,  0.5f+y,  0.5f+z+zOffset, 0.0f,  0.0f, 1.0f, 0.0f+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-left
            -0.5f+x+xOffset, -0.5f+y,  0.5f+z+zOffset, 0.0f,  0.0f, 1.0f, 0.0f+tex_x, 0.0f+tex_y,// bottom-left
            // Left face
            -0.5f+x+xOffset,  0.5f+y,  0.5f+z+zOffset, -1.0f,  0.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-right
            -0.5f+x+xOffset,  0.5f+y, -0.5f+z+zOffset, -1.0f,  0.0f,  0.0f, 0.0f+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-left
            -0.5f+x+xOffset, -0.5f+y, -0.5f+z+zOffset, -1.0f,  0.0f,  0.0f, 0.0f+tex_x, 0.0f+tex_y,// bottom-left
            -0.5f+x+xOffset, -0.5f+y, -0.5f+z+zOffset, -1.0f,  0.0f,  0.0f, 0.0f+tex_x, 0.0f+tex_y,// bottom-left
            -0.5f+x+xOffset, -0.5f+y,  0.5f+z+zOffset, -1.0f,  0.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 0.0f+tex_y,// bottom-right
            -0.5f+x+xOffset,  0.5f+y,  0.5f+z+zOffset, -1.0f,  0.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-right
            // Right face
            0.5f+x+xOffset,  0.5f+y,  0.5f+z+zOffset,  1.0f,  0.0f,  0.0f, 0.0f+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-left
            0.5f+x+xOffset, -0.5f+y, -0.5f+z+zOffset,  1.0f,  0.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 0.0f+tex_y,// bottom-right
            0.5f+x+xOffset,  0.5f+y, -0.5f+z+zOffset,  1.0f,  0.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-right
            0.5f+x+xOffset, -0.5f+y, -0.5f+z+zOffset,  1.0f,  0.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 0.0f+tex_y,// bottom-right
            0.5f+x+xOffset,  0.5f+y,  0.5f+z+zOffset,  1.0f,  0.0f,  0.0f, 0.0f+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-left
            0.5f+x+xOffset, -0.5f+y,  0.5f+z+zOffset,  1.0f,  0.0f,  0.0f, 0.0f+tex_x, 0.0f+tex_y,// bottom-left
            // Bottom face
            -0.5f+x+xOffset, -0.5f+y, -0.5f+z+zOffset, 0.0f, -1.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-right
            0.5f+x+xOffset, -0.5f+y, -0.5f+z+zOffset,  0.0f, -1.0f,  0.0f, 0.0f+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-left
            0.5f+x+xOffset, -0.5f+y,  0.5f+z+zOffset,  0.0f, -1.0f,  0.0f, 0.0f+tex_x, 0.0f+tex_y,// bottom-left
            0.5f+x+xOffset, -0.5f+y,  0.5f+z+zOffset,  0.0f, -1.0f,  0.0f, 0.0f+tex_x, 0.0f+tex_y,// bottom-left
            -0.5f+x+xOffset, -0.5f+y,  0.5f+z+zOffset, 0.0f, -1.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 0.0f+tex_y,// bottom-right
            -0.5f+x+xOffset, -0.5f+y, -0.5f+z+zOffset, 0.0f, -1.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-right
            // Top face
            -0.5f+x+xOffset,  0.5f+y, -0.5f+z+zOffset, 0.0f,  1.0f,  0.0f, 0.0f+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-left
            0.5f+x+xOffset,  0.5f+y,  0.5f+z+zOffset,  0.0f,  1.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 0.0f+tex_y,// bottom-right
            0.5f+x+xOffset,  0.5f+y, -0.5f+z+zOffset,  0.0f,  1.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-right
            0.5f+x+xOffset,  0.5f+y,  0.5f+z+zOffset,  0.0f,  1.0f,  0.0f, 1/ResourceManager::atlasRows+tex_x, 0.0f+tex_y,// bottom-right
            -0.5f+x+xOffset,  0.5f+y, -0.5f+z+zOffset, 0.0f,  1.0f,  0.0f, 0.0f+tex_x, 1/ResourceManager::atlasRows+tex_y,// top-left
            -0.5f+x+xOffset,  0.5f+y,  0.5f+z+zOffset, 0.0f,  1.0f,  0.0f, 0.0f+tex_x, 0.0f+tex_y // bottom-left
    };

    // add data
    vertices->insert(vertices->end(), newVertices.begin(), newVertices.end() );

}

void Renderer::render(Drawable* d) {

    if (d->cat == Drawable::Category::NO_DRAW)
        return;

    // --------------------------------------------------------------------

    else if (d->cat == Drawable::Category::SKYBOX)
    {

        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
        d->setupBuffers();

        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

        // use shader
        d->shader->use();

        // view/projection transformations
        glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMatrix()));
        d->shader->setMat4("projection", projection);
        d->shader->setVec3("viewPos", camera->Position);
        d->shader->setMat4("view", view);

        // render skybox
        d->draw();

        glDepthFunc(GL_LESS); // set depth function back to default

    }

    // Set Vars
    //lastTexture = d->texture->name;
    //lastDrawn = d->shader->name;
}
