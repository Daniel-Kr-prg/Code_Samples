//----------------------------------------------------------------------------------------
/**
 * \file    directionallight.cpp : directional light initialization and updating
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "directionallight.h"

DirectionalLight::DirectionalLight()
{

}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::update(float elapsedTime, const glm::mat4* parentModelMatrix)
{
	ObjectInstance::update(elapsedTime, parentModelMatrix);
	Render::getRender()->setDirectionalLight(this);
}

void DirectionalLight::deserialize(nlohmann::json data)
{
	ObjectInstance::deserialize(data);

	if (data.contains("intensity"))
		intensity = data["intensity"].get<float>();
	if (data.contains("lightColor"))
		lightColor = readVectorFromJSON(data["lightColor"], glm::vec3(1));


}

void DirectionalLight::setUniforms(Shader* shader)
{
	glUniform3fv(shader->getShaderData().locations.directionalLightDirection, 1, glm::value_ptr(globalForward));
	glUniform1f(shader->getShaderData().locations.directionalLightIntensity, intensity);
	glUniform3fv(shader->getShaderData().locations.directionalLightColor, 1, glm::value_ptr(lightColor));
}