#pragma once
//----------------------------------------------------------------------------------------
/**
\file render.h : Render class declaration
\author Daniil Kryvarot
\date 2023/05/15
\brief Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
*/
#include "pgr.h"
#include "object.h"
#include "shader.h"
#include "skyboxshader.h"

class DirectionalLight;
class PointLight;
class SpotLight;
class Config;
class Camera;
class ObjectInstance;
class Splash;


//#include "data.h"

// parameters of individual objects in the scene (e.g. position, size, speed, etc.)
typedef struct _Object {
	glm::vec3 position;
	glm::vec3 direction;
	float     speed;
	float     size;

	bool destroyed;

	float startTime;
	float currentTime;

} Object;

typedef struct _commonShaderProgram {
	// identifier for the shader program
	GLuint program;          // = 0;
	// vertex attributes locations
	GLint posLocation;       // = -1;
	GLint colorLocation;     // = -1;
	GLint normalLocation;    // = -1;
	GLint texCoordLocation;  // = -1;
	// uniforms locations
	GLint PVMmatrixLocation;    // = -1;
	GLint VmatrixLocation;      // = -1;  view/camera matrix
	GLint MmatrixLocation;      // = -1;  modeling matrix
	GLint normalMatrixLocation; // = -1;  inverse transposed Mmatrix

	GLint timeLocation;         // = -1; elapsed time in seconds

	// material 
	GLint diffuseLocation;    // = -1;
	GLint ambientLocation;    // = -1;
	GLint specularLocation;   // = -1;
	GLint shininessLocation;  // = -1;
	// texture
	GLint useTextureLocation; // = -1; 
	GLint texSamplerLocation; // = -1;
	// reflector related uniforms
	GLint reflectorPositionLocation;  // = -1; 
	GLint reflectorDirectionLocation; // = -1;
} SCommonShaderProgram;

class Render {

public:

    /**
     * \brief Sets the material uniforms in the specified shader program.
     * \param shaderProgram The shader program to set the uniforms in.
     * \param ambient The ambient color of the material.
     * \param diffuse The diffuse color of the material.
     * \param specular The specular color of the material.
     * \param shininess The shininess of the material.
     * \param texture The texture of the material.
     */
    void setMaterialUniforms(const ShaderProgram& shaderProgram, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, GLuint texture);

    /**
     * \brief Initializes the rendering system with the specified configuration.
     * \param config The configuration object.
     */
    void initialize(Config* config);

    /**
     * \brief Adds a shader to the rendering system.
     * \param shader The shader to add.
     */
    void addShader(Shader* shader);

    /**
     * \brief Creates a splash effect at the specified position.
     * \param position The position of the splash effect.
     */
    void splashAtPosition(glm::vec3 position);

    /**
     * \brief Sets the current splash effect.
     * \param splash The splash effect to set.
     */
    void setSplash(Splash* splash);

    /**
     * \brief Sets the camera to use for rendering.
     * \param camera The camera to set.
     */
    void setCamera(Camera* camera);

    /**
     * \brief Sets the directional light to use for rendering.
     * \param light The directional light to set.
     */
    void setDirectionalLight(DirectionalLight* light);

    /**
     * \brief Sets the point light to use for rendering.
     * \param light The point light to set.
     */
    void setPointLight(PointLight* light);

    /**
     * \brief Sets the spot light to use for rendering.
     * \param light The spot light to set.
     */
    void setSpotLight(SpotLight* light);

    /**
     * \brief Sets the uniforms for camera and lights in the specified shader.
     * \param shader The shader program to set the uniforms in.
     */
    void setCameraAndLightsUniforms(Shader* shader);

    /**
     * \brief Returns the current aspect ratio.
     * \return The current aspect ratio.
     */
    float getCurrentAspect();

    /**
     * \brief Returns the shader at the specified index.
     * \param index The index of the shader.
     * \return The shader at the specified index, or nullptr if not found.
     */
    Shader* getShader(int index);

    /**
     * \brief Initializes the skybox geometry with the specified shaders and file path.
     * \param skyVertexPath The vertex shader path for the skybox.
     * \param skyFragmentPath The fragment shader path for the skybox.
     * \param filePath The file path of the skybox textures.
     */
    void initializeSkyboxGeometry(std::string skyVertexPath, std::string skyFragmentPath, std::string filePath);

    /**
     * \brief Updates the rendering system.
     * \param elapsedTime The elapsed time since the last update.
     */
    void update(float elapsedTime);

    /**
     * \brief Ends the current draw operation.
     */
    void endDraw();

    /**
 * \brief Returns the root node of the scene.
 * \return Pointer to the root node of the scene.
 */
    ObjectInstance* getRootNode();

    /**
     * \brief Returns the instance of the Render class.
     * \return Pointer to the Render instance.
     */
    static Render* getRender();
private:
    ObjectInstance rootNode; ///< The root node of the scene.
    ShaderList shaders; ///< The list of shaders.
    SkyBoxShader* skyShader; ///< The skybox shader.

    Config* config; ///< The configuration object.
    Camera* camera; ///< The camera.
    DirectionalLight* directionalLight; ///< The directional light.
    PointLight* pointLight; ///< The point light.
    SpotLight* spotLight; ///< The spot light.

    ObjectGeometry* skyboxGeometry; ///< The skybox geometry.
    Splash* splash; ///< The splash effect.

    bool initialized = false; ///< Flag indicating whether the rendering system is initialized or not.
};