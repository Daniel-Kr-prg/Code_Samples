#pragma once
//----------------------------------------------------------------------------------------
/**

\file object.h : Object class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/

#include "pgr.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <unordered_map>
#include "json.hpp"
#include "jsonutils.h"

class Config;
class Camera;
class Shader;
class ObjectInstance;

/**
 * \brief Geometry of an object (vertices, triangles).
 */
typedef struct _ObjectGeometry {
	GLuint        vertexBufferObject;   ///< identifier for the vertex buffer object
	GLuint        elementBufferObject;  ///< identifier for the element buffer object
	GLuint        vertexArrayObject;    ///< identifier for the vertex array object
	unsigned int  numTriangles;         ///< number of triangles in the mesh

	// material
	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;
	float         shininess;
	GLuint        texture;
} ObjectGeometry;

/**
 * \brief Linear representation of the scene objects.  The objects themselves may represent the subtrees.
 */
typedef std::list<ObjectInstance*> ObjectList;
/**
 * \class ObjectInstance
 * \brief Represents an instance of an object in the scene.
 */
typedef std::vector<Shader*> ShaderList;

class ObjectInstance {

protected:
	ObjectGeometry* geometry; /**< Pointer to the geometry of the object. */
	glm::mat4 localModelMatrix; /**< Local model matrix of the object. */
	glm::mat4 globalModelMatrix; /**< Global model matrix of the object. */

	glm::vec3 globalPosition; /**< Global position of the object. */
	glm::quat globalRotation; /**< Global rotation of the object. */
	glm::vec3 globalScale; /**< Global scale of the object. */
	glm::vec3 globalForward; /**< Global forward direction of the object. */

	glm::vec3 position; /**< Local position of the object. */
	glm::quat rotation; /**< Local rotation of the object. */
	glm::vec3 scale; /**< Local scale of the object. */

	glm::vec3 up; /**< Up vector of the object. */
	glm::vec3 right; /**< Right vector of the object. */
	glm::vec3 forward; /**< Forward vector of the object. */

	Shader* shaderProgram; /**< Pointer to the shader program used for rendering the object. */

	float lastUpdateTime; /**< Last update time of the object. */
	float frameTime; /**< Frame time of the object. */
	ObjectList children; /**< List of child objects. */
	ObjectInstance* parent = nullptr; /**< Pointer to the parent object. */
	std::list<ObjectInstance*>::iterator iteratorAtParent; /**< Iterator pointing to the object's position in the parent's child list. */

	int index = 0; /**< Index of the object. */

public:
	/**
		*\brief Constructor for the ObjectInstance class.
		* \param shdrPrg Pointer to the shader program.
		*/
	ObjectInstance(Shader * shdrPrg = nullptr);

	/**
	 * \brief Destructor for the ObjectInstance class.
	 */
	~ObjectInstance();

	/**
	 * \brief Adds a child object to this object.
	 * \param child Pointer to the child object.
	 */
	void addChild(ObjectInstance * child);

	/**
	 * \brief Removes a child object from this object.
	 * \param childToRemove Pointer to the child object to remove.
	 */
	void removeChild(ObjectInstance * childToRemove);

	/**
	 * \brief Removes this object from its parent.
	 */
	void removeFromParent();

	/**
	 * \brief Retrieves the position of the object in local coordinates.
	 * \return The position of the object.
	 */
	glm::vec3 getPosition();

	/**
	 * \brief Sets the position of the object in local coordinates.
	 * \param position The position to set.
	 */
	void setPosition(glm::vec3 position);

	/**
	 * \brief Adds a displacement to the current position of the object in local coordinates.
	 * \param move The displacement vector to add.
	 */
	void addPosition(glm::vec3 move);

	/**
	 * \brief Retrieves the rotation of the object in radians.
	 * \return The rotation of the object in radians.
	 */
	glm::vec3 getRotationRad();

	/**
	 * \brief Retrieves the rotation of the object in degrees.
	 * \return The rotation of the object in degrees.
	 */
	glm::vec3 getRotationDeg();

	/**
	 * \brief Retrieves the rotation of the object as a quaternion.
	 * \return The rotation of the object as a quaternion.
	 */
	glm::quat getRotation();

	/**
	 * \brief Sets the rotation of the object in radians.
	 * \param rotation The rotation to set in radians.
	 */
	void setRotationRad(glm::vec3 rotation);

	/**
	 * \brief Sets the rotation of the object in degrees.
	 * \param rotation The rotation to set in degrees.
	 */
	void setRotationDeg(glm::vec3 rotation);

	/**
	 * \brief Sets the rotation of the object as a quaternion.
	 * \param rotation The rotation to set as a quaternion.
	 */
	void setRotation(glm::quat rotation);

	/**
	 * \brief Retrieves the scale of the object.
	 * \return The scale of the object.
	 */
	glm::vec3 getScale();

	/**
	 * \brief Sets the scale of the object.
	 * \param scale The scale to set.
	 */
	void setScale(glm::vec3 scale);

	/**
	 * \brief Retrieves the up vector of the object.
	 * \return The up vector of the object.
	 */
	glm::vec3 getUp();

	/**
	 * \brief Retrieves the right vector of the object.
	 * \return The right vector of the object.
	 */
	glm::vec3 getRight();

