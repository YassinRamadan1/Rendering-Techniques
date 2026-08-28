#include "pbr.h"

PBR::PBR(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window)
	: SCREEN_WIDTH(scrWidth), SCREEN_HEIGHT(scrHeight), FRAMEBUFFER_WIDTH(fbWidth), FRAMEBUFFER_HEIGHT(fbHeight),
	INPUT(&input), WINDOW(window), sphereModel(std::string(RESOURCES_PATH) + "models/sphere/sphere.glb"),
	model(std::string(RESOURCES_PATH) + "models/gun/gun.fbx")
{

	lightPositions[0] = glm::vec3(0.0, 10.0, 0.0);
	lightPositions[1] = glm::vec3(-5.0, 2.0, -3.0);
	lightPositions[2] = glm::vec3(5.0, 1.5, 3.0);
	lightPositions[3] = glm::vec3(0.0, 4.0, 8.0);

	lightColors[0] = glm::vec3(8.0, 8.0, 8.0);
	lightColors[1] = glm::vec3(6.0, 4.0, 2.0);
	lightColors[2] = glm::vec3(1.0, 6.0, 8.0);
	lightColors[3] = glm::vec3(10.0, 0.0, 0.0);

	std::string texAlbedo = std::string(RESOURCES_PATH) + "models/gun/albedo.tga";
	std::string texNormal = std::string(RESOURCES_PATH) + "models/gun/normal.tga";
	std::string texMetalness = std::string(RESOURCES_PATH) + "models/gun/metalness.tga";
	std::string texRoughness = std::string(RESOURCES_PATH) + "models/gun/roughness.tga";
	std::string texAO = std::string(RESOURCES_PATH) + "models/gun/ao.tga";
	std::string equirectangularMap = std::string(RESOURCES_PATH) + "textures/pbr/hdr/newport_loft.hdr";

	textureSlotAlbedo = 5;
	textureSlotNormal = 6;
	textureSlotMetalness = 7;
	textureSlotRoughness = 8;
	textureSlotAO = 9;
	textureSlotHDR = 10;
	textureSlotEnvMap = 11;
	textureSlotIrradianceMap = 12;
	textureSlotPrefilteredMap = 13;
	textureSlotBRDFMap = 14;
	textureIdAlbedo = loadTexture2D(texAlbedo.c_str(), textureSlotAlbedo, false);
	textureIdNormal = loadTexture2D(texNormal.c_str(), textureSlotNormal, false);
	textureIdMetalness = loadTexture2D(texMetalness.c_str(), textureSlotMetalness, false);
	textureIdRoughness = loadTexture2D(texRoughness.c_str(), textureSlotRoughness, false);
	textureIdAO = loadTexture2D(texAO.c_str(), textureSlotAO, false);
	textureIdHDR = loadHDRTexture2D(equirectangularMap.c_str(), textureSlotHDR);

	std::string e;
	std::string s = std::string(TECHNIQUES_PATH);
	std::string vsPath = s + "PBR/shaders/pbr_vs.shader";
	std::string fsPath = s + "PBR/shaders/pbr_fs.shader";
	shader.createProgram(vsPath, fsPath, e);

	vsPath = s + "PBR/shaders/envCube_vs.shader";
	fsPath = s + "PBR/shaders/envCube_fs.shader";
	envMapShader.createProgram(vsPath, fsPath, e);
	
	vsPath = s + "PBR/shaders/envCube_vs.shader";
	fsPath = s + "PBR/shaders/irradianceCube_fs.shader";
	irradianceMapShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "PBR/shaders/prefilteredCube_vs.shader";
	fsPath = s + "PBR/shaders/prefilteredCube_fs.shader";
	prefilteredMapShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "PBR/shaders/brdfQuad_vs.shader";
	fsPath = s + "PBR/shaders/brdfQuad_fs.shader";
	brdfMapShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "PBR/shaders/skybox_vs.shader";
	fsPath = s + "PBR/shaders/skybox_fs.shader";
	skyboxShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "PBR/shaders/light_vs.shader";
	fsPath = s + "PBR/shaders/light_fs.shader";
	lightShader.createProgram(vsPath, fsPath, e);

	float cubeVertices[] =
	{
		// back face (z = -0.5), normal (0,0,-1)
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		// front face (z = 0.5), normal (0,0,1)
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

		// left face (x = -0.5), normal (-1,0,0)
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		// right face (x = 0.5), normal (1,0,0)
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 // bottom face (y = -0.5), normal (0,-1,0)
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		 // top face (y = 0.5), normal (0,1,0)
		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	};

	float quadVertices[] =
	{
		-1, -1,
		1, -1,
		-1, 1,

		1, -1,
		1, 1,
		-1, 1
	};

	glGenVertexArrays(1, &quad_vao);
	glBindVertexArray(quad_vao);

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetFramebufferSize(WINDOW, &FRAMEBUFFER_WIDTH, &FRAMEBUFFER_HEIGHT);
	glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	glClearColor(0, 0, 0, 1.);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	cubeFaceDimension = 512;
	cubeFaceProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	cubeFaceView[0] = glm::lookAt(glm::vec3(0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	cubeFaceView[1] = glm::lookAt(glm::vec3(0), glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	cubeFaceView[2] = glm::lookAt(glm::vec3(0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	cubeFaceView[3] = glm::lookAt(glm::vec3(0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
	cubeFaceView[4] = glm::lookAt(glm::vec3(0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
	cubeFaceView[5] = glm::lookAt(glm::vec3(0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

	glGenTextures(1, &textureIdEnvMap);
	glActiveTexture(GL_TEXTURE0 + textureSlotEnvMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIdEnvMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cubeFaceDimension, cubeFaceDimension, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glGenFramebuffers(1, &envMapFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, envMapFbo);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, cubeFaceDimension, cubeFaceDimension);
	glClearColor(0, 0, 0, 1);
	envMapShader.use();
	envMapShader.setMatrix4f("projection", false, glm::value_ptr(cubeFaceProjection));
	glActiveTexture(GL_TEXTURE0 + textureSlotHDR);
	glBindTexture(GL_TEXTURE_2D, textureIdHDR);
	envMapShader.set1Int("tex_equiRectangular", textureSlotHDR);
	glBindVertexArray(cube_vao);
	for (int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureIdEnvMap, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		envMapShader.setMatrix4f("view", false, glm::value_ptr(cubeFaceView[i]));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glActiveTexture(GL_TEXTURE0 + textureSlotEnvMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIdEnvMap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glGenTextures(1, &textureIdIrradianceMap);
	glActiveTexture(GL_TEXTURE0 + textureSlotIrradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIdIrradianceMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	irradianceMapShader.use();
	irradianceMapShader.setMatrix4f("projection", false, glm::value_ptr(cubeFaceProjection));
	glActiveTexture(GL_TEXTURE0 + textureSlotEnvMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIdEnvMap);
	irradianceMapShader.set1Int("tex_environment", textureSlotEnvMap);
	glViewport(0, 0, 32, 32);
	glBindVertexArray(cube_vao);
	for (int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureIdIrradianceMap, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		irradianceMapShader.setMatrix4f("view", false, glm::value_ptr(cubeFaceView[i]));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glGenTextures(1, &textureIdPrefilteredMap);
	glActiveTexture(GL_TEXTURE0 + textureSlotPrefilteredMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIdPrefilteredMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	int maxMipLevel = 5;

	prefilteredMapShader.use();
	glActiveTexture(GL_TEXTURE0 + textureSlotEnvMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIdEnvMap);
	prefilteredMapShader.set1Int("tex_environment", textureSlotEnvMap);
	prefilteredMapShader.setMatrix4f("projection", false, glm::value_ptr(cubeFaceProjection));
	glBindVertexArray(cube_vao);
	for (int i = 0; i < maxMipLevel; ++i)
	{
		int mipWidth = 128 * pow(0.5, i);
		int mipHeight = 128 * pow(0.5, i);
		float roughness = i / (maxMipLevel - 1.f);
		prefilteredMapShader.set1Float("roughness", roughness);
		glViewport(0, 0, mipWidth, mipHeight);
		for (int j = 0; j < 6; ++j)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, textureIdPrefilteredMap, i);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			prefilteredMapShader.setMatrix4f("view", false, glm::value_ptr(cubeFaceView[j]));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	glGenTextures(1, &textureIdBRDFMap);
	glActiveTexture(GL_TEXTURE0 + textureSlotBRDFMap);
	glBindTexture(GL_TEXTURE_2D, textureIdBRDFMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glViewport(0, 0, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureIdBRDFMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	brdfMapShader.use();
	brdfMapShader.set2Float("resolution", 512, 512);
	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
}

void PBR::run(float deltaTime)
{
	handleInput(deltaTime);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(camera.zoom), SCREEN_WIDTH * 1.F / SCREEN_HEIGHT, .1F, 100.F);

	lightShader.use();
	lightShader.setMatrix4f("view", false, glm::value_ptr(view));
	lightShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	for (int i = 0; i < 4; ++i)
	{
		modelMat = glm::translate(glm::mat4(1.), lightPositions[i]);
		modelMat = modelMat * glm::scale(glm::mat4(1.), glm::vec3(1.));
		lightShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
		lightShader.set3Float("lightColor", lightColors[i].x, lightColors[i].y, lightColors[i].z);
		sphereModel.draw(lightShader);
	}

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(0., -2., -7.)) * glm::rotate(glm::mat4(1.), glm::radians(float(-90)), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1.), glm::vec3(0.05));

	shader.use();
	shader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	shader.setMatrix4f("view", false, glm::value_ptr(view));
	shader.setMatrix4f("projection", false, glm::value_ptr(projection));
	shader.set3Float("cameraPosition", camera.position.x, camera.position.y, camera.position.z);

	for (int i = 0; i < 4; ++i)
	{
		std::string temp = "lightPosition[" + std::to_string(i) + "]";
		shader.set3Float(temp.c_str(), lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
		temp = "lightColor[" + std::to_string(i) + "]";
		shader.set3Float(temp.c_str(), lightColors[i].x, lightColors[i].y, lightColors[i].z);
	}
	
	glActiveTexture(GL_TEXTURE0 + textureSlotAlbedo);
	glBindTexture(GL_TEXTURE_2D, textureIdAlbedo);
	glActiveTexture(GL_TEXTURE0 + textureSlotNormal);
	glBindTexture(GL_TEXTURE_2D, textureIdNormal);
	glActiveTexture(GL_TEXTURE0 + textureSlotMetalness);
	glBindTexture(GL_TEXTURE_2D, textureIdMetalness);
	glActiveTexture(GL_TEXTURE0 + textureSlotRoughness);
	glBindTexture(GL_TEXTURE_2D, textureIdRoughness);
	glActiveTexture(GL_TEXTURE0 + textureSlotAO);
	glBindTexture(GL_TEXTURE_2D, textureIdAO);
	glActiveTexture(GL_TEXTURE0 + textureSlotBRDFMap);
	glBindTexture(GL_TEXTURE_2D, textureIdBRDFMap);
	glActiveTexture(GL_TEXTURE0 + textureSlotIrradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIdIrradianceMap);
	glActiveTexture(GL_TEXTURE0 + textureSlotPrefilteredMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIdPrefilteredMap);
	glActiveTexture(GL_TEXTURE0 + textureSlotEnvMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureIdEnvMap);

	shader.set1Int("tex_albedo", textureSlotAlbedo);
	shader.set1Int("tex_normal", textureSlotNormal);
	shader.set1Int("tex_metalness", textureSlotMetalness);
	shader.set1Int("tex_roughness", textureSlotRoughness);
	shader.set1Int("tex_ao", textureSlotAO);
	shader.set1Int("tex_brdf", textureSlotBRDFMap);
	shader.set1Int("tex_irradiance", textureSlotIrradianceMap);
	shader.set1Int("tex_specular", textureSlotPrefilteredMap);
	model.draw(shader);

	modelMat = glm::mat4(1.0);
	skyboxShader.use();
	skyboxShader.setMatrix4f("view", false, glm::value_ptr(view));
	skyboxShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	skyboxShader.set1Int("tex_skybox", textureSlotEnvMap);
	glBindVertexArray(cube_vao);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);

	glfwSwapBuffers(WINDOW);
	glfwPollEvents();
}

void PBR::handleInput(float deltaTime)
{
	processInput(WINDOW, *INPUT);

	if (INPUT->keyboard[Button::ESCAPE].isPressed)
		glfwSetWindowShouldClose(WINDOW, 1);

	if (INPUT->keyboard[Button::W].isPressed)
	{
		camera.processMovement(Camera_Movement::MOVE_FORWARD, deltaTime);
	}
	if (INPUT->keyboard[Button::S].isPressed)
	{
		camera.processMovement(Camera_Movement::MOVE_BACKWARD, deltaTime);
	}
	if (INPUT->keyboard[Button::D].isPressed)
	{
		camera.processMovement(Camera_Movement::MOVE_RIGHT, deltaTime);
	}
	if (INPUT->keyboard[Button::A].isPressed)
	{
		camera.processMovement(Camera_Movement::MOVE_LEFT, deltaTime);
	}

	camera.processMouseMovement(INPUT->distanceTravelledX, -INPUT->distanceTravelledY);
}