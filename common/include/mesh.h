#pragma once
#include <iostream>
#include <vector>
#include "utility.h"
#include "Shader.h"

struct Vertex 
{
	glm::vec3 position_;
	glm::vec3 normal_;
	glm::vec3 tangent_;
	glm::vec3 biTangent_;
	glm::vec2 texture_coordinates_;
};

struct Texture
{
	unsigned int id_;
	std::string type_;
	std::string path_;
};

class Mesh 
{
public:

	std::vector<Vertex> vertices_;
	std::vector<unsigned int> indices_;
	std::vector<Texture> textures_;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader& shader);

	unsigned int vao_;
private:

	unsigned int vbo_, ebo_;
	
	void setupMesh();
};