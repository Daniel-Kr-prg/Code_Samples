//----------------------------------------------------------------------------------------
/**
 * \file    camera.cpp : Camera functions implementation. All camera actions are handled there
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "camera.h"
#include "object.h"
#include "config.h"
#include "player.h"
/**
 * \brief Creates the camera object.
 * \param fov Field of view
 * \param aspect Screen aspect ratio
 * \param zNear Near clipping plane distance
 * \param zFar Far clipping plane distance
 */
Camera::Camera(float fov, float aspect, float zNear, float zFar, float speed, float keySensitivity, float mouseSensitivity) {
	projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
	this->speed = speed;
	this->keySensitivity = keySensitivity;
	this->mouseSensitivity = mouseSensitivity;
}

Camera::Camera() {

}

/**
 * \brief Set camera parameters.
 * \param fov Field of view
 * \param aspect Screen aspect ratio
 * \param zNear Near clipping plane distance
 * \param zFar Far clipping plane distance
 */
void Camera::setCameraView(glm::vec3 position, float yaw, float pitch)
{
	setYawPitch(yaw, pitch);
	setPosition(position);
}

/**
 * \brief Allows camera to move
 */
void Camera::setDynamicCamera()
{
	setCameraState(DYNAMIC_CAMERA);
}


void Camera::setStaticView1()
{
	setCameraState(STATIC_CAMERA);
	setCameraView(glm::vec3(3.98f, 0.637f, 4.60f), -5.54f, -0.14f);
}

void Camera::setStaticView2()
{
	setCameraState(STATIC_CAMERA);
	setCameraView(glm::vec3(5.516f, 4.74f, -2.87f), -4.31f, -0.71f);
}

void Camera::setCameraState(camState newState)
{
	cameraState = newState;
}

camState Camera::getCameraState()
{
	return cameraState;
}

/**
 * \brief Returns the current projection matrix
 */
glm::mat4 Camera::getProjection() {
	return projectionMatrix;
}

/**
 * \brief Returns the camera speed
 */
float Camera::getSpeed() {
	return speed;
}

/**
 * \brief Returns the camera rotation sensitivity (rotation by keys)
 */
float Camera::getKeySensitivity()
{
	return keySensitivity;
}

/**
 * \brief Returns the camera rotation sensitivity (rotation by mouse)
 */
float Camera::getMouseSensitivity()
{
	return mouseSensitivity;
}

/**
 * \brief Returns the current view matrix
 */
glm::mat4 Camera::getView() {
	return glm::inverse(globalModelMatrix);
}

/**
 * \brief Add camera rotation from input
 */
void Camera::addYawPitch(float yaw, float pitch) {
	this->yaw += yaw;
	this->pitch += pitch;

	if (this->pitch > 1.55f)
	{
		this->pitch = 1.55f;
	}
	else
	if (this->pitch < -1.55f)
	{
		this->pitch = -1.55f;
	}

	setRotation(glm::quat());
	rotateRadY(this->yaw);
	rotateRadX(this->pitch);
}

void Camera::setYawPitch(float yaw, float pitch) {
	this->yaw = yaw;
	this->pitch = pitch;

	if (this->pitch > 1.55f)
	{
		this->pitch = 1.55f;
	}
	else
		if (this->pitch < -1.55f)
		{
			this->pitch = -1.55f;
		}

	setRotation(glm::quat());
	rotateRadY(this->yaw);
	rotateRadX(this->pitch);
}
float Camera::getYaw()
{
	return this->yaw;
}
float Camera::getPitch()
{
	return this->pitch;
}

void Camera::initialize() 
{
	setStaticView1();
	ObjectInstance::initialize();
}

void Camera::update(float elapsedTime, const glm::mat4* parentModelMatrix)
{
	getFrameTime(elapsedTime);
	Render::getRender()->setCamera(this);

	if (getCameraState() == STATIC_CAMERA)
	{
		ObjectInstance::update(elapsedTime, parentModelMatrix);
		return;
	}

	if (getCameraState() == DYNAMIC_CAMERA)
	{
		glm::vec3 movementVector(0.0f);

		if (Input::getKeyPressed('w'))
		{
			movementVector += getForward();
		}
		else
		if (Input::getKeyPressed('s'))
		{
			movementVector -= getForward();
		}

		if (Input::getKeyPressed('a'))
		{
			movementVector -= getRight();
		}
		else
		if (Input::getKeyPressed('d'))
		{
			movementVector += getRight();
		}

		movementVector *= frameTime * getSpeed();
		glm::vec3 currentPosition = getPosition();
		glm::vec3 result = currentPosition + movementVector;

		float x;
		float y;
		float z;

		if (result.x >= 0)
			x = glm::min(result.x, cameraBoxRadius.x);
		else
			x = glm::max(result.x, -cameraBoxRadius.x);
		y = glm::max(0.2f, glm::min(result.y, cameraBoxRadius.x));
		
		if (result.z >= 0)
			z = glm::min(result.z, cameraBoxRadius.z);
		else
			z = glm::max(result.z, -cameraBoxRadius.z);

		setPosition(glm::vec3(x, y, z));

	}


	float rotationX = 0;
	float rotationY = 0;

	if (Input::getSpecialKeyPressed(GLUT_KEY_UP))
	{
		rotationX += 1 * frameTime * getKeySensitivity();
	}
	else
	if (Input::getSpecialKeyPressed(GLUT_KEY_DOWN))
	{
		rotationX -= 1 * frameTime * getKeySensitivity();
	}

	if (Input::getSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		rotationY += 1 * frameTime * getKeySensitivity();
	}
	else
	if (Input::getSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		rotationY -= 1 * frameTime * getKeySensitivity();
	}

	addYawPitch(rotationY, rotationX);
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Camera::onMouseMove(int deltaX, int deltaY, int mouseX, int mouseY)
{
	if (cameraState == STATIC_CAMERA)
	{
		ObjectInstance::onMouseMove(deltaX, deltaY, mouseX, mouseY);
		return;
	}

	addYawPitch(deltaX * getMouseSensitivity(), deltaY * getMouseSensitivity());
	ObjectInstance::onMouseMove(deltaX, deltaY, mouseX, mouseY);
}

void Camera::deserialize(nlohmann::json data)
{
	ObjectInstance::deserialize(data);

	if (data.contains("fov") && data.contains("zNear") && data.contains("zFar"))
		projectionMatrix = glm::perspective(
			data["fov"].get<float>(),
			Render::getRender()->getCurrentAspect(),
			data["zNear"].get<float>(),
			data["zFar"].get<float>());
	if (data.contains("speed"))
		speed = data["speed"].get<float>();
	if (data.contains("keySensitivity"))
		keySensitivity = data["keySensitivity"].get<float>();
	if (data.contains("mouseSensitivity"))
		mouseSensitivity = data["mouseSensitivity"].get<float>();
}

/**
 * \brief Destroys the camera object
 */
Camera::~Camera() {

}