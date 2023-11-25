#pragma once

//----------------------------------------------------------------------------------------
/**
 * \file    directionallight.h : Directional light class declaration
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "object.h"
#include "json.hpp"
#include "config.h"
#include "render.h"

 /**
  * \class DirectionalLight
  * \brief Represents a directional light in the scene.
  */
class DirectionalLight : public ObjectInstance
{
public:
    /**
     * \brief Constructor for the DirectionalLight class.
     */
    DirectionalLight();

    /**
     * \brief Destructor for the DirectionalLight class.
     */
    ~DirectionalLight();

    /**
     * \brief Updates the directional light.
     * Overrides the update method from the ObjectInstance class.
     * \param elapsedTime Elapsed time since the last update.
     * \param parentModelMatrix Pointer to the parent model matrix.
     */
    void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;

    /**
     * \brief Deserializes the directional light from JSON data.
     * Overrides the deserialize method from the ObjectInstance class.
     * \param data JSON data to deserialize from.
     */
    void deserialize(nlohmann::json data) override;

    /**
     * \brief Sets the uniforms for the directional light in the specified shader.
     * \param shader Pointer to the shader to set the uniforms in.
     */
    void setUniforms(Shader* shader);

private:
    float intensity; /**< Intensity of the directional light. */
    glm::vec3 lightColor; /**< Color of the directional light. */
};