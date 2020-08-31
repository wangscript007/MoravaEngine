#pragma once

#include <assimp/scene.h>

#include "MeshSSAO.h"


/**
 * Model class compatible with LearnOpenGL code / shaders
 * Vertex buffer structure as follows:
 *
 * layout (location = 0) in vec3 aPos;
 * layout (location = 1) in vec3 aNormal;
 * layout (location = 2) in vec2 aTexCoords;
 */
class ModelSSAO
{

public:
    ModelSSAO(std::string const& path, std::string const& textureDirectory = "", bool gamma = false);
    virtual ~ModelSSAO();

    void Draw(Shader* shader);
    inline std::vector<TextureData> GetTextures() { return textures_loaded; };
    inline std::vector<MeshSSAO> GetMeshes() { return meshes; };

private:
    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    MeshSSAO processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureData> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

protected:
    std::vector<TextureData> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<MeshSSAO> meshes;
    std::string m_ModelDirectory;
    std::string m_TextureDirectory;
    bool gammaCorrection;

};
