#pragma once
//----------------------------------------------------------------------------------------
/**
\file pointlight.h : PointLight class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include "object.h"
#include "json.hpp"
#include "config.h"
#include "render.h"

class PointLight : public ObjectInstance
{
public:
    /**
     * \brief Constructor for the PointLight class.
     */
    PointLight();

    /**
     * \brief Destructor for the PointLight class.
     */
    ~PointLight();

    /**
     * \brief Updates the point light.
     * \param elapsedTime The elapsed time since the last update.
     * \param parentModelMatrix Pointer to the parent's model matrix.
     */
    void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;

    /**
     * \brief Deserializes the point light from JSON data.
     * \param data The JSON data to deserialize.
     */
    void deserialize(nlohmann::json data) override;

    /**
     * \brief Sets the uniforms of the point light in the specified shader.
     * \param shader The shader program to set the uniforms in.
     */
    void setUniforms(Shader* shader);
private:
    float attenuation; /**< Attenuation factor of the point light. */
    float intensity; /**< Intensity of the point light. */
    glm::vec3 lightColor; /**< Color of the point light. */
};

