//----------------------------------------------------------------------------------------
/**
 * \file    island.cpp : This file contains the 'main' function and callbacks.
			Program execution begins and ends there.
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

 /**
  * \brief	\mainpage Documentation of the skeleton for the Computer graphics course on FEE and FIT CTU CZ
  *
  * This is the skeleton of an OpenGL application written in FreeGLUT and PGR framework.
  * It serves as a starter boiler-plate code for a semester project - a simple interactive application.
  * Write your code into the functions prepared. Add your data (models, textures, ...) and logic.
  *
  *
  * Comment your code using the [doxygen](https://www.doxygen.nl/index.html) documenting system style.
  * Create "doxygen" directory, make it current by "cd doxygen", prepare a configuration file with "doxygen -g" and edit the details.
  *
  * Start by renaming of this file from skeleton.cpp to <your_name>.cpp and the project to <your_name>.vcxproj
  *
  * In <your_name>.vcxproj:
  *   - rename skeleton in <RootNamespace>skeleton</RootNamespace> to <your_name>
  *   - replace skeleton.cpp in <ClCompile Include="skeleton.cpp" /> to <your_name>.cpp
  *
  * Start coding and documenting. Generate the documentation by the command "doxygen" in the "doxygen" directory.
  *
  */


  // TODO: tabulka klaves a jeji obsluha keyPressed/keyReleased a timer

#include <iostream>
#include <unordered_map>
#include "pgr.h"
#include "object.h"
#include "singlemesh.h"
#include "camera.h"
#include "config.h"
#include "input.h"
#include "player.h"
#include "spinobjects.h"


/// <summary>
/// Config instance;
/// </summary>
Config* config;

// -----------------------  OpenGL stuff ---------------------------------


/**
 * \brief Draw all scene objects.
 */
void drawScene(void)
{
	Render::getRender()->getRootNode()->draw();
	Render::getRender()->endDraw();
}


// -----------------------  Window callbacks ---------------------------------

/**
 * \brief Draw the window contents.
 */
void displayCb() {

	glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw the window contents (scene objects)
	drawScene();

	glutSwapBuffers();
}

/**
 * \brief Window was reshaped.
 * \param newWidth New window width
 * \param newHeight New window height
 */
void reshapeCb(int newWidth, int newHeight) {
	// glViewport(...);
};


// -----------------------  Keyboard ---------------------------------


float y_desired = 0.0f;
bool place_mode = false;

/**
 * \brief Handle the key pressed event.
 * Called whenever a key on the keyboard was pressed. The key is given by the "keyPressed"
 * parameter, which is an ASCII character. It's often a good idea to have the escape key (ASCII value 27)
 * to call glutLeaveMainLoop() to exit the program.
 * \param keyPressed ASCII code of the key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardCb(unsigned char keyPressed, int mouseX, int mouseY) {
	Input::processKeyPressed(keyPressed);

	if (!place_mode)
		return;

	if (keyPressed == '[')
		y_desired += 0.01f;
	if (keyPressed == ']')
		y_desired -= 0.01f;
}

// Called whenever a key on the keyboard was released. The key is given by
// the "keyReleased" parameter, which is in ASCII. 
/**
 * \brief Handle the key released event.
 * \param keyReleased ASCII code of the released key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardUpCb(unsigned char keyReleased, int mouseX, int mouseY) {
	Input::processKeyReleased(keyReleased);
}

//
/**
 * \brief Handle the non-ASCII key pressed event (such as arrows or F1).
 *  The special keyboard callback is triggered when keyboard function (Fx) or directional
 *  keys are pressed.
 * \param specKeyPressed int value of a predefined glut constant such as GLUT_KEY_UP
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void specialKeyboardCb(int specKeyPressed, int mouseX, int mouseY) {
	Input::processSpecialKeyPressed(specKeyPressed);
}

void specialKeyboardUpCb(int specKeyReleased, int mouseX, int mouseY) {
	Input::processSpecialKeyReleased(specKeyReleased);
} // key released

// -----------------------  Mouse ---------------------------------
// three events - mouse click, mouse drag, and mouse move with no button pressed

// 
/**
 * \brief React to mouse button press and release (mouse click).
 * When the user presses and releases mouse buttons in the window, each press
 * and each release generates a mouse callback (including release after dragging).
 *
 * \param buttonPressed button code (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON)
 * \param buttonState GLUT_DOWN when pressed, GLUT_UP when released
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseCb(int buttonPressed, int buttonState, int mouseX, int mouseY) {
	
	if (buttonState == GLUT_DOWN)
	{
		unsigned int objectID = 0;
		glReadPixels(mouseX, mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &objectID);

		if (objectID > 0) {
			std::cout << "Clicked on object with ID: " << objectID << "\n";
			ObjectInstance* object = Render::getRender()->getRootNode()->firstNodeByIndex(objectID);
			if (object != nullptr)
			{
				if (objectID == 4)
				{
					Camera* cam = Render::getRender()->getRootNode()->firstNodeByType<Camera>();
					glm::vec3 forward = cam->getForward();
					glm::vec3 pos = cam->getPosition();

					float t = -pos.y / forward.y;

					glm::vec3 intersect = glm::vec3(pos.x + forward.x * t, 0.15, pos.z + forward.z * t);

					Render::getRender()->splashAtPosition(intersect);
				}
				else
				{
					object->Interact();
				}
			}
		}
	}
}

/**
 * \brief Handle mouse dragging (mouse move with any button pressed).
 *        This event follows the glutMouseFunc(mouseCb) event.
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseMotionCb(int mouseX, int mouseY) {

}

/**
 * \brief Handle mouse movement over the window (with no button pressed).
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void passiveMouseMotionCb(int mouseX, int mouseY) {
	Input::processPassiveMouseMotion(mouseX, mouseY);

	if (!place_mode)
		return;
	Camera* cam = Render::getRender()->getRootNode()->firstNodeByType<Camera>();
	glm::vec3 forward = cam->getGlobalForward();
	glm::vec3 pos = cam->getPosition();

	float t = (- pos.y) / forward.y;

	glm::vec3 intersect = glm::vec3(pos.x + forward.x * t, y_desired, pos.z + forward.z * t);

	ObjectInstance* boat = Render::getRender()->getRootNode()->firstNodeByIndex(1);
	boat->setPosition(intersect);

	std::cout << "x:" << intersect.x << ",y:" << intersect.y << ",z:" << intersect.z << "\n";
}


// -----------------------  Timer ---------------------------------

/**
 * \brief Callback responsible for the scene update.
 */
