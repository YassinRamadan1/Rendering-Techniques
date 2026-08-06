#pragma once
#include "utility.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

class Model
{
public:

	Model(std::string const &path);
	void draw(Shader& shader);

	std::vector<Mesh> meshes_;
private:
	std::vector<Texture> cached_textures_;
	std::string directory_;

	void loadModel(std::string const &path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int loadTextureFromFile(const char* path, std::string directory, bool gamma = true);
};