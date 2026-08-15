#pragma once
#include "utility.h"
#include "camera.h"
#include "model.h"
#include "shader.h"

class ShadowMapping
{
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMEBUFFER_WIDTH;
	int FRAMEBUFFER_HEIGHT;
	Input* INPUT;
	GLFWwindow* WINDOW;

	unsigned int textureIdBackPackSpec, textureIdCubeSpec, textureIdCube, textureIdPlane;
	int textureSlotBackPackSpec, textureSlotCubeSpec, textureSlotCube, textureSlotPlane;
	unsigned int quad_vao, quad_vbo, plane_vao, plane_vbo, cube_vao, cube_vbo;
	unsigned int fbo, depthAttachment[2];
	Camera camera;
	Shader planeShader, emptyPlaneShader, cubeShader, emptyCubeShader, modelShader, emptyModelShader;
	glm::mat4 modelMat;
	glm::mat4 view, dirLightView[2];
	glm::mat4 projection, dirLightProjection;
	glm::mat4 dirLightTransform[2];
	Model model;
	PointLight l[4];
	DirLight dirL[2];

	void handleInput(float deltaTime);

	void renderSceneLightView(int i);

	void renderSceneCameraView();
public:
	ShadowMapping(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window);
	void run(float deltaTime);
};