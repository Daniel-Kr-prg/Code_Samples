#pragma once
//----------------------------------------------------------------------------------------
/**

\file water.h : Water class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics
*/

#include "singlemesh.h"

/**
 * \brief Represents a Water object that can be rendered as a single mesh.
 */
class Water : public SingleMesh {
public:
	/**
	 * \brief Constructs a Water object.
	 */
	Water();

	/**
	 * \brief Destroys the Water object.
	 */
	~Water();

	/**
	 * \brief Updates the Water object.
	 * \param elapsedTime The elapsed time since the last update.
	 * \param parentModelMatrix The model matrix of the parent object.
	 */
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;

	/**
	 * \brief Draws the Water object.
	 */
	void draw() override;

	/**
	 * \brief Performs an interaction with the Water object.
	 */
	void Interact() override;
private:
	glm::mat3 texCoordMatrix = glm::mat3(1);  ///< The texture coordinate matrix for the water.
};

