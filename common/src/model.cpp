#include "model.h"

Model::Model(std::string const& path)
{
	loadModel(path);
}

void Model::draw(Shader& shader)
{
	for (int i{ 0 }; i < meshes_.size(); ++i)
		meshes_[i].draw(shader);
}

void Model::loadModel(std::string const& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Error! " << importer.GetErrorString() << '\n';
		return;
	}

	directory_ = path.substr(0, path.find_last_of("/"));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	aiMesh* mesh;
	for (unsigned int i{ 0 }; i < node->mNumMeshes; ++i)
	{
		mesh = scene->mMeshes[node->mMeshes[i]];
		meshes_.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i{ 0 }; i < node->mNumChildren; ++i)
		processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Vertex vertex;

	for (unsigned int i{ 0 }; i < mesh->mNumVertices; ++i)
	{
		vertex.position_.x = mesh->mVertices[i].x;
		vertex.position_.y = mesh->mVertices[i].y;
		vertex.position_.z = mesh->mVertices[i].z;
		vertex.normal_.x = mesh->mNormals[i].x;
		vertex.normal_.y = mesh->mNormals[i].y;
		vertex.normal_.z = mesh->mNormals[i].z;
		vertex.tangent_.x = mesh->mTangents[i].x;
		vertex.tangent_.y = mesh->mTangents[i].y;
		vertex.tangent_.z = mesh->mTangents[i].z;
		vertex.biTangent_.x = mesh->mBitangents[i].x;
		vertex.biTangent_.y = mesh->mBitangents[i].y;
		vertex.biTangent_.z = mesh->mBitangents[i].z;

		if (mesh->mTextureCoords[0])
			vertex.texture_coordinates_ = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.texture_coordinates_ = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (unsigned int i{ 0 }; i < mesh->mNumFaces; ++i)
	{
		aiFace& face = mesh->mFaces[i];
		for (unsigned int j{ 0 }; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), maps.begin(), maps.end());

		maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), maps.begin(), maps.end());

		maps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), maps.begin(), maps.end());

		maps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), maps.begin(), maps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name)
{
	std::vector<Texture> textures;
	Texture texture;
	for (unsigned int i{ 0 }; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (int j{ 0 }; j < cached_textures_.size(); ++j)
			if (std::strcmp(cached_textures_[j].path_.data(), str.C_Str()) == 0)
			{
				skip = true;
				textures.push_back(cached_textures_[j]);
				break;
			}

		if (!skip)
		{
			if (type_name == "texture_normal")
				texture.id_ = loadTextureFromFile(str.C_Str(), directory_, false);
			else
				texture.id_ = loadTextureFromFile(str.C_Str(), directory_);
			texture.type_ = type_name;
			texture.path_ = str.C_Str();
			if (texture.id_)
				textures.push_back(texture);
			cached_textures_.push_back(texture);
		}
	}

	return textures;
}

unsigned int Model::loadTextureFromFile(const char* path, std::string directory, bool gamma)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels, format, internal_format;
	unsigned int id;
	std::string filename = (directory + "/" + path);

	unsigned char* image = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (!image) {

		std::cout << "Error! Couldn't load texture with path :" << filename << '\n';
		return 0;
	}

	format = nrChannels == 3 ? GL_RGB : nrChannels == 4 ? GL_RGBA : GL_RED;
	if (gamma)
		internal_format = nrChannels == 3 ? GL_SRGB : nrChannels == 4 ? GL_SRGB_ALPHA : GL_RED;
	else
		internal_format = format;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(image);

	return id;
}