void timerCb(int)
{
#ifndef SKELETON // @task_1_0
	const glm::mat4 sceneRootMatrix = glm::mat4(1.0f);

	float elapsedTime = 0.001f * static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // milliseconds => seconds
	
	// update the application state
	Render::getRender()->getRootNode()->update(elapsedTime, &sceneRootMatrix);
	Render::getRender()->update(elapsedTime);
#endif // task_1_0

	
	// and plan a new event
	glutTimerFunc(33, timerCb, 0);

	// create display event
	glutPostRedisplay();
}

// -----------------------  Application ---------------------------------

/**
 * \brief Initialize application data and OpenGL stuff.
 */
void initApplication() {
	// init OpenGL
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// - all programs (shaders), buffers, textures, ...
	config->loadScene(*Render::getRender()->getRootNode());
	Render::getRender()->getRootNode()->initialize();
	Input::initialize(config);
	// objects.push_back(new SingleMesh(&commonShaderProgram));

	// init your Application
	// - setup the initial application state
	glutSetCursor(GLUT_CURSOR_NONE);
}

/**
 * \brief Delete all OpenGL objects and application data.
 */
void finalizeApplication(void) {

	// cleanUpObjects();

	// delete buffers
	// cleanupModels();

	// delete shaders
}


/**
 * \brief Entry point of the application.
 * \param argc number of command line arguments
 * \param argv array with argument strings
 * \return 0 if OK, <> elsewhere
 */
int main(int argc, char** argv) {

	config = new Config("data/config.json");

	// initialize the GLUT library (windowing system)
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_STENCIL | GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// for each window
	{
		//   initial window size + title
		glutInitWindowSize(config->getWindowWidth(), config->getWindowHeight());
		glutCreateWindow(config->getWindowTitle().c_str());

		// callbacks - use only those you need
		glutDisplayFunc(displayCb);
		glutReshapeFunc(reshapeCb);
		glutKeyboardFunc(keyboardCb);
		glutKeyboardUpFunc(keyboardUpCb);
		glutSpecialFunc(specialKeyboardCb);     // key pressed
		glutSpecialUpFunc(specialKeyboardUpCb); // key released
		glutPassiveMotionFunc(passiveMouseMotionCb);
		glutMouseFunc(mouseCb);
		// glutMotionFunc(mouseMotionCb);
#ifndef SKELETON // @task_1_0
		glutTimerFunc(33, timerCb, 0);
#else
		// glutTimerFunc(33, timerCb, 0);
#endif // task_1_0

	}
	// end for each window 

	// initialize pgr-framework (GL, DevIl, etc.)
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	// init your stuff - shaders & program, buffers, locations, state of the application
	initApplication();

	// handle window close by the user
	glutCloseFunc(finalizeApplication);

	// Infinite loop handling the events
	glutMainLoop();

	// code after glutLeaveMainLoop()
	// cleanup

	return EXIT_SUCCESS;
}
