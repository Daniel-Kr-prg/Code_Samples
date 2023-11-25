//----------------------------------------------------------------------------------------
/**
 * \file    player.cpp : Player class implementation. Player is a class for movable object (using WASD).
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "player.h"

Player::Player() : SingleMesh() 
{
	setIndex(1);
};

Player::~Player()
{
};

void Player::initialize()
{
	camera = Render::getRender()->getRootNode()->firstNodeByType<Camera>();
}

void Player::onSpecialKeyPress(int key)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		freeCamera();
		camera->setStaticView1();
		break;
	case GLUT_KEY_F2:
		freeCamera();
		camera->setStaticView2();
		break;
	case GLUT_KEY_F3:
		setCameraOnObject();
		break;
	case GLUT_KEY_F4:
		freeCamera();
		camera->setDynamicCamera();
		break;

	default:
		break;
	}
}

float Player::getSpeed()
{
	return currentSpeed;
}

float Player::getRotationSpeed() 
{
	return rotationSpeed;
}

float Player::getDrag()
{
	return drag;
}

void Player::update(const float elapsedTime, const glm::mat4* parentModelMatrix)
{
	bool movingAllowed = camera->getCameraState() == CAMERA_ON_OBJECT;
	float direction = 0;

	getFrameTime(elapsedTime);

	if (movingAllowed)
	{
		direction = Input::getKeyPressed('w') - Input::getKeyPressed('s');

		float rotation = Input::getKeyPressed('a') - Input::getKeyPressed('d');
		if (rotation != 0)
			rotateDegY(rotationSpeed * frameTime * rotation);

	}
	
	if (direction == 0)
	{
		currentSpeed = currentSpeed - currentSpeed * drag * frameTime;
	}
	else 
	{
		float currentAcceleration = acceleration;
		if (Input::getSpecialKeyPressed(GLUT_KEY_SHIFT_L))
			currentAcceleration *= 2;

		if (direction > 0)
			currentSpeed = glm::min(maxSpeed, currentSpeed + direction * currentAcceleration * frameTime);
		else
			currentSpeed = glm::max(-maxSpeed, currentSpeed + direction * currentAcceleration * frameTime);
	}

	//std::cout << currentSpeed << "      FRAME TIME: <<" << frameTime << "\n";

	float x;
	float y;
	float z;

	/*if (result.x >= 0)
		x = glm::min(result.x, cameraBoxRadius.x);
	else
		x = glm::max(result.x, -cameraBoxRadius.x);
	y = glm::max(0.1f, glm::min(result.y, cameraBoxRadius.x));

	if (result.z >= 0)
		z = glm::min(result.z, cameraBoxRadius.z);
	else
		z = glm::max(result.z, -cameraBoxRadius.z);

	setPosition(glm::vec3(x, y, z));*/

	addPosition(currentSpeed * frameTime * -forward);
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Player::setCameraOnObject()
{
	if (cameraOnObject)
		return;

	camera->setCameraState(CAMERA_ON_OBJECT);
	//camera->removeFromParent();
	addChild(camera);
	camera->setPosition(cameraPosition);
	cameraOnObject = true;
}

void Player::freeCamera()
{
	if (!cameraOnObject)
		return;
	removeChild(camera);
	camera->setPosition(this->globalPosition);
	cameraOnObject = false;
}

void Player::Interact()
{
	setCameraOnObject();
}