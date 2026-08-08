#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices_ = vertices, this->indices_ = indices, this->textures_ = textures;
	setupMesh();
}

void Mesh::draw(Shader& shader)
{
	unsigned int specular_map_number = 1, diffuse_map_number = 1, normal_map_number = 1;
	shader.use();
	for (int i = 0; i < textures_.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
		std::string number;
		if (textures_[i].type_ == "texture_diffuse")
			number = std::to_string(diffuse_map_number++);
		else if (textures_[i].type_ == "texture_specular")
			number = std::to_string(specular_map_number++);
		else if (textures_[i].type_ == "texture_normal")
			number = std::to_string(normal_map_number++);
		std::string textureName = std::string("material.") + textures_[i].type_ + number;
		shader.set1Int(textureName.c_str(), i);
	}

	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position_));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal_));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent_));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent_));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinates_));
	glEnableVertexAttribArray(4);
}