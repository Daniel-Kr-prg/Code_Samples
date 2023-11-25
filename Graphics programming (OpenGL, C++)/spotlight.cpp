//----------------------------------------------------------------------------------------
/**
 * \file    spotlight.cpp : Spot light class implementation. Loads data from config and prepares uniforms for shader
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "spotlight.h"

SpotLight::SpotLight()
{

}

SpotLight::~SpotLight()
{

}

void SpotLight::update(float elapsedTime, const glm::mat4* parentModelMatrix)
{
	ObjectInstance::update(elapsedTime, parentModelMatrix);
	Render::getRender()->setSpotLight(this);
}

void SpotLight::deserialize(nlohmann::json data)
{
	ObjectInstance::deserialize(data);

	if (data.contains("attenuation"))
		attenuation = data["attenuation"].get<float>();
	if (data.contains("intensity"))
		intensity = data["intensity"].get<float>();
	if (data.contains("lightColor"))
		lightColor = readVectorFromJSON(data["lightColor"], glm::vec3(1));
	if (data.contains("innerCutoff"))
		innerCutoff = data["innerCutoff"].get<float>();
	if (data.contains("outerCutoff"))
		outerCutoff = data["outerCutoff"].get<float>();
}

void SpotLight::setUniforms(Shader* shader)
{
	glUniform1f(shader->getShaderData().locations.spotLightAttenuation, attenuation);
	glUniform3fv(shader->getShaderData().locations.spotLightPosition, 1, glm::value_ptr(globalPosition));
	glUniform3fv(shader->getShaderData().locations.spotLightDirection, 1, glm::value_ptr(globalForward));
	glUniform1f(shader->getShaderData().locations.spotLightIntensity, intensity);
	glUniform3fv(shader->getShaderData().locations.spotLightColor, 1, glm::value_ptr(lightColor));
	glUniform1f(shader->getShaderData().locations.spotLightInnerCutoff, innerCutoff);
	glUniform1f(shader->getShaderData().locations.spotLightOuterCutoff, outerCutoff);
}