#pragma once
#include "utility.h"
#include "camera.h"
#include "model.h"
#include "shader.h"

class DirectLighting
{
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMEBUFFER_WIDTH;
	int FRAMEBUFFER_HEIGHT;
	Input* INPUT;
	GLFWwindow* WINDOW;

	unsigned int textureIdAlbedo, textureIdNormal, textureIdMetalness, textureIdRoughness, textureIdAO;
	int textureSlotAlbedo, textureSlotNormal, textureSlotMetalness, textureSlotRoughness, textureSlotAO;

	Camera camera;
	Shader shader, lightShader;
	glm::mat4 modelMat;
	glm::mat4 view;
	glm::mat4 projection;
	Model model;
	glm::vec3 lightPositions[4], lightColors[4];
	void handleInput(float deltaTime);

public:
	DirectLighting(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window);
	void run(float deltaTime);
};