//----------------------------------------------------------------------------------------
/**
 * \file    splashshader.cpp : Prepares splash shader for use
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "splashShader.h"

SplashShader::SplashShader(const char* vertexFilename, const char* fragmentFilename)
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
		shaderData.locations.texCoord = glGetAttribLocation(shaderData.program, "texCoord");

		// other attributes and uniforms
		shaderData.locations.Pmatrix = glGetUniformLocation(shaderData.program, "projection");
		shaderData.locations.Vmatrix = glGetUniformLocation(shaderData.program, "view");
		shaderData.locations.Mmatrix = glGetUniformLocation(shaderData.program, "model");
		shaderData.locations.time = glGetUniformLocation(shaderData.program, "time");
		shaderData.locations.texSampler = glGetUniformLocation(shaderData.program, "texSampler");
		shaderData.locations.frameDuration = glGetUniformLocation(shaderData.program, "frameDuration");

		assert(shaderData.locations.position != -1);
		assert(shaderData.locations.texCoord != -1);
		assert(shaderData.locations.Pmatrix != -1);
		assert(shaderData.locations.Vmatrix != -1);
		assert(shaderData.locations.Mmatrix != -1);
		assert(shaderData.locations.time != -1);
		assert(shaderData.locations.texSampler != -1);
		assert(shaderData.locations.frameDuration != -1);
		// ...
	}
}

SplashShaderProgram SplashShader::getShaderData()
{
	return shaderData;
}

SplashShader::~SplashShader()
{
	pgr::deleteProgramAndShaders(shaderData.program);
}