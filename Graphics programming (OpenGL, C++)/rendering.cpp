#include <iostream>
#include "pgr.h"
#include "rendering.h"

SCommonShaderProgram shaderProgram;

bool useLighting = false;

bool loadSingleMesh(const std::string& fileName, SCommonShaderProgram& shader, MeshGeometry** geometry) {
    Assimp::Importer importer;

    // Unitize object in size (scale the model to fit into (-1..1)^3)
    importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

    // Load asset from the file - you can play with various processing steps
    const aiScene* scn = importer.ReadFile(fileName.c_str(), 0
        | aiProcess_Triangulate             // Triangulate polygons (if any).
        | aiProcess_PreTransformVertices    // Transforms scene hierarchy into one root with geometry-leafs only. For more see Doc.
        | aiProcess_GenSmoothNormals        // Calculate normals per vertex.
        | aiProcess_JoinIdenticalVertices);

    // abort if the loader fails
    if (scn == NULL) {
        std::cerr << "assimp error: " << importer.GetErrorString() << std::endl;
        *geometry = NULL;
        return false;
    }

    // some formats store whole scene (multiple meshes and materials, lights, cameras, ...) in one file, we cannot handle that in our simplified example
    if (scn->mNumMeshes != 1) {
        std::cerr << "this simplified loader can only process files with only one mesh" << std::endl;
        *geometry = NULL;
        return false;
    }

    // in this phase we know we have one mesh in our loaded scene, we can directly copy its data to OpenGL ...
    const aiMesh* mesh = scn->mMeshes[0];

    *geometry = new MeshGeometry;

    // vertex buffer object, store all vertex positions and normals
    glGenBuffers(1, &((*geometry)->vertexBufferObject));
    glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * mesh->mNumVertices, 0, GL_STATIC_DRAW); // allocate memory for vertices, normals, and texture coordinates
    // first store all vertices
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * mesh->mNumVertices, mesh->mVertices);
    // then store all normals
    glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * mesh->mNumVertices, 3 * sizeof(float) * mesh->mNumVertices, mesh->mNormals);

    // just texture 0 for now
    float* textureCoords = new float[2 * mesh->mNumVertices];  // 2 floats per vertex
    float* currentTextureCoord = textureCoords;

    // copy texture coordinates
    aiVector3D vect;

    if (mesh->HasTextureCoords(0)) {
        // we use 2D textures with 2 coordinates and ignore the third coordinate
        for (unsigned int idx = 0; idx < mesh->mNumVertices; idx++) {
            vect = (mesh->mTextureCoords[0])[idx];
            *currentTextureCoord++ = vect.x;
            *currentTextureCoord++ = vect.y;
        }
    }

    // finally store all texture coordinates
    glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float) * mesh->mNumVertices, 2 * sizeof(float) * mesh->mNumVertices, textureCoords);

    // copy all mesh faces into one big array (assimp supports faces with ordinary number of vertices, we use only 3 -> triangles)
    unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];
    for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
        indices[f * 3 + 0] = mesh->mFaces[f].mIndices[0];
        indices[f * 3 + 1] = mesh->mFaces[f].mIndices[1];
        indices[f * 3 + 2] = mesh->mFaces[f].mIndices[2];
    }

    // copy our temporary index array to OpenGL and free the array
    glGenBuffers(1, &((*geometry)->elementBufferObject));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned) * mesh->mNumFaces, indices, GL_STATIC_DRAW);

    delete[] indices;

    // copy the material info to MeshGeometry structure
    const aiMaterial* mat = scn->mMaterials[mesh->mMaterialIndex];
    aiColor4D color;
    aiString name;
    aiReturn retValue = AI_SUCCESS;

    // Get returns: aiReturn_SUCCESS 0 | aiReturn_FAILURE -1 | aiReturn_OUTOFMEMORY -3
    mat->Get(AI_MATKEY_NAME, name); // may be "" after the input mesh processing. Must be aiString type!

    if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color)) != AI_SUCCESS)
        color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);

    (*geometry)->diffuse = glm::vec3(color.r, color.g, color.b);

    if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &color)) != AI_SUCCESS)
        color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
    (*geometry)->ambient = glm::vec3(color.r, color.g, color.b);

    if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &color)) != AI_SUCCESS)
        color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
    (*geometry)->specular = glm::vec3(color.r, color.g, color.b);

    ai_real shininess, strength;
    unsigned int max;	// changed: to unsigned

    max = 1;
    if ((retValue = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &shininess, &max)) != AI_SUCCESS)
        shininess = 1.0f;
    max = 1;
    if ((retValue = aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS_STRENGTH, &strength, &max)) != AI_SUCCESS)
        strength = 1.0f;
    (*geometry)->shininess = shininess * strength;

    (*geometry)->texture = 0;

    // load texture image
    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        // get texture name 
        aiString path; // filename

        aiReturn texFound = mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
        std::string textureName = path.data;

        size_t found = fileName.find_last_of("/\\");
        // insert correct texture file path 
        if (found != std::string::npos) { // not found
            //subMesh_p->textureName.insert(0, "/");
            textureName.insert(0, fileName.substr(0, found + 1));
        }

        std::cout << "Loading texture file: " << textureName << std::endl;
        (*geometry)->texture = pgr::createTexture(textureName);
    }
    CHECK_GL_ERROR();

    glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
    glBindVertexArray((*geometry)->vertexArrayObject);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject); // bind our element array buffer (indices) to vao
    glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);

    glEnableVertexAttribArray(shader.posLocation);
    glVertexAttribPointer(shader.posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (useLighting == true) {
        glEnableVertexAttribArray(shader.normalLocation);
        glVertexAttribPointer(shader.normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float) * mesh->mNumVertices));
    }
    else {
        glDisableVertexAttribArray(shader.colorLocation);
        // following line is problematic on AMD/ATI graphic cards
        // -> if you see black screen (no objects at all) than try to set color manually in vertex shader to see at least something
        glVertexAttrib3f(shader.colorLocation, color.r, color.g, color.b);
    }

    glEnableVertexAttribArray(shader.texCoordLocation);
    glVertexAttribPointer(shader.texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * sizeof(float) * mesh->mNumVertices));
    CHECK_GL_ERROR();

    glBindVertexArray(0);

    (*geometry)->numTriangles = mesh->mNumFaces;

    return true;
}

