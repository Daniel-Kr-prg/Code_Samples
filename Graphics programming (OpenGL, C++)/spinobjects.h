#pragma once
//----------------------------------------------------------------------------------------
/**

\file spinobject.h : SpinObject class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include "singlemesh.h"


/**
 * \brief Represents a buoy object.
 */
class Bouy : public SingleMesh {
private:
	float rotationPerSecond = 0.5f; ///< The rotation speed per second.
	bool lightsOn = true; ///< Indicates if the lights are on.

public:
	/**
	 * \brief Constructs a Bouy object.
	 */
	Bouy();

	/**
	 * \brief Destroys the Bouy object.
	 */
	~Bouy();

	/**
	 * \brief Updates the buoy object.
	 * \param elapsedTime The elapsed time since the last update.
	 * \param parentModelMatrix The parent model matrix.
	 */
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;

	/**
	 * \brief Handles the interaction with the buoy object.
	 */
	void Interact() override;
};


/**
 * \brief Represents an airplane object.
 */
class Airplane : public SingleMesh {
private:
	float rotationPerSecond = 0.5f; ///< The rotation speed per second.
	float radius = 2.0f; ///< The radius of the airplane.

	bool falling = false; ///< Indicates if the airplane is falling.

public:
	/**
	 * \brief Constructs an Airplane object.
	 */
	Airplane();

	/**
	 * \brief Destroys the Airplane object.
	 */
	~Airplane();

	/**
	 * \brief Updates the airplane object.
	 * \param elapsedTime The elapsed time since the last update.
	 * \param parentModelMatrix The parent model matrix.
	 */
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;

	/**
	 * \brief Handles the interaction with the airplane object.
	 */
	void Interact() override;
};