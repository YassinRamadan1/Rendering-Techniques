#pragma once
#include "utility.h"
#include "camera.h"
#include "model.h"
#include "shader.h"

class PBR
{
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMEBUFFER_WIDTH;
	int FRAMEBUFFER_HEIGHT;
	Input* INPUT;
	GLFWwindow* WINDOW;

	unsigned int textureIdAlbedo, textureIdNormal, textureIdMetalness, textureIdRoughness, textureIdAO, textureIdHDR;
	int textureSlotAlbedo, textureSlotNormal, textureSlotMetalness, textureSlotRoughness, textureSlotAO, textureSlotHDR;
	unsigned int textureIdEnvMap, textureIdIrradianceMap, textureIdPrefilteredMap, textureIdBRDFMap;
	int textureSlotEnvMap, textureSlotIrradianceMap, textureSlotPrefilteredMap, textureSlotBRDFMap;
	int cubeFaceDimension;
	
	unsigned int cube_vao, cube_vbo, envMapFbo, quad_vao, quad_vbo;
	Camera camera;
	Shader shader, envMapShader, irradianceMapShader, prefilteredMapShader, skyboxShader, lightShader, brdfMapShader;
	glm::mat4 modelMat;
	glm::mat4 view, cubeFaceView[6];
	glm::mat4 projection, cubeFaceProjection;
	Model sphereModel, model;
	glm::vec3 lightPositions[4], lightColors[4];
	void handleInput(float deltaTime);

public:
	PBR(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window);
	void run(float deltaTime);
};