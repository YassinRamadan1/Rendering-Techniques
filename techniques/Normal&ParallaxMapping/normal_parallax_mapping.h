#pragma once
#include "utility.h"
#include "camera.h"
#include "model.h"
#include "shader.h"

class NormalMapping
{
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMEBUFFER_WIDTH;
	int FRAMEBUFFER_HEIGHT;
	Input* INPUT;
	GLFWwindow* WINDOW;

	float timer, timeToUpdate;
	unsigned int textureIdHeight, textureIdAO;
	int textureSlotHeight, textureSlotAO;
	unsigned int vao, vbo;
	Camera camera;
	Shader shader, normalShader, normalParallaxShader, normalParallaxAOShader, lightShader;
	glm::mat4 modelMat;
	glm::mat4 view;
	glm::mat4 projection;
	Model model;

	void handleInput(float deltaTime);
	void draw();
public:
	NormalMapping(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window);
	void run(float deltaTime);
};