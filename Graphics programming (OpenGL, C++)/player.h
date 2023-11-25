#pragma once
//----------------------------------------------------------------------------------------
/**
\file player.h : Player class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include "singlemesh.h"

class Player : public SingleMesh
{
private:
	float maxSpeed = 5.0f;
	float currentSpeed = 0.0f;
	float acceleration = 1.0f;
	float rotationSpeed = 60.0f;
	float drag = 0.75f;

	glm::vec3 boatBoxRadius = glm::vec3(9.0f, 5.0f, 9.0f);
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.5f, -1.0f);
	std::vector<ObjectInstance*>::iterator cameraIterator;
	bool cameraOnObject = false;
	Camera* camera;


public:
	Player();
	~Player();

	float getSpeed();

	float getRotationSpeed();

	float getDrag();

	void setCameraOnObject();

	void freeCamera();

	void initialize() override;
	void update(const float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void onSpecialKeyPress(int key) override;

	void Interact() override;
};

