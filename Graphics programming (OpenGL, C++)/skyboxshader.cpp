//----------------------------------------------------------------------------------------
/**
 * \file    skyboxshader.cpp : Skybox shader implementation
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "skyboxshader.h"

SkyBoxShader::SkyBoxShader(const char* vertexFilename, const char* fragmentFilename)
{
	std::ifstream vertexFile(vertexFilename);
	std::ifstream fragmentFile(fragmentFilename);

	if (vertexFile.is_open() && fragmentFile.is_open())
	{
		std::ostringstream vertexShaderSrc;
		std::ostringstream fragmentShaderSrc;

		vertexShaderSrc << vertexFile.rdbuf();
		fragmentShaderSrc << fragmentFile.rdbuf();

		GLuint shaders[] = {
			pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc.str()),
			pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc.str()),
			0
		};

		shaderData.program = pgr::createProgram(shaders);
		shaderData.locations.position = glGetAttribLocation(shaderData.program, "position");

		// other attributes and uniforms
		shaderData.locations.PMatrix = glGetUniformLocation(shaderData.program, "projection");
		shaderData.locations.VMatrix = glGetUniformLocation(shaderData.program, "view");
		shaderData.locations.skySampler = glGetUniformLocation(shaderData.program, "skySampler");
		shaderData.locations.fogHeightStart = glGetUniformLocation(shaderData.program, "fogHeightStart");
		shaderData.locations.fogHeightEnd = glGetUniformLocation(shaderData.program, "fogHeightEnd");
		shaderData.locations.fogColor = glGetUniformLocation(shaderData.program, "fogColor");

		assert(shaderData.locations.position != -1);
		assert(shaderData.locations.PMatrix != -1);
		assert(shaderData.locations.VMatrix != -1);
		assert(shaderData.locations.skySampler != -1);
		// ...

		shaderData.initialized = true;
	}
}

SkyShaderProgram SkyBoxShader::getShaderData()
{
	return shaderData;
}

SkyBoxShader::~SkyBoxShader()
{
	pgr::deleteProgramAndShaders(shaderData.program);
}