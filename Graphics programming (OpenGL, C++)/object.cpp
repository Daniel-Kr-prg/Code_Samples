//----------------------------------------------------------------------------------------
/**
 * \file    object.cpp : ObjectInstance class implementation. ObjectInstance is used for any object on the scene
 * \author  Daniil Kryvarot
 * \date    2023/05/15
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

#include "object.h"

#include "shader.h"
#include "render.h"
#include <iostream>

/**
 * \brief ObjectInstance constructor. Takes a pointer to the shader and must create object resources (VBO and VAO)
 * \param shdrPrg pointer to the shader program for rendering objects data
 */
ObjectInstance::ObjectInstance(Shader* shdrPrg) : geometry(nullptr), shaderProgram(shdrPrg)
{
	this->position = { 0, 0, 0 };
	this->rotation = glm::quat(1, 0, 0, 0);
	this->scale = { 1, 1, 1 };
	updateLocalVectors();
}

ObjectInstance::~ObjectInstance() {}

void ObjectInstance::addChild(ObjectInstance* child)
{
	if (child != nullptr)
	{
		if (child->parent != nullptr)
		{
			child->parent->children.erase(child->iteratorAtParent);
		}
		children.push_back(child);
		child->parent = this;
		child->iteratorAtParent = --children.end();
	}
}

void ObjectInstance::removeChild(ObjectInstance* childToRemove)
{
	if (childToRemove != nullptr && childToRemove->parent == this)
	{
		Render::getRender()->getRootNode()->addChild(childToRemove);
	}
}

void ObjectInstance::removeFromParent()
{
	if (parent != nullptr)
	{
		parent->removeChild(this);
	}
}

/**
 * \brief Returns the current local position
 */
glm::vec3 ObjectInstance::getPosition() 
{
	return position;
}

/**
 * \brief Returns the current global position
 */
glm::vec3 ObjectInstance::getGlobalPosition()
{
	return globalPosition;
}

/**
 * \brief Sets the current local position
 * \param position New local position
 */
void ObjectInstance::setPosition(glm::vec3 position)
{
	this->position = position;
	updateLocalMatrix();
}

void ObjectInstance::addPosition(glm::vec3 move)
{
	this->position += move;
	updateLocalMatrix();
}

void ObjectInstance::Interact()
{

}
/**
 * \brief Returns the current local rotation (Euler angeles, in radians)
 */
glm::vec3 ObjectInstance::getRotationRad()
{
	return glm::vec3(glm::eulerAngles(rotation));
}

/**
 * \brief Returns the current local rotation (Euler angeles, in degrees)
 */
glm::vec3 ObjectInstance::getRotationDeg() 
{
	return glm::vec3(glm::eulerAngles(rotation)) * 180.0f / glm::pi<float>();
}

/**
 * \brief Returns the current local rotation (quaternion)
 */
glm::quat ObjectInstance::getRotation() 
{
	return rotation;
}

/**
 * \brief Returns the current global rotation (quaternion)
 */
glm::quat ObjectInstance::getGlobalRotation()
{
	return globalRotation;
}

/**
 * \brief Sets the current local rotation
 * \param rotation New local rotation (Euler angeles, in radians)
 */
void ObjectInstance::setRotationRad(glm::vec3 rotation) 
{
	this->rotation = glm::quat(rotation);
	updateLocalVectors();
}

/**
 * \brief Sets the current local rotation
 * \param rotation New local rotation (Euler angeles, in degrees)
 */
void ObjectInstance::setRotationDeg(glm::vec3 rotation) 
{
	this->rotation = glm::quat(rotation / 180.0f * glm::pi<float>());
	updateLocalVectors();
}

/**
 * \brief Sets the current local rotation
 * \param rotation New local rotation (quaternion)
 */
void ObjectInstance::setRotation(glm::quat rotation)
{
	this->rotation = rotation;
	updateLocalVectors();
}

/**
 * \brief Returns the current local scale
 */
glm::vec3 ObjectInstance::getScale()
{
	return scale;
}

/**
 * \brief Returns the current global scale
 */
glm::vec3 ObjectInstance::getGlobalScale()
{
	return globalScale;
}


/**
 * \brief Sets the current local scale
 * \param scale New local scale
 */
void ObjectInstance::setScale(glm::vec3 scale)
{
	this->scale = scale;
	updateLocalMatrix();
}

/**
 * \brief Returns the current local up vector
 */
glm::vec3 ObjectInstance::getUp() 
{
	return up;
}

/**
 * \brief Returns the current local right vector
 */
glm::vec3 ObjectInstance::getRight()
{
	return right;
}

/**
 * \brief Returns the current local forward vector
 */
glm::vec3 ObjectInstance::getForward() 
{
	return forward;
}

/**
 * \brief Returns the current global forward vector
 */
glm::vec3 ObjectInstance::getGlobalForward()
{
	return globalForward;
}

/**
 * \brief Rotates the object around global X axis
 * \param angleRad Angle in radians
 */
void ObjectInstance::rotateRadX(float angleRad)
{
	rotation = glm::rotate(rotation, angleRad, { 1, 0, 0 });
	updateLocalVectors();
}

/**
 * \brief Rotates the object around global Y axis
 * \param angleRad Angle in radians
 */
void ObjectInstance::rotateRadY(float angleRad)
{
	rotation = glm::rotate(rotation, angleRad, { 0, 1, 0 });
	updateLocalVectors();
}

/**
 * \brief Rotates the object around global Z axis
 * \param angleRad Angle in radians
 */
void ObjectInstance::rotateRadZ(float angleRad)
{
	rotation = glm::rotate(rotation, angleRad, { 0, 0, 1 });
	updateLocalVectors();
}

