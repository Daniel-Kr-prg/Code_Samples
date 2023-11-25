//----------------------------------------------------------------------------------------
/**
 * \file    water.cpp : Water object implementation.
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "water.h"

Water::Water()
{
	setIndex(4);
}

void Water::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	// Texture translation
	texCoordMatrix[2][0] = 0.5 * elapsedTime;
	texCoordMatrix[2][1] = 0.5 * elapsedTime;
	// Texture rotation
	float rotation = std::remainder(0.01 * elapsedTime, 6.28);

	texCoordMatrix[0][0] = std::cos(rotation);
	texCoordMatrix[0][1] = -std::sin(rotation);
	texCoordMatrix[1][0] = std::sin(rotation);
	texCoordMatrix[1][1] = std::cos(rotation);

	SingleMesh::update(elapsedTime, parentModelMatrix);
}

void Water::draw()
{
	if (initialized && (shaderProgram != nullptr)) {

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilFunc(GL_ALWAYS, getIndex(), 0);

		glUseProgram(shaderProgram->getShaderData().program);

		Render::getRender()->setMaterialUniforms(
			shaderProgram->getShaderData(),
			geometry->ambient,
			geometry->diffuse,
			geometry->specular,
			geometry->shininess,
			geometry->texture
		);

		Render::getRender()->setCameraAndLightsUniforms(shaderProgram);

		glUniformMatrix3fv(shaderProgram->getShaderData().locations.texCoordMatrix, 1, GL_FALSE, glm::value_ptr(texCoordMatrix));
		glUniformMatrix4fv(shaderProgram->getShaderData().locations.normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(globalModelMatrix))));
		glUniformMatrix4fv(shaderProgram->getShaderData().locations.MMatrix, 1, GL_FALSE, glm::value_ptr(globalModelMatrix));

		glBindVertexArray(geometry->vertexArrayObject);
		glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDisable(GL_STENCIL_TEST);

		ObjectInstance::draw();
	}
	else {
		std::cerr << "SingleMesh::draw(): Can't draw, mesh not initialized properly!" << std::endl;
	}
}

Water::~Water()
{

}

void Water::Interact()
{

}