void setTransformUniforms(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

    glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv(shaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

    glUniformMatrix4fv(shaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shaderProgram.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // just take 3x3 rotation part of the modelMatrix
    // we presume the last row contains 0,0,0,1
    const glm::mat4 modelRotationMatrix = glm::mat4(
        modelMatrix[0],
        modelMatrix[1],
        modelMatrix[2],
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelRotationMatrix));

    //or an alternative single-line method: 
    //glm::mat4 normalMatrix = glm::transpose(glm::inverse(glm::mat4(glm::mat3(modelRotationMatrix))));

    glUniformMatrix4fv(shaderProgram.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));  // correct matrix for non-rigid transform
}

void setMaterialUniforms(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, GLuint texture) {

    glUniform3fv(shaderProgram.diffuseLocation, 1, glm::value_ptr(diffuse));  // 2nd parameter must be 1 - it declares number of vectors in the vector array
    glUniform3fv(shaderProgram.ambientLocation, 1, glm::value_ptr(ambient));
    glUniform3fv(shaderProgram.specularLocation, 1, glm::value_ptr(specular));
    glUniform1f(shaderProgram.shininessLocation, shininess);

    if (texture != 0) {
        glUniform1i(shaderProgram.useTextureLocation, 1);  // do texture sampling
        glUniform1i(shaderProgram.texSamplerLocation, 0);  // texturing unit 0 -> samplerID   [for the GPU linker]
        glActiveTexture(GL_TEXTURE0 + 0);                  // texturing unit 0 -> to be bound [for OpenGL BindTexture]
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    else {
        glUniform1i(shaderProgram.useTextureLocation, 0);  // do not sample the texture
    }
}