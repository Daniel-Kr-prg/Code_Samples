#pragma once

//----------------------------------------------------------------------------------------
/**
 * \file    config.h : Config class declaration
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include <string>
#include <vector>
#include <fstream>

#include "json.hpp"
#include "jsonutils.h"
#include "object.h"
#include "directionallight.h"
#include "pointlight.h"
#include "spotlight.h"


 /**
  * \struct ShaderObject
  * \brief Structure representing a shader object.
  */
typedef struct _ShaderObject {
    std::string fragmentPath; /**< Path to the fragment shader file. */
    std::string vertexPath; /**< Path to the vertex shader file. */
} ShaderObject;

/**
 * \class Config
 * \brief Represents the configuration for the application.
 */
class Config
{
public:
    /**
     * \brief Constructor for the Config class.
     * \param filename Name of the configuration file.
     */
    Config(const char* filename);

    /**
     * \brief Destructor for the Config class.
     */
    ~Config();

    /**
     * \brief Retrieves the window width.
     * \return The window width.
     */
    int getWindowWidth();

    /**
     * \brief Retrieves the window height.
     * \return The window height.
     */
    int getWindowHeight();

    /**
     * \brief Retrieves the window title.
     * \return The window title.
     */
    std::string getWindowTitle();

    /**
     * \brief Retrieves the field of view.
     * \return The field of view.
     */
    float getFov();

    /**
     * \brief Retrieves the near clipping plane.
     * \return The near clipping plane.
     */
    float getZNear();

    /**
     * \brief Retrieves the far clipping plane.
     * \return The far clipping plane.
     */
    float getZFar();

    /**
     * \brief Retrieves the camera movement speed.
     * \return The camera movement speed.
     */
    float getSpeed();

    /**
     * \brief Retrieves the key sensitivity for camera movement.
     * \return The key sensitivity for camera movement.
     */
    float getKeySensitivity();

    /**
     * \brief Retrieves the mouse sensitivity for camera movement.
     * \return The mouse sensitivity for camera movement.
     */
    float getMouseSensitivity();

    /**
     * \brief Retrieves the ambient light intensity.
     * \return The ambient light intensity.
     */
    float getAmbientLightIntensity();

    /**
     * \brief Retrieves the ambient light color.
     * \return The ambient light color.
     */
    glm::vec3 getAmbientLightColor();

    /**
     * \brief Retrieves the fog start distance.
     * \return The fog start distance.
     */
    float getFogStart();

    /**
     * \brief Retrieves the fog end distance.
     * \return The fog end distance.
     */
    float getFogEnd();

    /**
     * \brief Retrieves the fog start height.
     * \return The fog start height.
     */
    float getFogHeightStart();

    /**
     * \brief Retrieves the fog end height.
     * \return The fog end height.
     */
    float getFogHeightEnd();

    /**
     * \brief Retrieves the fog color.
     * \return The fog color.
     */
    glm::vec3 getFogColor();

    /**
     * \brief Loads the scene from the configuration file.
     * \param rootNode Root node of the scene.
     */
    void loadScene(ObjectInstance& rootNode);

    /**
     * \brief Creates an object instance based on the specified type.
     * \param typeName Type of the object to create.
     * \return Pointer to the created object instance.
     */
    ObjectInstance* createObjectByType(std::string typeName);

private:
    /**
     * \brief Helper function to recursively process the next object in the JSON data.
     * \param data JSON data to process.
     * \param node Current node to append objects to.
     */
    void getNextObject(nlohmann::json data, ObjectInstance& node);

    const char* configFilename; /**< Name of the configuration file. */

    // Window options
    int windowWidth; /**< Width of the window. */
    int windowHeight; /**< Height of the window. */
    std::string windowTitle; /**< Title of the window. */

    // Camera options
    float fov; /**< Field of view of the camera. */
    float zNear; /**< Near clipping plane of the camera. */
    float zFar; /**< Far clipping plane of the camera. */
    float speed; /**< Movement speed of the camera. */
    float keySensitivity; /**< Key sensitivity for camera movement. */
    float mouseSensitivity; /**< Mouse sensitivity for camera movement. */

    // Light options
    float ambientLightIntensity; /**< Intensity of the ambient light. */
    glm::vec3 ambientLightColor; /**< Color of the ambient light. */
    float fogStart; /**< Start distance of the fog. */
    float fogEnd; /**< End distance of the fog. */
    float fogHeightStart; /**< Start height of the fog. */
    float fogHeightEnd; /**< End height of the fog. */
    glm::vec3 fogColor; /**< Color of the fog. */

    // Graphics objects
    std::vector<ShaderObject> shaders; /**< Vector of shader objects. */
};