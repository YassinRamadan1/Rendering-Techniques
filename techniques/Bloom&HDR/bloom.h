#pragma once
#include "utility.h"
#include "camera.h"
#include "shader.h"

struct PointLight
{
	float constant;
	float linear;
	float quadratic;

	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class Bloom
{
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMEBUFFER_WIDTH;
	int FRAMEBUFFER_HEIGHT;
	Input* INPUT;
	GLFWwindow* WINDOW;

	unsigned int textureIdCube, textureIdPlane, textureIdCubeSpec;
	int textureSlotCube, textureSlotPlane, textureSlotCubeSpec;
	unsigned int cube_vao, plane_vao, quad_vao, cube_vbo, plane_vbo, quad_vbo;
	Camera camera;
	Shader lightShader, cubeShader, planeShader, quadShader, blurShader;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	PointLight l[4];

	unsigned int fbo, colorAttachment0Tex, colorAttachment1Tex, rbo;
	unsigned int fbo2, colorAttachment2Tex, fbo3, colorAttachment3Tex;

	void handleInput(float deltaTime);
	void draw();
	void renderScene();
public:
	Bloom(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window);
	void run(float deltaTime);
};