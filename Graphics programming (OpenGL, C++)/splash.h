#pragma once
//----------------------------------------------------------------------------------------
/**
\file splash.h : Splash class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include "pgr.h"
#include <fstream>
#include <sstream>
#include <string>
#include "object.h"
#include "splashshader.h"

/**
 * \brief The number of vertices in a splash quad.
 */
const int splashNumQuadVertices = 4;

/**
 * \brief The vertex data for the splash quad.
 */
const float splashVertexData[splashNumQuadVertices * 5] = {
	// x      y     z     u     v
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};

/**
 * \brief Represents a splash effect.
 */
class Splash {
public:
	/**
	 * \brief Constructs a Splash object.
	 * \param shader The SplashShader object used for rendering.
	 * \param textureName The name of the texture for the splash effect.
	 * \param frameDuration The duration of each frame in seconds.
	 */
	Splash(SplashShader* shader, std::string textureName, float frameDuration);

	/**
	 * \brief Destroys the Splash object.
	 */
	~Splash();

	/**
	 * \brief Creates a splash effect at the specified position.
	 * \param position The position of the splash effect.
	 */
	void splashAtPosition(glm::vec3 position);

	/**
	 * \brief Updates the splash effect.
	 * \param elapsedTime The elapsed time since the last update.
	 */
	void update(float elapsedTime);

	/**
	 * \brief Draws the splash effect.
	 */
	void draw();

private:
	bool isDrawing = false; ///< Indicates if the splash effect is currently being drawn.
	float drawStartTime = 0; ///< The start time of the drawing.
	float frameDuration; ///< The duration of each frame in seconds.
	float elapsedTime; ///< The elapsed time since the last update.
	glm::vec3 drawPosition = glm::vec3(0, 1, 0); ///< The position of the splash effect.
	SplashShader* shader; ///< The SplashShader object used for rendering.
	ObjectGeometry* geometry; ///< The geometry of the splash quad.
};