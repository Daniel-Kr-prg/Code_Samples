//----------------------------------------------------------------------------------------
/**
 * \file    input.cpp : handle key actions (KeyPressed, KeyUp)
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "input.h"

std::unordered_map<char, bool> keyPressedState;
std::unordered_map<int, bool> keyPressedSpecialState;
int previousMouseX;
int previousMouseY;
int deltaX;
int deltaY;
Config* inputConfig;
bool initialized = false;

void Input::initialize(Config* config)
{
	inputConfig = config;

	if (inputConfig != nullptr)
	{
		initialized = true;
	}
	else
	{
		initialized = false;
	}
}

void Input::processKeyPressed(unsigned char keyPressed)
{
	if (initialized)
	{
		if (keyPressed == 27)
		{
			glutLeaveMainLoop();
			exit(EXIT_SUCCESS);
		}
		else if (keyPressed >= 'A' && keyPressed <= 'Z') {
			char lowerCaseKey = keyPressed + 32; // Преобразование в маленькую букву
			keyPressedState[lowerCaseKey] = true;
		}
		else
		{
			keyPressedState[keyPressed] = true;
		}

		// ON KEY DOWN HANDLE
		switch (keyPressed)
		{
		case 'r':
			//RESTART
			break;
		case 'c':
			//std::cout << "CAMERA POS: " << camera->getPosition().x << " " << camera->getPosition().y << " " << camera->getPosition().z << "\n";
			//std::cout << "YAW PITCH: " << camera->getYaw() << " " << camera->getPitch() << "\n";
			break;
		}

		Render::getRender()->getRootNode()->onKeyPress(keyPressed);
	}
}

void Input::processKeyReleased(unsigned char keyReleased) {
	if (initialized)
	{
		if (keyReleased >= 'A' && keyReleased <= 'Z') {
			char lowerCaseKey = keyReleased + 32; // Преобразование в маленькую букву
			keyPressedState[lowerCaseKey] = false;
		}
		else
		{
			keyPressedState[keyReleased] = false;
		}
	}
}


void Input::processSpecialKeyPressed(int keyPressed)
{
	if (initialized)
	{
		keyPressedSpecialState[keyPressed] = true;

		// ON KEY DOWN HANDLE

		Render::getRender()->getRootNode()->onSpecialKeyPress(keyPressed);
	}
}

void Input::processSpecialKeyReleased(int keyReleased)
{
	if (initialized)
	{
		keyPressedSpecialState[keyReleased] = false;
	}
}

void Input::processPassiveMouseMotion(int mouseX, int mouseY)
{
	if (initialized)
	{
		int halfWidth = inputConfig->getWindowWidth() / 2;
		int halfHeight = inputConfig->getWindowHeight() / 2;

		int deltaX = previousMouseX - mouseX;
		int deltaY = previousMouseY - mouseY;

		Render::getRender()->getRootNode()->onMouseMove(deltaX, deltaY, mouseX, mouseY);

		glutWarpPointer(halfWidth, halfHeight);

		previousMouseX = halfWidth;
		previousMouseY = halfHeight;
	}
}

bool Input::getKeyPressed(unsigned char keyPressed)
{
	return keyPressedState[keyPressed];
}

bool Input::getSpecialKeyPressed(unsigned char keyPressed)
{
	return keyPressedSpecialState[keyPressed];
}