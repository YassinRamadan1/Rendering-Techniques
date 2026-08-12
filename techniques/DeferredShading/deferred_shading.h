#pragma once
#include "utility.h"
#include "camera.h"
#include "model.h"
#include "shader.h"

class DeferredShading
{
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMEBUFFER_WIDTH;
	int FRAMEBUFFER_HEIGHT;
	Input* INPUT;
	GLFWwindow* WINDOW;

	unsigned int textureIdSpec;
	int textureSlotSpec;
	unsigned int vao, vbo, quad_vao, quad_vbo;
	unsigned int fbo, depthAttachment, colorAttachments[3];
	Camera camera;
	Shader lightPassShader, geometryPassShader, lightCubeShader;
	glm::mat4 modelMat;
	glm::mat4 view;
	glm::mat4 projection;
	Model model;
	PointLight l[4];

	void handleInput(float deltaTime);
	void draw();
public:
	DeferredShading(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window);
	void run(float deltaTime);
};