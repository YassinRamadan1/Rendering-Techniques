#pragma once
#include "utility.h"
#include "camera.h"
#include "shader.h"

class Outlining
{
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMEBUFFER_WIDTH;
	int FRAMEBUFFER_HEIGHT;
	Input* INPUT;
	GLFWwindow* WINDOW;

	float timer, timeToUpdate;
	unsigned int textureId;
	int textureSlot;
	unsigned int vao, vbo;
	Camera camera;
	Shader shader, borderShader;
	glm::mat4 model, model2;
	glm::mat4 view;
	glm::mat4 projection;

	void handleInput(float deltaTime);
	void draw();
public:
	Outlining(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window);
	void run(float deltaTime);
};