	/**
	 * \brief Retrieves the forward vector of the object.
	 * \return The forward vector of the object.
	 */
	glm::vec3 getForward();

	/**
	 * \brief Retrieves the global position of the object.
	 * \return The global position of the object.
	 */
	glm::vec3 getGlobalPosition();

	/**
	 * \brief Retrieves the global rotation of the object.
	 * \return The global rotation of the object.
	 */
	glm::quat getGlobalRotation();

	/**
	 * \brief Retrieves the global scale of the object.
	 * \return The global scale of the object.
	 */
	glm::vec3 getGlobalScale();

	/**
	 * \brief Retrieves the global forward vector of the object.
	 * \return The global forward vector of the object.
	 */
	glm::vec3 getGlobalForward();

	/**
	 * \brief Rotates the object around the X-axis in radians.
	 * \param angleRad The rotation angle in radians.
	 */
	void rotateRadX(float angleRad);

	/**
	 * \brief Rotates the object around the Y-axis in radians.
	 * \param angleRad The rotation angle in radians.
	 */
	void rotateRadY(float angleRad);

	/**
	 * \brief Rotates the object around the Z-axis in radians.
	 * \param angleRad The rotation angle in radians.
	 */
	void rotateRadZ(float angleRad);

	/**
	 * \brief Rotates the object around the X-axis in degrees.
	 * \param angleRad The rotation angle in degrees.
	 */
	void rotateDegX(float angleRad);

	/**
	 * \brief Rotates the object around the Y-axis in degrees.
	 * \param angleRad The rotation angle in degrees.
	 */
	void rotateDegY(float angleRad);

	/**
	 * \brief Rotates the object around the Z-axis in degrees.
	 * \param angleRad The rotation angle in degrees.
	 */
	void rotateDegZ(float angleRad);

	/**
	 * \brief Retrieves the index of the object.
	 * \return The index of the object.
	 */
	int getIndex() { return index; }

	/**
	 * \brief Sets the index of the object.
	 * \param indexToSet The index to set.
	 */
	void setIndex(int indexToSet) { index = indexToSet; }

	/**
	 * \brief Retrieves the last update time of the object.
	 * \return The last update time of the object.
	 */
	float getLastUpdateTime();

	/**
	 * \brief Retrieves the frame time of the object.
	 * \param elapsedTime The elapsed time since the last frame.
	 * \return The frame time of the object.
	 */
	float getFrameTime(float elapsedTime);

	/**
	 * \brief Handles the interaction with the object.
	 */
	virtual void Interact();

	/**
	 * \brief Finds the first child node of the specified type.
	 * \tparam T The type of the node to find.
	 * \return Pointer to the first child node of the specified type, or nullptr if not found.
	 */
	template<class T>
	T* firstNodeByType() {
		T* result = dynamic_cast<T*>(this);
		if (result != nullptr)
			return result;

		for (ObjectInstance* child : children) {
			if (child != nullptr) {
				result = child->firstNodeByType<T>();
				if (result != nullptr)
					return result;
			}
		}

		return nullptr;
	}

	/**
	 * \brief Finds the first node with the specified index.
	 * \param index The index to find.
	 * \return Pointer to the first node with the specified index, or nullptr if not found.
	 */
	ObjectInstance* firstNodeByIndex(int index) {
		if (index == getIndex())
			return this;

		for (ObjectInstance* child : children) {
			if (child != nullptr) {
				ObjectInstance* result = child->firstNodeByIndex(index);
				if (result != nullptr)
					return result;
			}
		}

		return nullptr;
	}

	/**
	 * \brief Updates the object.
	 * \param elapsedTime The elapsed time since the last update.
	 * \param parentModelMatrix Pointer to the parent's model matrix.
	 */
	virtual void update(const float elapsedTime, const glm::mat4* parentModelMatrix);

	/**
	 * \brief Draws the object.
	 */
	virtual void draw();

	/**
	 * \brief Initializes the object.
	 */
	virtual void initialize();

	/**
	 * \brief Handles key press events.
	 * \param key The key that was pressed.
	 */
	virtual void onKeyPress(unsigned char key);

	/**
	 * \brief Handles special key press events.
	 * \param key The special key that was pressed.
	 */
	virtual void onSpecialKeyPress(int key);

	/**
	 * \brief Handles mouse move events.
	 * \param deltaX The change in X-coordinate of the mouse.
	 * \param deltaY The change in Y-coordinate of the mouse.
	 * \param mouseX The current X-coordinate of the mouse.
	 * \param mouseY The current Y-coordinate of the mouse.
	 */
	virtual void onMouseMove(int deltaX, int deltaY, int mouseX, int mouseY);

	/**
	 * \brief Deserializes the object from JSON data.
	 * \param data The JSON data to deserialize from.
	 */
	virtual void deserialize(nlohmann::json data);

protected:
	/**
	 * \brief Prints the object graph.
	 * \param level The current level of the object in the graph.
	 */
	void printGraph(int level);

	/**
	 * \brief Updates the local vectors of the object.
	 */
	void updateLocalVectors();

	/**
	 * \brief Updates the local model matrix of the object.
	 */
	void updateLocalMatrix();

	/**
	 * \brief Updates the world matrix of the object.
	 * \param parentMatrix The parent's model matrix.
	 */
	void updateWorldMatrix(glm::mat4 parentMatrix);
};





 