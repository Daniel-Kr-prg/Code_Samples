#pragma once
// ----------------------------------------------------------------------------------------
/**
\file splashshader.h : SplashShader class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include <fstream>
#include <sstream>
#include <string>
#include <pgr.h>

/**
 * \brief Structure representing a SplashShaderProgram.
 */
typedef struct _SplashShaderProgram {
	GLuint program;              ///< Identifier for the shader program
	struct {
		GLint position;          ///< Vertex attribute location for position
		GLint texCoord;          ///< Vertex attribute location for texture coordinates
		GLint Pmatrix;           ///< Uniform location for the projection matrix
		GLint Vmatrix;           ///< Uniform location for the view matrix
		GLint Mmatrix;           ///< Uniform location for the model matrix
		GLint time;              ///< Uniform location for time
		GLint texSampler;        ///< Uniform location for texture sampler
		GLint frameDuration;     ///< Uniform location for frame duration
	} locations;
} SplashShaderProgram;

/**
 * \brief Represents a SplashShader used for rendering splash effects.
 */
class SplashShader {
public:
	/**
	 * \brief Constructs a SplashShader object.
	 * \param vertexFilename The filename of the vertex shader source code.
	 * \param fragmentFilename The filename of the fragment shader source code.
	 */
	SplashShader(const char* vertexFilename, const char* fragmentFilename);

	/**
	 * \brief Destroys the SplashShader object.
	 */
	~SplashShader();

	/**
	 * \brief Retrieves the shader data of the SplashShader.
	 * \return The SplashShaderProgram representing the shader data.
	 */
	SplashShaderProgram getShaderData();
private:
	SplashShaderProgram shaderData; ///< The shader data of the SplashShader.
};