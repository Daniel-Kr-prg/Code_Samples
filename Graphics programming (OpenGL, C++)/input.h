#pragma once
/**

\file input.h : Input class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include <unordered_map>
#include "pgr.h"
#include "config.h"

/**
 * \class Input
 * \brief Handles input from the user.
 */
class Input
{
public:
    /**
     * \brief Initializes the Input system.
     * \param config Pointer to the configuration object.
     */
    static void initialize(Config* config);

    /**
     * \brief Processes a key press event.
     * \param keyPressed The ASCII code of the pressed key.
     */
    static void processKeyPressed(unsigned char keyPressed);

    /**
     * \brief Processes a key release event.
     * \param keyReleased The ASCII code of the released key.
     */
    static void processKeyReleased(unsigned char keyReleased);

    /**
     * \brief Processes a special key press event.
     * \param keyPressed The integer value of the pressed special key.
     */
    static void processSpecialKeyPressed(int keyPressed);

    /**
     * \brief Processes a special key release event.
     * \param keyReleased The integer value of the released special key.
     */
    static void processSpecialKeyReleased(int keyReleased);

    /**
     * \brief Processes passive mouse motion event.
     * \param mouseX The X position of the mouse.
     * \param mouseY The Y position of the mouse.
     */
    static void processPassiveMouseMotion(int mouseX, int mouseY);

    /**
     * \brief Retrieves the state of a regular key.
     * \param keyPressed The ASCII code of the key.
     * \return True if the key is pressed, false otherwise.
     */
    static bool getKeyPressed(unsigned char keyPressed);

    /**
     * \brief Retrieves the state of a special key.
     * \param keyPressed The integer value of the special key.
     * \return True if the key is pressed, false otherwise.
     */
    static bool getSpecialKeyPressed(unsigned char keyPressed);
};