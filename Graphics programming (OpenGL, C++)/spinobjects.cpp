//----------------------------------------------------------------------------------------
/**
 * \file    spinobjects.cpp : Objects which are supposed to rotate or move around
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "spinobjects.h"

Bouy::Bouy() : SingleMesh()
{
	setIndex(2);
};

Bouy::~Bouy()
{
};

void Bouy::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	getFrameTime(elapsedTime);
	setRotationRad(glm::vec3(0.2f * glm::cos(elapsedTime * rotationPerSecond / 2), elapsedTime * rotationPerSecond, 0.2f * glm::sin(elapsedTime * rotationPerSecond)));
	//rotateRadY(-frameTime * rotationPerSecond);

	ObjectInstance::update(elapsedTime, parentModelMatrix);
}


void Bouy::Interact()
{
	if (lightsOn)
	{

	}
	else 
	{

	}
}


Airplane::Airplane() : SingleMesh()
{
	setIndex(3);
};

Airplane::~Airplane()
{
};

void Airplane::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	getFrameTime(elapsedTime);
	if (falling)
	{
		glm::vec3 pos = getPosition();
		if (pos.y <= -0.2f)
		{
			// BOOM BABAX
		}
		else
		{
			addPosition(-getForward() * frameTime * 1.5f);
		}
	}
	else
	{
		float rotation = rotationPerSecond * elapsedTime;

		glm::vec3 pos = getPosition();
		setPosition(glm::vec3(radius * cos(rotation), pos.y, radius * sin(rotation)));
	
		
		rotateRadY(-frameTime * rotationPerSecond);
	}
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Airplane::Interact()
{
	if (falling)
		return;
	falling = true;
	rotateRadX(0.35f);
}