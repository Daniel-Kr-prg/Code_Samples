#pragma once
//----------------------------------------------------------------------------------------
/**
\file spotlight.h : SpotLight class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include "object.h"
#include "json.hpp"
#include "config.h"
#include "render.h"

/**
 * \brief Represents a SpotLight that emits light in a specific direction.
 */
class SpotLight : public ObjectInstance {
public:
	/**
	 * \brief Constructs a SpotLight object.
	 */
	SpotLight();

	/**
	 * \brief Destroys the SpotLight object.
	 */
	~SpotLight();

	/**
	 * \brief Updates the SpotLight.
	 * \param elapsedTime The elapsed time since the last update.
	 * \param parentModelMatrix The model matrix of the parent object.
	 */
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;

	/**
	 * \brief Deserializes the SpotLight from JSON data.
	 * \param data The JSON data representing the SpotLight.
	 */
	void deserialize(nlohmann::json data) override;

	/**
	 * \brief Sets the uniforms of the SpotLight in the given shader.
	 * \param shader The shader to set the uniforms in.
	 */
	void setUniforms(Shader* shader);
private:
	float attenuation;     ///< The attenuation of the light
	float intensity;       ///< The intensity of the light
	glm::vec3 lightColor;  ///< The color of the light
	float innerCutoff;     ///< The inner cutoff angle of the light cone
	float outerCutoff;     ///< The outer cutoff angle of the light cone
};

