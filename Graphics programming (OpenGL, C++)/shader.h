#pragma once
//----------------------------------------------------------------------------------------
/**
\file shader.h : Shader class declaration
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
typedef struct _ShaderProgram {
	/// identifier for the shader program
	GLuint program;

	bool initialized;

	/**
	  * \brief Indices of the vertex shader inputs (locations)
	  */
	struct {
		// vertex attributes locations
		GLint position;
		GLint normal;
		GLint texCoord;
		// uniforms locations
		GLint PMatrix;
		GLint VMatrix;
		GLint MMatrix;
		GLint normalMatrix;
		GLint texCoordMatrix;
		GLint time;
		GLint diffuse;
		GLint ambient;
		GLint specular;
		GLint shininess;
		GLint texSampler;
		GLint useTexture;
		// camera
		GLint viewPosition;
		// light
		GLint ambientLightIntensity;
		GLint ambientLightColor;
		GLint directionalLightDirection;
		GLint directionalLightIntensity;
		GLint directionalLightColor;
		GLint pointLightAttenuation;
		GLint pointLightPosition;
		GLint pointLightIntensity;
		GLint pointLightColor;
		GLint spotLightAttenuation;
		GLint spotLightPosition;
		GLint spotLightDirection;
		GLint spotLightIntensity;
		GLint spotLightColor;
		GLint spotLightInnerCutoff;
		GLint spotLightOuterCutoff;

		GLint fogStart;
		GLint fogEnd;
		GLint fogColor;
	} locations;

	// ...

	_ShaderProgram() : program(0), initialized(false) {
		locations.position = -1;
		locations.normal = -1;
		locations.texCoord = -1;
		locations.PMatrix = -1;
		locations.VMatrix = -1;
		locations.MMatrix = -1;
		locations.time = -1;
		locations.diffuse = -1;
		locations.ambient = -1;
		locations.specular = -1;
		locations.shininess = -1;
		locations.texSampler = -1;
		locations.useTexture = -1;
		locations.ambientLightIntensity = -1;
		locations.ambientLightColor = -1;
		locations.directionalLightDirection = -1;
		locations.directionalLightIntensity = -1;
		locations.directionalLightColor = -1;
		locations.pointLightPosition = -1;
		locations.pointLightIntensity = -1;
		locations.pointLightColor = -1;
		locations.spotLightPosition = -1;
		locations.spotLightDirection = -1;
		locations.spotLightIntensity = -1;
		locations.spotLightColor = -1;
		locations.spotLightInnerCutoff = -1;
		locations.spotLightOuterCutoff = -1;
		locations.fogStart = -1;
		locations.fogEnd = -1;
	}

} ShaderProgram;

/**
 * \brief Represents a shader program.
 */
class Shader
{
public:
	/**
	 * \brief Constructs a Shader object.
	 * \param vertexFilename The filename of the vertex shader.
	 * \param fragmentFilename The filename of the fragment shader.
	 */
	Shader(const char* vertexFilename, const char* fragmentFilename);

	/**
	 * \brief Destroys the Shader object.
	 */
	~Shader();

	/**
	 * \brief Returns the ShaderProgram data.
	 * \return The ShaderProgram data.
	 */
	ShaderProgram getShaderData();

private:
	ShaderProgram shaderData; ///< The ShaderProgram data.
};

