//----------------------------------------------------------------------------------------
/**
 * \file    config.cpp : Config initialization from json and scene loading. 
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "config.h"
#include "singlemesh.h"
#include "player.h"
#include "spinobjects.h"
#include "water.h"
#include "splashshader.h"
#include "splash.h"
#include "boat.h"
#include "pgr.h"

Config::Config(const char* filename)
{
	configFilename = filename;

	std::ifstream file(filename);

	if (file.is_open())
	{
		nlohmann::json data = nlohmann::json::parse(file);
		
		if (data.contains("window"))
		{
			nlohmann::json windowData = data["window"];
			if (windowData.contains("windowWidth"))
				windowWidth = windowData["windowWidth"].get<int>();
			if (windowData.contains("windowHeight"))
				windowHeight = windowData["windowHeight"].get<int>();
			if (windowData.contains("windowTitle"))
				windowTitle = windowData["windowTitle"].get<std::string>();
		}

		Render::getRender()->initialize(this);

		if (data.contains("ambient"))
		{
			nlohmann::json ambientData = data["ambient"];

			if (ambientData.contains("intensity"))
				ambientLightIntensity = ambientData["intensity"].get<float>();
			if (ambientData.contains("lightColor"))
				ambientLightColor = readVectorFromJSON(ambientData["lightColor"], glm::vec3(1));
			if (ambientData.contains("fogStart"))
				fogStart = ambientData["fogStart"].get<float>();
			if (ambientData.contains("fogEnd"))
				fogEnd = ambientData["fogEnd"].get<float>();
			if (ambientData.contains("fogHeightStart"))
				fogHeightStart = ambientData["fogHeightStart"].get<float>();
			if (ambientData.contains("fogHeightEnd"))
				fogHeightEnd = ambientData["fogHeightEnd"].get<float>();
			if (ambientData.contains("fogColor"))
				fogColor = readVectorFromJSON(ambientData["fogColor"], glm::vec3(1));
		}

		if (data.contains("shaders"))
		{
			for (nlohmann::json shaderData : data["shaders"])
			{
				if (shaderData.contains("fragmentPath") && shaderData.contains("vertexPath"))
					shaders.push_back({ shaderData["fragmentPath"].get<std::string>(), shaderData["vertexPath"].get<std::string>() });
			}
		}
	}
}

/**

\brief Returns the width of the window
\return The width of the window
*/
int Config::getWindowWidth()
{
	return windowWidth;
}
/**

\brief Returns the height of the window
\return The height of the window
*/
int Config::getWindowHeight()
{
	return windowHeight;
}
/**

\brief Returns the title of the window
\return The title of the window
*/
std::string Config::getWindowTitle()
{
	return windowTitle;
}
/**

\brief Returns the field of view (FOV) of the camera
\return The field of view (FOV) of the camera
*/
float Config::getFov()
{
	return fov;
}
/**

\brief Returns the near clipping plane value (ZNear) of the camera
\return The near clipping plane value (ZNear) of the camera
*/
float Config::getZNear()
{
	return zNear;
}
/**

\brief Returns the far clipping plane value (ZFar) of the camera
\return The far clipping plane value (ZFar) of the camera
*/
float Config::getZFar()
{
	return zFar;
}
/**

\brief Returns the speed value for movement
\return The speed value for movement
*/
float Config::getSpeed()
{
	return speed;
}
/**

\brief Returns the sensitivity value for keyboard input
\return The sensitivity value for keyboard input
*/
float Config::getKeySensitivity()
{
	return keySensitivity;
}
/**

\brief Returns the sensitivity value for mouse input
\return The sensitivity value for mouse input
*/
float Config::getMouseSensitivity()
{
	return mouseSensitivity;
}

float Config::getAmbientLightIntensity() {
	return ambientLightIntensity;
}

glm::vec3 Config::getAmbientLightColor() {
	return ambientLightColor;
}

float Config::getFogStart()
{
	return fogStart;
}

float Config::getFogEnd()
{
	return fogEnd;
}

float Config::getFogHeightStart()
{
	return fogHeightStart;
}

float Config::getFogHeightEnd()
{
	return fogHeightEnd;
}

glm::vec3 Config::getFogColor() 
{
	return fogColor;
}

/**
 * \brief Loads objects from json to scene
 * \param scene List of objects in scene
 * \param shaderList List of shaders
 */
void Config::loadScene(ObjectInstance& rootNode)
{
	std::ifstream file(configFilename);

	// Load shaders
	for (ShaderObject shaderData : shaders)
	{
		Render::getRender()->addShader(new Shader(shaderData.vertexPath.c_str(), shaderData.fragmentPath.c_str()));
	}

	if (file.is_open())
	{
		nlohmann::json data = nlohmann::json::parse(file);

		if (data.contains("skyBox"))
		{
			nlohmann::json skyData = data["skyBox"];

			if (skyData.contains("fragmentPath") && skyData.contains("vertexPath") && skyData.contains("texturePath"))
			{
				Render::getRender()->initializeSkyboxGeometry(skyData["vertexPath"].get<std::string>(), skyData["fragmentPath"].get<std::string>(), skyData["texturePath"].get<std::string>());
			}
		}

		if (data.contains("splash"))
		{
			nlohmann::json splashData = data["splash"];

			if (splashData.contains("frameDuration") && splashData.contains("fragmentPath") && splashData.contains("vertexPath") && splashData.contains("texturePath"))
			{
				SplashShader* shader = new SplashShader(splashData["vertexPath"].get<std::string>().c_str(), splashData["fragmentPath"].get<std::string>().c_str());
				Render::getRender()->setSplash(new Splash(shader, splashData["texturePath"].get<std::string>(), splashData["frameDuration"].get<float>()));
			}
		}


		if (data.contains("scene"))
		{
			for (nlohmann::json sceneObjectData : data["scene"])
			{
				getNextObject(sceneObjectData, rootNode);
			}
		}
	}
}

/**
 * \brief Processes next json scene object
 * \param data Json data
 * \param scene Scene objects list
 * \param shaderList List of shaders
 */
void Config::getNextObject(nlohmann::json data, ObjectInstance& node)
{
	if (data.contains("objectType"))
	{
		ObjectInstance* object = createObjectByType(data["objectType"].get<std::string>());

		object->deserialize(data);
		node.addChild(object);

		if (data.contains("children"))
		{
			for (nlohmann::json childrenData : data["children"])
			{
				getNextObject(childrenData, *object);
			}
		}
	}
}

ObjectInstance* Config::createObjectByType(std::string typeName)
{
	if (typeName == "SingleMesh")
		return new SingleMesh();
	else if (typeName == "Camera")
		return new Camera();
	else if (typeName == "SpotLight")
		return new SpotLight();
	else if (typeName == "DirectionalLight")
		return new DirectionalLight();
	else if (typeName == "PointLight")
		return new PointLight();
	else if (typeName == "Player")
		return (ObjectInstance*) new Player();
	else if (typeName == "Water")
		return (ObjectInstance*) new Water();
	else if (typeName == "Bouy")
		return (ObjectInstance*) new Bouy();
	else if (typeName == "Airplane")
		return (ObjectInstance*) new Airplane();
	else if (typeName == "Boat")
		return (ObjectInstance*) new Boat();
	else
		throw "Config::createObjectByType(): Type not found";
}

Config::~Config()
{

}