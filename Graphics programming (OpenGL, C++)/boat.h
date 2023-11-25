#pragma once

//----------------------------------------------------------------------------------------
/**
 * \file    boat.h : Boat class declaration
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "object.h"

class Boat : public ObjectInstance
{
public:
	/**
	* \brief Constructor for the Boat class.
	*/
	Boat();
	/**
	* \brief Destructor for the Boat class.
	*/
	~Boat();
	/**
	 * \brief Draws the Boat object.
	 * Overrides the draw method from the ObjectInstance class.
	 */
	void draw() override;

	/**
	 * \brief Deserializes the Boat object from JSON data.
	 * Overrides the deserialize method from the ObjectInstance class.
	 * \param data JSON data to deserialize from.
	 */
	void deserialize(nlohmann::json data) override;
private:
	bool initialized; /* Flag indicating whether the object has the shader with defined locations. */
};

