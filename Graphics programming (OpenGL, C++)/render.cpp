//----------------------------------------------------------------------------------------
/**
 * \file    render.cpp : Render class implementation. Contains all rendering methods
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include <time.h>
#include <list>
#include "pgr.h"
#include <iostream>
#include "singlemesh.h"
#include "render.h"
#include "directionallight.h"
#include "pointlight.h"
#include "spotlight.h"
#include "splash.h"


static Render render;

Render* Render::getRender()
{
    return &render;
}

ObjectInstance* Render::getRootNode()
{
    return &rootNode;
}

void Render::setMaterialUniforms(const ShaderProgram& shaderProgram, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, GLuint texture) {

    glUniform3fv(shaderProgram.locations.diffuse, 1, glm::value_ptr(diffuse));
    glUniform3fv(shaderProgram.locations.ambient, 1, glm::value_ptr(ambient));
    glUniform3fv(shaderProgram.locations.specular, 1, glm::value_ptr(specular));
    glUniform1f(shaderProgram.locations.shininess, shininess);

    if (texture != 0) {
        glUniform1i(shaderProgram.locations.useTexture, 1);  
        glUniform1i(shaderProgram.locations.texSampler, 0);  
        glActiveTexture(GL_TEXTURE0 + 0);                  
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    else {
        glUniform1i(shaderProgram.locations.useTexture, 0);  
    }
}

void Render::initializeSkyboxGeometry(std::string skyVertexPath, std::string skyFragmentPath, std::string filePath)
{
    skyShader = new SkyBoxShader(skyVertexPath.c_str(), skyFragmentPath.c_str());
    skyboxGeometry = new ObjectGeometry();

    float skyboxCube[] = {         
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &(skyboxGeometry->vertexArrayObject));
    glBindVertexArray(skyboxGeometry->vertexArrayObject);

    glGenBuffers(1, &(skyboxGeometry->vertexBufferObject));
    glBindBuffer(GL_ARRAY_BUFFER, skyboxGeometry->vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxCube), skyboxCube, GL_STATIC_DRAW);

    glEnableVertexAttribArray(skyShader->getShaderData().locations.position);
    glVertexAttribPointer(skyShader->getShaderData().locations.position, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    CHECK_GL_ERROR();

    skyboxGeometry->numTriangles = 12;

    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &(skyboxGeometry->texture));
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxGeometry->texture);

    const char* suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
    GLuint targets[] = {
      GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
      GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
      GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    for (int i = 0; i < 6; i++) {
        std::string texName = filePath + "_" + suffixes[i] + ".jpg";
        std::cout << "Loading cube map texture: " << texName << std::endl;
        if (!pgr::loadTexImage2D(texName, targets[i])) {
            pgr::dieWithError("Skybox cube map loading failed!");
        }
    }

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    CHECK_GL_ERROR();
}

void Render::splashAtPosition(glm::vec3 position)
{
    splash->splashAtPosition(position);
}

void Render::update(float elapsedTime)
{
    if (splash != nullptr)
    {
        splash->update(elapsedTime);
    }
}

void Render::endDraw()
{
    //Draw skybox
    glDepthFunc(GL_LEQUAL);
    glUseProgram(skyShader->getShaderData().program);
    glUniformMatrix4fv(skyShader->getShaderData().locations.PMatrix, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
    glUniformMatrix4fv(skyShader->getShaderData().locations.VMatrix, 1, GL_FALSE, glm::value_ptr(camera->getView()));

    //Fog
    glUniform1f(skyShader->getShaderData().locations.fogHeightStart, config->getFogHeightStart());
    glUniform1f(skyShader->getShaderData().locations.fogHeightEnd, config->getFogHeightEnd());
    glUniform3fv(skyShader->getShaderData().locations.fogColor, 1, glm::value_ptr(config->getFogColor()));
    
    // skybox cube
    glBindVertexArray(skyboxGeometry->vertexArrayObject);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxGeometry->texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    if (splash != nullptr)
    {
        splash->draw();
    }
}

void cleanupGeometry(ObjectGeometry* geometry) {

    glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
    glDeleteBuffers(1, &(geometry->elementBufferObject));
    glDeleteBuffers(1, &(geometry->vertexBufferObject));

    if (geometry->texture != 0)
        glDeleteTextures(1, &(geometry->texture));
}

void Render::initialize(Config* config)
{
    if (config != nullptr)
    {
        this->config = config;
        initialized = true;
    }
}

void Render::addShader(Shader* shader) {
    shaders.push_back(shader);
}

void Render::setSplash(Splash* splash)
{
    this->splash = splash;
}

void Render::setCamera(Camera* camera)
{
    this->camera = camera;
}

void Render::setDirectionalLight(DirectionalLight* light)
{
    directionalLight = light;
}

void Render::setPointLight(PointLight* light)
{
    pointLight = light;
}

void Render::setSpotLight(SpotLight* light)
{
    spotLight = light;
}

void Render::setCameraAndLightsUniforms(Shader* shader)
{
    // Camera
    glUniform3fv(shader->getShaderData().locations.viewPosition, 1, glm::value_ptr(camera->getGlobalPosition()));
    glUniformMatrix4fv(shader->getShaderData().locations.PMatrix, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
    glUniformMatrix4fv(shader->getShaderData().locations.VMatrix, 1, GL_FALSE, glm::value_ptr(camera->getView()));

    //Fog
    glUniform1f(shader->getShaderData().locations.fogStart, config->getFogStart());
    glUniform1f(shader->getShaderData().locations.fogEnd, config->getFogEnd());
    glUniform3fv(shader->getShaderData().locations.fogColor, 1, glm::value_ptr(config->getFogColor()));

    // Ambient light
    glUniform1f(shader->getShaderData().locations.ambientLightIntensity, config->getAmbientLightIntensity());
    glUniform3fv(shader->getShaderData().locations.ambientLightColor, 1, glm::value_ptr(config->getAmbientLightColor()));

    // Directional light
    if (directionalLight != nullptr)
    {
        directionalLight->setUniforms(shader);
    }
    // Point light
    if (pointLight != nullptr)
    {
        pointLight->setUniforms(shader);
    }
    // Spot light
    if (spotLight != nullptr)
    {
        spotLight->setUniforms(shader);
    }
}

float Render::getCurrentAspect()
{
    if (!initialized)
        throw "Render::getCurrentAspect(): Render is not initialized";
    return (float)config->getWindowWidth() / config->getWindowHeight();
}

Shader* Render::getShader(int index)
{
    if (shaders.size() > index && index != -1)
    {
        return shaders[index];
    }
    else
    {
        return nullptr;
    }
}