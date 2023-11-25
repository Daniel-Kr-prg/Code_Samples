#pragma once
/**

\file jsonutils.h : Json special conversion methods
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include "pgr.h"
#include "json.hpp"
/**

\brief Reads a vector from JSON data.
\param data JSON data to read from.
\param defaultValue Default value to return if the vector is not found in the JSON data.
\return The vector read from the JSON data, or the default value if not found.
*/
static glm::vec3 readVectorFromJSON(nlohmann::json data, glm::vec3 defaultValue)
{
	if (data.contains("x") && data.contains("y") && data.contains("z"))
		return glm::vec3(data["x"].get<float>(), data["y"].get<float>(), data["z"].get<float>());
	else
		if (data.contains("r") && data.contains("g") && data.contains("b"))
			return glm::vec3(data["r"].get<float>(), data["g"].get<float>(), data["b"].get<float>());
		else
			return defaultValue;
}