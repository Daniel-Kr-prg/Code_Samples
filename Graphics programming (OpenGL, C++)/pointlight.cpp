//----------------------------------------------------------------------------------------
/**
 * \file    pointlight.cpp : Point light initialization and updating
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "pointlight.h"

PointLight::PointLight()
{

}

PointLight::~PointLight()
{

}

void PointLight::update(float elapsedTime, const glm::mat4* parentModelMatrix)
{
	ObjectInstance::update(elapsedTime, parentModelMatrix);
	Render::getRender()->setPointLight(this);
}

void PointLight::deserialize(nlohmann::json data)
{
	ObjectInstance::deserialize(data);

	if (data.contains("attenuation"))
		attenuation = data["attenuation"].get<float>();
	if (data.contains("intensity"))
		intensity = data["intensity"].get<float>();
	if (data.contains("lightColor"))
		lightColor = readVectorFromJSON(data["lightColor"], glm::vec3(1));
}

void PointLight::setUniforms(Shader* shader)
{
	glUniform1f(shader->getShaderData().locations.pointLightAttenuation, attenuation);
	glUniform3fv(shader->getShaderData().locations.pointLightPosition, 1, glm::value_ptr(globalPosition));
	glUniform1f(shader->getShaderData().locations.pointLightIntensity, intensity);
	glUniform3fv(shader->getShaderData().locations.pointLightColor, 1, glm::value_ptr(lightColor));
}