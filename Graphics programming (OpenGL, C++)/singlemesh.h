#pragma once
//----------------------------------------------------------------------------------------
/**
\file singlemesh.h : SingleMesh class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include "object.h"
#include <iostream>
#include "render.h"
#include "camera.h"
#include "config.h"

/**
 * \brief Represents a single mesh object.
 */
class SingleMesh : public ObjectInstance
{
public:
	/**
	 * \brief Constructs a SingleMesh object.
	 */
	SingleMesh();

	/**
	 * \brief Destroys the SingleMesh object.
	 */
	~SingleMesh();

	/**
	 * \brief Updates the SingleMesh object.
	 * \param elapsedTime The elapsed time since the last update.
	 * \param parentModelMatrix The parent model matrix.
	 */
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;

	/**
	 * \brief Draws the SingleMesh object.
	 */
	void draw() override;

	/**
	 * \brief Deserializes the SingleMesh object from JSON data.
	 * \param data The JSON data to deserialize.
	 */
	void deserialize(nlohmann::json data) override;

private:
	/**
	 * \brief Loads a single mesh from a file.
	 * \param fileName The filename of the mesh file.
	 * \param shader The shader to use for rendering the mesh.
	 * \param geometry A pointer to store the loaded geometry.
	 * \return True if the mesh was loaded successfully, false otherwise.
	 */
	bool loadSingleMesh(std::string fileName, Shader* shader, ObjectGeometry** geometry);

protected:
	bool initialized; ///< Flag indicating whether the object has the shader with defined locations.
};