/**
 * \brief Rotates the object around global X axis
 * \param angleRad Angle in degrees
 */
void ObjectInstance::rotateDegX(float angleRad) 
{
	rotation = glm::rotate(rotation, float(angleRad / 180.0f * glm::pi<float>()), { 0, 0, 1 });
	updateLocalVectors();
}

/**
 * \brief Rotates the object around global Y axis
 * \param angleRad Angle in degrees
 */
void ObjectInstance::rotateDegY(float angleRad) 
{
	rotation = glm::rotate(rotation, float(angleRad / 180.0f * glm::pi<float>()), { 0, 1, 0 });
	updateLocalVectors();
}

/**
 * \brief Rotates the object around global Z axis
 * \param angleRad Angle in degrees
 */
void ObjectInstance::rotateDegZ(float angleRad)
{
	rotation = glm::rotate(rotation, float(angleRad / 180.0f * glm::pi<float>()), { 0, 0, 1 });
	updateLocalVectors();
}

float ObjectInstance::getLastUpdateTime()
{
	return lastUpdateTime;
}

float ObjectInstance::getFrameTime(float elapsedTime)
{
	frameTime = elapsedTime - lastUpdateTime;
	lastUpdateTime = elapsedTime;
	return frameTime;
}

/**
* \brief Recalculates the global matrix and updates all children.
*   Derived classes should also call this method (using ObjectInstance::update()).
* \param elapsedTime time value in seconds, such as 0.001*glutGet(GLUT_ELAPSED_TIME) (conversion milliseconds => seconds)
* \param parentModelMatrix parent transformation in the scene-graph subtree
*/
void ObjectInstance::update(const float elapsedTime, const glm::mat4* parentModelMatrix) {
	// update model matrix - localModelMatrix - of the instance 
	// ...

	getFrameTime(elapsedTime);
	// if we have parent, multiply parent's matrix with ours
	updateWorldMatrix(*parentModelMatrix);

	// update all children
	for (ObjectInstance* child : children) {
		if (child != nullptr)
			child->update(elapsedTime, &globalModelMatrix);
	}
}

/**
 * \brief Draw instance geometry and calls the draw() on child nodes.
 */
void ObjectInstance::draw() {
	// draw instance geometry using globalModelMatrix
	// ...

	// process all children
	for (ObjectInstance* child : children) {   //for (auto child : children) {
		if (child != nullptr)
			child->draw();
	}
}

void ObjectInstance::initialize() {
	// update all children
	for (ObjectInstance* child : children) {
		if (child != nullptr)
			child->initialize();
	}
}

void ObjectInstance::onKeyPress(unsigned char key) {
	// update all children
	if (key == 'q' && this == Render::getRender()->getRootNode())
	{
		printGraph(0);
	}

	for (ObjectInstance* child : children) {
		if (child != nullptr)
			child->onKeyPress(key);
	}
}

void ObjectInstance::printGraph(int level)
{
	for(int i = 0; i < level; i++)
		std::cout << "\t";
	std::cout << typeid(this).name() << std::endl;

	for (ObjectInstance* child : children) {
		if (child != nullptr)
			child->printGraph(level + 1);
	}
}

void ObjectInstance::onSpecialKeyPress(int key) {
	// update all children
	for (ObjectInstance* child : children) {
		if (child != nullptr)
			child->onSpecialKeyPress(key);
	}
}

void ObjectInstance::onMouseMove(int deltaX, int deltaY, int mouseX, int mouseY) {
	// update all children
	for (ObjectInstance* child : children) {
		if (child != nullptr)
			child->onMouseMove(deltaX, deltaY, mouseX, mouseY);
	}
}

void ObjectInstance::deserialize(nlohmann::json data)
{
	if (data.contains("position"))
	{
		nlohmann::json positionData = data["position"];
		setPosition(readVectorFromJSON(positionData, glm::vec3(0)));
	}

	if (data.contains("rotation"))
	{
		nlohmann::json rotationData = data["rotation"];
		setRotationRad(readVectorFromJSON(rotationData, glm::vec3(0)));
	}

	if (data.contains("scale"))
	{
		nlohmann::json scaleData = data["scale"];
		setScale(readVectorFromJSON(scaleData, glm::vec3(1)));
	}
}

/**
 * \brief Updates object's local vectors (forward, up, right). Call after any rotation changes.
 */
void ObjectInstance::updateLocalVectors() {
	forward = { -2 * (rotation.x * rotation.z + rotation.w * rotation.y), -2 * (rotation.y * rotation.z - rotation.w * rotation.x), -(1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y)) };
	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, { 0, 1, 0 }));
	up = glm::normalize(glm::cross(right, forward));
	updateLocalMatrix();
}

/**
 * \brief Updates object's model matrix. Call after any transform changes.
 */
void ObjectInstance::updateLocalMatrix() {
	localModelMatrix = glm::mat4(1.0f);
	localModelMatrix = glm::translate(localModelMatrix, (glm::vec3)position);
	localModelMatrix = localModelMatrix * glm::toMat4(rotation);
	localModelMatrix = glm::scale(localModelMatrix, (glm::vec3)scale);
}

void ObjectInstance::updateWorldMatrix(glm::mat4 parentMatrix) {
	globalModelMatrix = parentMatrix * localModelMatrix;
	glm::vec3 globalSkew;
	glm::vec4 globalPerspective;
	glm::decompose(globalModelMatrix, globalScale, globalRotation, globalPosition, globalSkew, globalPerspective);
	glm::mat3 rotationMatrix = glm::mat3(globalModelMatrix);
	globalForward = -rotationMatrix[2];
}