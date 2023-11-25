//----------------------------------------------------------------------------------------
/**
 * \file    splash.cpp : Splash effect implementation. drawas splash and sets up position for splash
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "splash.h"
#include "render.h"
#include "camera.h"

Splash::Splash(SplashShader* shader, std::string textureName, float frameDuration)
{
	this->shader = shader;
	this->frameDuration = frameDuration;
	geometry = new ObjectGeometry;

	geometry->texture = pgr::createTexture(textureName);

	glGenVertexArrays(1, &(geometry->vertexArrayObject));
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &(geometry->vertexBufferObject)); \
		glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(splashVertexData), splashVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shader->getShaderData().locations.position);
	// vertices of triangles - start at the beginning of the array (interlaced array)
	glVertexAttribPointer(shader->getShaderData().locations.position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(shader->getShaderData().locations.texCoord);
	// texture coordinates are placed just after the position of each vertex (interlaced array)
	glVertexAttribPointer(shader->getShaderData().locations.texCoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	geometry->numTriangles = splashNumQuadVertices;
}

Splash::~Splash()
{

}

void Splash::splashAtPosition(glm::vec3 position)
{
	if (!isDrawing)
	{
		isDrawing = true;
		drawStartTime = elapsedTime;
		drawPosition = position;
	}
}

void Splash::update(float elapsedTime)
{
	this->elapsedTime = elapsedTime;

	if (elapsedTime - drawStartTime > 50 * frameDuration)
	{
		isDrawing = false;
	}
}

void Splash::draw()
{
	if (isDrawing)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(shader->getShaderData().program);

		//  // just take 3x3 rotation part of the view transform
		Camera* camera = Render::getRender()->getRootNode()->firstNodeByType<Camera>();

		glm::mat4 rotationMatrix = glm::mat4(
			camera->getView()[0],
			camera->getView()[1],
			camera->getView()[2],
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
		// inverse view rotation
		rotationMatrix = glm::transpose(rotationMatrix);

		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, drawPosition);
		matrix = glm::scale(matrix, glm::vec3(0.2));

		matrix = matrix * rotationMatrix;

		glUniformMatrix4fv(shader->getShaderData().locations.Mmatrix, 1, GL_FALSE, glm::value_ptr(matrix));
		glUniformMatrix4fv(shader->getShaderData().locations.Vmatrix, 1, GL_FALSE, glm::value_ptr(camera->getView()));
		glUniformMatrix4fv(shader->getShaderData().locations.Pmatrix, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));

		glUniform1f(shader->getShaderData().locations.time, elapsedTime - drawStartTime);
		glUniform1i(shader->getShaderData().locations.texSampler, 0);
		glUniform1f(shader->getShaderData().locations.frameDuration, frameDuration);

		glBindVertexArray(geometry->vertexArrayObject);
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry->numTriangles);
		CHECK_GL_ERROR();
		glBindVertexArray(0);
		glUseProgram(0);

		glDisable(GL_BLEND);
	}

	return;
}