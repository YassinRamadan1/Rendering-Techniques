#include "shadow_mapping.h"

ShadowMapping::ShadowMapping(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window)
	: SCREEN_WIDTH(scrWidth), SCREEN_HEIGHT(scrHeight), FRAMEBUFFER_WIDTH(fbWidth), FRAMEBUFFER_HEIGHT(fbHeight),
	INPUT(&input), WINDOW(window), model(std::string(RESOURCES_PATH) + "models/backpack/backpack.obj")
{
	dirL[0].direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	dirL[0].ambient = glm::vec3(0.02f, 0.02f, 0.04f);
	dirL[0].diffuse = glm::vec3(.15, 0.15, 0.15);
	dirL[0].specular = glm::vec3(0.20f, 0.20f, 0.30f);

	dirL[1].direction = glm::vec3(1.f, 0.5f, 0.5f);
	dirL[1].ambient = glm::vec3(0.02f, 0.01f, 0.0f);
	dirL[1].diffuse = glm::vec3(.15, 0.1, 0.05);
	dirL[1].specular = glm::vec3(0.20f, 0.15f, 0.10f);

	for (int i = 0; i < 4; ++i)
	{
		l[i].specular = glm::vec3(0.5f);
		l[i].constant = 1.0f;
		l[i].linear = 0.007f;
		l[i].quadratic = 0.0002f;
	}

	l[0].position = glm::vec3(5.0f, 2.0f, 1.0f);
	l[0].ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	l[0].diffuse = glm::vec3(1.f, 1.f, 1.f);

	l[1].position = glm::vec3(-5.0f, 0.0f, -8.0f);
	l[1].ambient = glm::vec3(0.0f, 0.0f, 0.02f);
	l[1].diffuse = glm::vec3(0.0f, 0.0f, 1.0f);

	l[2].position = glm::vec3(2.0f, 1.0f, -10.0f);
	l[2].ambient = glm::vec3(0.0f, 0.02f, 0.0f);
	l[2].diffuse = glm::vec3(0.0f, 1.0f, 0.0f);

	l[3].position = glm::vec3(3.0f, 0.0f, -8.0f);
	l[3].ambient = glm::vec3(0.02f, 0.0f, 0.0f);
	l[3].diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	
	std::string texturePathPlane = RESOURCES_PATH + std::string("textures/plane.png");
	std::string texturePathCube = RESOURCES_PATH + std::string("textures/container2.png");
	std::string texturePathCubeSpec = RESOURCES_PATH + std::string("textures/container2_specular.png");
	std::string texturePathBackPackSpec = std::string(RESOURCES_PATH) + "models/backpack/specular.jpg";
	
	textureSlotPlane = 5;
	textureSlotCube = 6;
	textureSlotCubeSpec = 7;
	textureSlotBackPackSpec = 8;

	textureIdPlane = loadTexture2D(texturePathPlane.c_str(), textureSlotPlane, true);
	textureIdCube = loadTexture2D(texturePathCube.c_str(), textureSlotCube, true);
	textureIdCubeSpec = loadTexture2D(texturePathCubeSpec.c_str(), textureSlotCubeSpec, true);
	textureIdBackPackSpec = loadTexture2D(texturePathBackPackSpec.c_str(), textureSlotBackPackSpec, false);
	
	std::string e;
	std::string s = std::string(TECHNIQUES_PATH);
	std::string vsPath = s + "ShadowMapping/shaders/plane_vs.shader";
	std::string fsPath = s + "ShadowMapping/shaders/plane_fs.shader";
	planeShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "ShadowMapping/shaders/plane_vs.shader";
	fsPath = s + "ShadowMapping/shaders/empty_fs.shader";
	emptyPlaneShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "ShadowMapping/shaders/plane_vs.shader";
	fsPath = s + "ShadowMapping/shaders/depth_fs.shader";
	depthPlaneShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "ShadowMapping/shaders/cube_vs.shader";
	fsPath = s + "ShadowMapping/shaders/cube_fs.shader";
	cubeShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "ShadowMapping/shaders/cube_vs.shader";
	fsPath = s + "ShadowMapping/shaders/empty_fs.shader";
	emptyCubeShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "ShadowMapping/shaders/cube_vs.shader";
	fsPath = s + "ShadowMapping/shaders/depth_fs.shader";
	depthCubeShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "ShadowMapping/shaders/backPack_vs.shader";
	fsPath = s + "ShadowMapping/shaders/backPack_fs.shader";
	modelShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "ShadowMapping/shaders/backPack_vs.shader";
	fsPath = s + "ShadowMapping/shaders/empty_fs.shader";
	emptyModelShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "ShadowMapping/shaders/backPack_vs.shader";
	fsPath = s + "ShadowMapping/shaders/backPackDepth_fs.shader";
	depthModelShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "ShadowMapping/shaders/light_vs.shader";
	fsPath = s + "ShadowMapping/shaders/light_fs.shader";
	lightShader.createProgram(vsPath, fsPath, e);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetFramebufferSize(WINDOW, &FRAMEBUFFER_WIDTH, &FRAMEBUFFER_HEIGHT);
	glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

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

	float planeVertices[] =
	{
		-0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		 0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		-0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
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

	glGenVertexArrays(1, &plane_vao);
	glBindVertexArray(plane_vao);
	glGenBuffers(1, &plane_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &quad_vao);
	glBindVertexArray(quad_vao);

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);


	glClearColor(0, 0, 0, 1.);
	glEnable(GL_DEPTH_TEST);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	for (int i = 0; i < 2; ++i)
	{
		glGenTextures(1, &depthAttachment[i]);
		glBindTexture(GL_TEXTURE_2D, depthAttachment[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	dirLightProjection = glm::ortho(-15.f, 15.f, -15.f, 15.f, 1.f, 150.f);
	glm::vec3 sceneCenter(0, 0, -4);
	for (int i = 0; i < 2; ++i)
	{
		dirLightView[i] = glm::lookAt(sceneCenter - 60.f * dirL[i].direction, sceneCenter, glm::vec3(0, 1, 0));
		dirLightTransform[i] = dirLightProjection * dirLightView[i];
	}

	cubeFaceDimension = 1024;
	glGenTextures(1, &pointLightCubeTex);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pointLightCubeTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, cubeFaceDimension, cubeFaceDimension, 0, GL_RED, GL_FLOAT, nullptr);
	}

	glGenFramebuffers(1, &pointLightFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFbo);
	glGenTextures(1, &pointLightDepthAttachment);
	glBindTexture(GL_TEXTURE_2D, pointLightDepthAttachment);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, cubeFaceDimension, cubeFaceDimension, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pointLightDepthAttachment, 0);

	pointLightProjection = glm::perspective(glm::radians(float(90)), 1.F, 0.1F, 50.F);
	
	pointLightView[0][0] = glm::lookAt(l[0].position, l[0].position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	pointLightView[0][1] = glm::lookAt(l[0].position, l[0].position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	pointLightView[0][2] = glm::lookAt(l[0].position, l[0].position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	pointLightView[0][3] = glm::lookAt(l[0].position, l[0].position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
	pointLightView[0][4] = glm::lookAt(l[0].position, l[0].position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
	pointLightView[0][5] = glm::lookAt(l[0].position, l[0].position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));
}

void ShadowMapping::renderSceneDirLightView(int i)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, depthAttachment[i]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment[i], 0);
	glClear(GL_DEPTH_BUFFER_BIT);

	modelMat = glm::mat4(1.);
	view = dirLightView[i];
	projection = dirLightProjection;

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(0, -1.8, -3)) * glm::scale(glm::mat4(1.), glm::vec3(15.));

	emptyPlaneShader.use();
	emptyPlaneShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	emptyPlaneShader.setMatrix4f("view", false, glm::value_ptr(view));
	emptyPlaneShader.setMatrix4f("projection", false, glm::value_ptr(projection));

	glBindVertexArray(plane_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	modelMat = glm::mat4(1.);
	modelMat = glm::translate(glm::mat4(1.), glm::vec3(0, -1.25, -5));

	emptyCubeShader.use();
	emptyCubeShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	emptyCubeShader.setMatrix4f("view", false, glm::value_ptr(view));
	emptyCubeShader.setMatrix4f("projection", false, glm::value_ptr(projection));

	glBindVertexArray(cube_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(2, -1.25, -3));
	emptyCubeShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelMat = glm::mat4(1.);
	modelMat = glm::translate(glm::mat4(1.), glm::vec3(-2, 0, -7));

	emptyModelShader.use();
	emptyModelShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	emptyModelShader.setMatrix4f("view", false, glm::value_ptr(view));
	emptyModelShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	model.draw(emptyModelShader);
}

void ShadowMapping::renderScenePointLightView()
{
	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFbo);
	for (int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, pointLightCubeTex, 0);
		glViewport(0, 0, cubeFaceDimension, cubeFaceDimension);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelMat = glm::mat4(1.);
		view = pointLightView[0][i];
		projection = pointLightProjection;

		modelMat = glm::translate(glm::mat4(1.), glm::vec3(0, -1.8, -3)) * glm::scale(glm::mat4(1.), glm::vec3(15.));

		depthPlaneShader.use();
		depthPlaneShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
		depthPlaneShader.setMatrix4f("view", false, glm::value_ptr(view));
		depthPlaneShader.setMatrix4f("projection", false, glm::value_ptr(projection));
		depthPlaneShader.set3Float("lightPosition", l[0].position.x, l[0].position.y, l[0].position.z);

		glBindVertexArray(plane_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		modelMat = glm::mat4(1.);
		modelMat = glm::translate(glm::mat4(1.), glm::vec3(0, -1.25, -5));

		depthCubeShader.use();
		depthCubeShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
		depthCubeShader.setMatrix4f("view", false, glm::value_ptr(view));
		depthCubeShader.setMatrix4f("projection", false, glm::value_ptr(projection));
		depthCubeShader.set3Float("lightPosition", l[0].position.x, l[0].position.y, l[0].position.z);
		
		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelMat = glm::translate(glm::mat4(1.), glm::vec3(2, -1.25, -3));
		depthCubeShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelMat = glm::mat4(1.);
		modelMat = glm::translate(glm::mat4(1.), glm::vec3(-2, 0, -7));

		depthModelShader.use();
		depthModelShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
		depthModelShader.setMatrix4f("view", false, glm::value_ptr(view));
		depthModelShader.setMatrix4f("projection", false, glm::value_ptr(projection));
		depthModelShader.set3Float("lightPosition", l[0].position.x, l[0].position.y, l[0].position.z); 
		
		model.draw(depthModelShader);
	}

	glClearColor(0, 0, 0, 1.);
	glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
}

void ShadowMapping::renderSceneCameraView()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelMat = glm::mat4(1.);
	view = camera.getViewMatrix();
	projection = glm::perspective(camera.zoom, SCREEN_WIDTH * 1.F / SCREEN_HEIGHT, 0.1F, 100.F);


	modelMat = glm::translate(glm::mat4(1.0), l[0].position);
	lightShader.use();
	lightShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	lightShader.setMatrix4f("view", false, glm::value_ptr(view));
	lightShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	lightShader.set3Float("lightColor", l[0].diffuse.x, l[0].diffuse.y, l[0].diffuse.z);
	glBindVertexArray(cube_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(0, -1.8, -3)) * glm::scale(glm::mat4(1.), glm::vec3(15.));
	glActiveTexture(GL_TEXTURE0 + textureSlotPlane);
	glBindTexture(GL_TEXTURE_2D, textureIdPlane);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, depthAttachment[0]);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, depthAttachment[1]);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pointLightCubeTex);

	planeShader.use();
	planeShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	planeShader.setMatrix4f("view", false, glm::value_ptr(view));
	planeShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	planeShader.set3Float("cameraPosition", camera.position.x, camera.position.y, camera.position.z);
	planeShader.setMatrix4f("dirLightTransform[0]", false, glm::value_ptr(dirLightTransform[0]));
	planeShader.setMatrix4f("dirLightTransform[1]", false, glm::value_ptr(dirLightTransform[1]));
	planeShader.set1Int("texture_depth[0]", 10);
	planeShader.set1Int("texture_depth[1]", 11);
	planeShader.set1Int("texture_pointLightDepth", 12);

	planeShader.set3Float("dirLight[0].direction", dirL[0].direction.x, dirL[0].direction.y, dirL[0].direction.z);
	planeShader.set3Float("dirLight[0].ambient", dirL[0].ambient.x, dirL[0].ambient.y, dirL[0].ambient.z);
	planeShader.set3Float("dirLight[0].diffuse", dirL[0].diffuse.x, dirL[0].diffuse.y, dirL[0].diffuse.z);
	planeShader.set3Float("dirLight[0].specular", dirL[0].specular.x, dirL[0].specular.y, dirL[0].specular.z);

	planeShader.set3Float("dirLight[1].direction", dirL[1].direction.x, dirL[1].direction.y, dirL[1].direction.z);
	planeShader.set3Float("dirLight[1].ambient", dirL[1].ambient.x, dirL[1].ambient.y, dirL[1].ambient.z);
	planeShader.set3Float("dirLight[1].diffuse", dirL[1].diffuse.x, dirL[1].diffuse.y, dirL[1].diffuse.z);
	planeShader.set3Float("dirLight[1].specular", dirL[1].specular.x, dirL[1].specular.y, dirL[1].specular.z);

	planeShader.set3Float("light[0].position", l[0].position.x, l[0].position.y, l[0].position.z);
	planeShader.set3Float("light[0].ambient", l[0].ambient.x, l[0].ambient.y, l[0].ambient.z);
	planeShader.set3Float("light[0].diffuse", l[0].diffuse.x, l[0].diffuse.y, l[0].diffuse.z);
	planeShader.set3Float("light[0].specular", l[0].specular.x, l[0].specular.y, l[0].specular.z);
	planeShader.set1Float("light[0].constant", l[0].constant);
	planeShader.set1Float("light[0].linear", l[0].linear);
	planeShader.set1Float("light[0].quadratic", l[0].quadratic);

	planeShader.set1Int("texture_diffuse", textureSlotPlane);
	planeShader.set1Float("shininessCoeffecient", 32.);

	glBindVertexArray(plane_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	modelMat = glm::mat4(1.);
	modelMat = glm::translate(glm::mat4(1.), glm::vec3(0, -1.25, -5));
	glActiveTexture(GL_TEXTURE0 + textureSlotCube);
	glBindTexture(GL_TEXTURE_2D, textureIdCube);
	glActiveTexture(GL_TEXTURE0 + textureSlotCubeSpec);
	glBindTexture(GL_TEXTURE_2D, textureIdCubeSpec);

	cubeShader.use();
	cubeShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	cubeShader.setMatrix4f("view", false, glm::value_ptr(view));
	cubeShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	cubeShader.set3Float("cameraPosition", camera.position.x, camera.position.y, camera.position.z);
	cubeShader.setMatrix4f("dirLightTransform[0]", false, glm::value_ptr(dirLightTransform[0]));
	cubeShader.setMatrix4f("dirLightTransform[1]", false, glm::value_ptr(dirLightTransform[1]));
	cubeShader.set1Int("texture_depth[0]", 10);
	cubeShader.set1Int("texture_depth[1]", 11);
	cubeShader.set1Int("texture_pointLightDepth", 12);

	cubeShader.set3Float("dirLight[0].direction", dirL[0].direction.x, dirL[0].direction.y, dirL[0].direction.z);
	cubeShader.set3Float("dirLight[0].ambient", dirL[0].ambient.x, dirL[0].ambient.y, dirL[0].ambient.z);
	cubeShader.set3Float("dirLight[0].diffuse", dirL[0].diffuse.x, dirL[0].diffuse.y, dirL[0].diffuse.z);
	cubeShader.set3Float("dirLight[0].specular", dirL[0].specular.x, dirL[0].specular.y, dirL[0].specular.z);

	cubeShader.set3Float("dirLight[1].direction", dirL[1].direction.x, dirL[1].direction.y, dirL[1].direction.z);
	cubeShader.set3Float("dirLight[1].ambient", dirL[1].ambient.x, dirL[1].ambient.y, dirL[1].ambient.z);
	cubeShader.set3Float("dirLight[1].diffuse", dirL[1].diffuse.x, dirL[1].diffuse.y, dirL[1].diffuse.z);
	cubeShader.set3Float("dirLight[1].specular", dirL[1].specular.x, dirL[1].specular.y, dirL[1].specular.z);

	cubeShader.set3Float("light[0].position", l[0].position.x, l[0].position.y, l[0].position.z);
	cubeShader.set3Float("light[0].ambient", l[0].ambient.x, l[0].ambient.y, l[0].ambient.z);
	cubeShader.set3Float("light[0].diffuse", l[0].diffuse.x, l[0].diffuse.y, l[0].diffuse.z);
	cubeShader.set3Float("light[0].specular", l[0].specular.x, l[0].specular.y, l[0].specular.z);
	cubeShader.set1Float("light[0].constant", l[0].constant);
	cubeShader.set1Float("light[0].linear", l[0].linear);
	cubeShader.set1Float("light[0].quadratic", l[0].quadratic);

	cubeShader.set1Int("texture_diffuse", textureSlotCube);
	cubeShader.set1Int("texture_spec", textureSlotCubeSpec);
	cubeShader.set1Float("shininessCoeffecient", 32.);

	glBindVertexArray(cube_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(2, -1.25, -3));
	cubeShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelMat = glm::mat4(1.);
	modelMat = glm::translate(glm::mat4(1.), glm::vec3(-2, 0, -7));

	glActiveTexture(GL_TEXTURE0 + textureSlotBackPackSpec);
	glBindTexture(GL_TEXTURE_2D, textureIdBackPackSpec);

	modelShader.use();
	modelShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	modelShader.setMatrix4f("view", false, glm::value_ptr(view));
	modelShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	modelShader.set3Float("cameraPosition", camera.position.x, camera.position.y, camera.position.z);
	modelShader.setMatrix4f("dirLightTransform[0]", false, glm::value_ptr(dirLightTransform[0]));
	modelShader.setMatrix4f("dirLightTransform[1]", false, glm::value_ptr(dirLightTransform[1]));
	modelShader.set1Int("texture_depth[0]", 10);
	modelShader.set1Int("texture_depth[1]", 11);
	modelShader.set1Int("texture_pointLightDepth", 12);

	modelShader.set3Float("dirLight[0].direction", dirL[0].direction.x, dirL[0].direction.y, dirL[0].direction.z);
	modelShader.set3Float("dirLight[0].ambient", dirL[0].ambient.x, dirL[0].ambient.y, dirL[0].ambient.z);
	modelShader.set3Float("dirLight[0].diffuse", dirL[0].diffuse.x, dirL[0].diffuse.y, dirL[0].diffuse.z);
	modelShader.set3Float("dirLight[0].specular", dirL[0].specular.x, dirL[0].specular.y, dirL[0].specular.z);

	modelShader.set3Float("dirLight[1].direction", dirL[1].direction.x, dirL[1].direction.y, dirL[1].direction.z);
	modelShader.set3Float("dirLight[1].ambient", dirL[1].ambient.x, dirL[1].ambient.y, dirL[1].ambient.z);
	modelShader.set3Float("dirLight[1].diffuse", dirL[1].diffuse.x, dirL[1].diffuse.y, dirL[1].diffuse.z);
	modelShader.set3Float("dirLight[1].specular", dirL[1].specular.x, dirL[1].specular.y, dirL[1].specular.z);

	modelShader.set3Float("light[0].position", l[0].position.x, l[0].position.y, l[0].position.z);
	modelShader.set3Float("light[0].ambient", l[0].ambient.x, l[0].ambient.y, l[0].ambient.z);
	modelShader.set3Float("light[0].diffuse", l[0].diffuse.x, l[0].diffuse.y, l[0].diffuse.z);
	modelShader.set3Float("light[0].specular", l[0].specular.x, l[0].specular.y, l[0].specular.z);
	modelShader.set1Float("light[0].constant", l[0].constant);
	modelShader.set1Float("light[0].linear", l[0].linear);
	modelShader.set1Float("light[0].quadratic", l[0].quadratic);

	modelShader.set1Int("material.texture_specular1", textureSlotBackPackSpec);
	modelShader.set1Float("material.shininessCoeffecient", 32.);
	model.draw(modelShader);
}

void ShadowMapping::run(float deltaTime)
{
	handleInput(deltaTime);

	for (int i = 0; i < 2; ++i)
	{
		renderSceneDirLightView(i);
	}

	renderScenePointLightView();

	renderSceneCameraView();


	glfwSwapBuffers(WINDOW);
	glfwPollEvents();
}

void ShadowMapping::handleInput(float deltaTime)
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