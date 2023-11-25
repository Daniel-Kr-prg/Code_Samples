#pragma once
//----------------------------------------------------------------------------------------
/**
\file skyboxshader.h : SkyboxShader class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include <fstream>
#include <sstream>
#include <string>
#include <pgr.h>
/**
 * \brief Shader program related stuff (id, locations, ...).
 */
typedef struct _SkyShaderProgram {
	/// identifier for the shader program
	GLuint program;

	bool initialized;

	/**
	  * \brief Indices of the vertex shader inputs (locations)
	  */
	struct {
		// vertex attributes locations
		GLint position;
		// uniforms locations
		GLint PMatrix;
		GLint VMatrix;
		GLint skySampler;
		// Fog
		GLint fogHeightStart;
		GLint fogHeightEnd;
		GLint fogColor;
	} locations;

	// ...

	_SkyShaderProgram() : program(0), initialized(false) {
		locations.position = -1;
		locations.PMatrix = -1;
		locations.VMatrix = -1;
		locations.skySampler = -1;
	}

} SkyShaderProgram;

/**
 * \brief Represents a skybox shader program.
 */
class SkyBoxShader
{
public:
	/**
	 * \brief Constructs a SkyBoxShader object.
	 * \param vertexFilename The filename of the vertex shader.
	 * \param fragmentFilename The filename of the fragment shader.
	 */
	SkyBoxShader(const char* vertexFilename, const char* fragmentFilename);

	/**
	 * \brief Destroys the SkyBoxShader object.
	 */
	~SkyBoxShader();

	/**
	 * \brief Returns the SkyShaderProgram data.
	 * \return The SkyShaderProgram data.
	 */
	SkyShaderProgram getShaderData();

private:
	SkyShaderProgram shaderData; ///< The SkyShaderProgram data.
};