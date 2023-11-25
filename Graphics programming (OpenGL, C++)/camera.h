#pragma once

//----------------------------------------------------------------------------------------
/**
 * \file    camera.h : Camera class declaration
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include <pgr.h>
#include "object.h"
#include <unordered_map>
#include "input.h"

 /**
 \enum camState
 \brief Enumeration defining the camera states.
 */
enum camState { STATIC_CAMERA, CAMERA_ON_OBJECT, DYNAMIC_CAMERA };
/**

\class Camera

\brief Represents a camera object in the scene.

Inherits from ObjectInstance class.
*/
class Camera : public ObjectInstance
{
public:
	/**
 * \brief Default constructor for the Camera class.
 */
	Camera();

	/**
	 * \brief Constructor for the Camera class.
	 * \param fov Field of view of the camera.
	 * \param aspect Aspect ratio of the camera.
	 * \param zNear Near clipping plane of the camera.
	 * \param zFar Far clipping plane of the camera.
	 * \param speed Camera movement speed.
	 * \param keySensitivity Key sensitivity for camera movement.
	 * \param mouseSensitivity Mouse sensitivity for camera movement.
	 */
	Camera(float fov, float aspect, float zNear, float zFar, float speed, float keySensitivity, float mouseSensitivity);

	/**
	 * \brief Destructor for the Camera class.
	 */
	~Camera();

	/**
	 * \brief Retrieves the projection matrix of the camera.
	 * \return The projection matrix of the camera.
	 */
	glm::mat4 getProjection();

	/**
	 * \brief Retrieves the movement speed of the camera.
	 * \return The movement speed of the camera.
	 */
	float getSpeed();

	/**
	 * \brief Retrieves the key sensitivity of the camera.
	 * \return The key sensitivity of the camera.
	 */
	float getKeySensitivity();

	/**
	 * \brief Retrieves the mouse sensitivity of the camera.
	 * \return The mouse sensitivity of the camera.
	 */
	float getMouseSensitivity();

	/**
	 * \brief Retrieves the view matrix of the camera.
	 * \return The view matrix of the camera.
	 */
	glm::mat4 getView();

	/**
	 * \brief Adds yaw and pitch values to the camera's current yaw and pitch angles.
	 * \param yaw Yaw angle to add.
	 * \param pitch Pitch angle to add.
	 */
	void addYawPitch(float yaw, float pitch);

	/**
	 * \brief Sets the yaw and pitch angles of the camera.
	 * \param yaw New yaw angle.
	 * \param pitch New pitch angle.
	 */
	void setYawPitch(float yaw, float pitch);

	/**
	 * \brief Retrieves the yaw angle of the camera.
	 * \return The yaw angle of the camera.
	 */
	float getYaw();

	/**
	 * \brief Retrieves the pitch angle of the camera.
	 * \return The pitch angle of the camera.
	 */
	float getPitch();

	/**
	 * \brief Retrieves the current camera state.
	 * \return The current camera state.
	 */
	camState getCameraState();

	/**
	 * \brief Sets the camera state to a new state.
	 * \param newState The new camera state.
	 */
	void setCameraState(camState newState);

	/**
	 * \brief Sets the camera view to a specific position, yaw, and pitch.
	 * \param position New position of the camera.
	 * \param yaw New yaw angle of the camera.
	 * \param pitch New pitch angle of the camera.
	 */
	void setCameraView(glm::vec3 position, float yaw, float pitch);

	/**
	 * \brief Sets the camera to a static view 1.
	 */
	void setStaticView1();

	/**
	 * \brief Sets the camera to a static view 2.
	 */
	void setStaticView2();

	/**
	 * \brief Sets the camera to a dynamic camera state.
	 */
	void setDynamicCamera();

	/**
	 * \brief Initializes the Camera object.
	 * Overrides the initialize method from the ObjectInstance class.
	 */
	void initialize() override;

	/**
	 * \brief Updates the Camera object.
	 * Overrides the update method from the ObjectInstance class.
	 * \param elapsedTime Elapsed time since the last update.
	 * \param parentModelMatrix Model matrix of the parent node.
	 */
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;

	/**
 * \brief Deserializes the Camera object from JSON data.
 * Overrides the deserialize method from the ObjectInstance class.
 * \param data JSON data to deserialize from.
 */
	void deserialize(nlohmann::json data) override;

	/**
	 * \brief Handles mouse movement event.
	 * Overrides the onMouseMove method from the ObjectInstance class.
	 * \param deltaX Change in X position of the mouse.
	 * \param deltaY Change in Y position of the mouse.
	 * \param mouseX Current X position of the mouse.
	 * \param mouseY Current Y position of the mouse.
	 */
	void onMouseMove(int deltaX, int deltaY, int mouseX, int mouseY) override;

private:
	glm::mat4 projectionMatrix; /**< Projection matrix of the camera. */
	float speed; /**< Movement speed of the camera. */
	float keySensitivity; /**< Key sensitivity for camera movement. */
	float mouseSensitivity; /**< Mouse sensitivity for camera movement. */
	float yaw = 0; /**< Yaw angle of the camera. */
	float pitch = 0; /**< Pitch angle of the camera. */
	int previousMouseX = 0; /**< Previous X position of the mouse. */
	int previousMouseY = 0; /**< Previous Y position of the mouse. */
	glm::vec3 cameraBoxRadius = glm::vec3(10.0f, 5.0f, 10.0f); /**< Radius of the camera bounding box. */
	camState cameraState = STATIC_CAMERA; /**< Current camera state. */
};