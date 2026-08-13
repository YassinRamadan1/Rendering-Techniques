#include "deferred_shading.h"

DeferredShading::DeferredShading(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window)
	: SCREEN_WIDTH(scrWidth), SCREEN_HEIGHT(scrHeight), FRAMEBUFFER_WIDTH(fbWidth), FRAMEBUFFER_HEIGHT(fbHeight),
	INPUT(&input), WINDOW(window), model(std::string(RESOURCES_PATH) + "models/backpack/backpack.obj"), sphereModel(std::string(RESOURCES_PATH) + "models/sphere/sphere.glb")
{
	dirL[0].direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	dirL[0].ambient = glm::vec3(0.02f, 0.02f, 0.04f);
	dirL[0].diffuse = glm::vec3(0.15f, 0.15f, 0.25f);
	dirL[0].specular = glm::vec3(0.20f, 0.20f, 0.30f);

	dirL[1].direction = glm::vec3(0.5f, 0.5f, 1.0f);
	dirL[1].ambient = glm::vec3(0.02f, 0.01f, 0.0f);
	dirL[1].diffuse = glm::vec3(0.15f, 0.10f, 0.05f);
	dirL[1].specular = glm::vec3(0.20f, 0.15f, 0.10f);


	for (int i = 0; i < 4; ++i)
	{
		l[i].specular = glm::vec3(0.5f);
		l[i].constant = 1.0f;
		l[i].linear = 0.09f;
		l[i].quadratic = 0.032f;
	}

	l[0].position = glm::vec3(3.0f, 0.0f, -8.0f);
	l[0].ambient = glm::vec3(0.02f, 0.0f, 0.0f);
	l[0].diffuse = glm::vec3(1.0f, 0.0f, 0.0f);

	l[1].position = glm::vec3(-5.0f, 0.0f, -8.0f);
	l[1].ambient = glm::vec3(0.0f, 0.0f, 0.02f);
	l[1].diffuse = glm::vec3(0.0f, 0.0f, 1.0f);

	l[2].position = glm::vec3(2.0f, 1.0f, -10.0f);
	l[2].ambient = glm::vec3(0.0f, 0.02f, 0.0f);
	l[2].diffuse = glm::vec3(0.0f, 1.0f, 0.0f);

	l[3].position = glm::vec3(-2.0f, 0.0f, -5.0f);
	l[3].ambient = glm::vec3(0.04f, 0.04f, 0.04f);
	l[3].diffuse = glm::vec3(1.0f, 1.0f, 1.0f);

	std::string texSpec = std::string(RESOURCES_PATH) + "models/backpack/specular.jpg";
	textureSlotSpec = 10;
	textureIdSpec = loadTexture2D(texSpec.c_str(), textureSlotSpec, false);

	std::string e;
	std::string s = std::string(TECHNIQUES_PATH);
	std::string vsPath = s + "DeferredShading/shaders/lightPassQuad_vs.shader";
	std::string fsPath = s + "DeferredShading/shaders/lightPassQuad_fs.shader";
	lightPassQuadShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "DeferredShading/shaders/lightPassSphere_vs.shader";
	fsPath = s + "DeferredShading/shaders/empty_fs.shader";
	emptyShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "DeferredShading/shaders/lightPassSphere_vs.shader";
	fsPath = s + "DeferredShading/shaders/lightPassSphere_fs.shader";
	lightPassSphereShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "DeferredShading/shaders/lightCube_vs.shader";
	fsPath = s + "DeferredShading/shaders/lightCube_fs.shader";
	lightCubeShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "DeferredShading/shaders/geometryPass_vs.shader";
	fsPath = s + "DeferredShading/shaders/geometryPass_fs.shader";
	geometryPassShader.createProgram(vsPath, fsPath, e);

	float vertices[] =
	{
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetFramebufferSize(WINDOW, &FRAMEBUFFER_WIDTH, &FRAMEBUFFER_HEIGHT);
	glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	glClearColor(0, 0, 0, 1.);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	for (int i = 0; i < 2; ++i)
	{
		glGenTextures(1, &colorAttachments[i]);
		glBindTexture(GL_TEXTURE_2D, colorAttachments[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorAttachments[i], 0);
	}
	glGenTextures(1, &colorAttachments[2]);
	glBindTexture(GL_TEXTURE_2D, colorAttachments[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorAttachments[2], 0);

	unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glGenTextures(1, &depthAttachment);
	glBindTexture(GL_TEXTURE_2D, depthAttachment);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
}

void DeferredShading::geometryPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelMat = glm::mat4(1.);
	view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(camera.zoom), SCREEN_WIDTH * 1.F / SCREEN_HEIGHT, .1F, 100.F);

	geometryPassShader.use();
	geometryPassShader.setMatrix4f("view", false, glm::value_ptr(view));
	geometryPassShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	geometryPassShader.set1Int("material.texture_specular1", textureSlotSpec);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(-4., -2., -9.));
	geometryPassShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	model.draw(geometryPassShader);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(3., -2., -9.));
	geometryPassShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	model.draw(geometryPassShader);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(0., -2., -10.));
	geometryPassShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	model.draw(geometryPassShader);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(0., 2., -16.));
	geometryPassShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	model.draw(geometryPassShader);
}

void DeferredShading::dirLightPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	lightPassQuadShader.use();
	lightPassQuadShader.set1Float("shininessCoeffecient", 32.);
	lightPassQuadShader.set2Float("resolution", FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	lightPassQuadShader.set3Float("cameraPosition", camera.position.x, camera.position.y, camera.position.z);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, colorAttachments[0]);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, colorAttachments[1]);
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, colorAttachments[2]);
	lightPassQuadShader.set1Int("texture_position", 11);
	lightPassQuadShader.set1Int("texture_normal", 12);
	lightPassQuadShader.set1Int("texture_color", 13);

	lightPassQuadShader.set3Float("light[0].direction", dirL[0].direction.x, dirL[0].direction.y, dirL[0].direction.z);
	lightPassQuadShader.set3Float("light[0].ambient", dirL[0].ambient.x, dirL[0].ambient.y, dirL[0].ambient.z);
	lightPassQuadShader.set3Float("light[0].diffuse", dirL[0].diffuse.x, dirL[0].diffuse.y, dirL[0].diffuse.z);
	lightPassQuadShader.set3Float("light[0].specular", dirL[0].specular.x, dirL[0].specular.y, dirL[0].specular.z);

	lightPassQuadShader.set3Float("light[1].direction", dirL[1].direction.x, dirL[1].direction.y, dirL[1].direction.z);
	lightPassQuadShader.set3Float("light[1].ambient", dirL[1].ambient.x, dirL[1].ambient.y, dirL[1].ambient.z);
	lightPassQuadShader.set3Float("light[1].diffuse", dirL[1].diffuse.x, dirL[1].diffuse.y, dirL[1].diffuse.z);
	lightPassQuadShader.set3Float("light[1].specular", dirL[1].specular.x, dirL[1].specular.y, dirL[1].specular.z);

	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DeferredShading::stencilPass(int i)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);
	glFrontFace(GL_CCW);
	glStencilFuncSeparate(GL_FRONT, GL_ALWAYS, 0, 0xFF);
	glStencilFuncSeparate(GL_BACK, GL_ALWAYS, 0, 0xFF);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP,GL_KEEP);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glDrawBuffer(GL_NONE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	emptyShader.use();
	emptyShader.setMatrix4f("view", false, glm::value_ptr(view));
	emptyShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	
	float lightMax = std::fmaxf(std::fmaxf(l[i].diffuse.r, l[i].diffuse.g), l[i].diffuse.b);
	float radius = (-l[i].linear + std::sqrtf(l[i].linear * l[i].linear - 4 * l[i].quadratic * (l[i].constant - (256.0 / 5.0) * lightMax))) / (2 * l[i].quadratic);

	modelMat = glm::translate(glm::mat4(1.), l[i].position) * glm::scale(glm::mat4(1.), glm::vec3(radius));
	emptyShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	sphereModel.draw(emptyShader);
}

void DeferredShading::pointLightPass(int i)
{
	glDrawBuffer(GL_BACK);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0x00);
	glStencilFuncSeparate(GL_FRONT, GL_NOTEQUAL, 0, 0xFF);
	glStencilFuncSeparate(GL_BACK, GL_NOTEQUAL, 0, 0xFF);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	lightPassSphereShader.use();
	lightPassSphereShader.setMatrix4f("view", false, glm::value_ptr(view));
	lightPassSphereShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	lightPassSphereShader.set1Float("shininessCoeffecient", 32.);
	lightPassSphereShader.set2Float("resolution", FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	lightPassSphereShader.set3Float("cameraPosition", camera.position.x, camera.position.y, camera.position.z);
	lightPassSphereShader.set1Int("material.texture_specular1", textureSlotSpec);
	lightPassSphereShader.set1Int("texture_position", 11);
	lightPassSphereShader.set1Int("texture_normal", 12);
	lightPassSphereShader.set1Int("texture_color", 13);

	float lightMax = std::fmaxf(std::fmaxf(l[i].diffuse.r, l[i].diffuse.g), l[i].diffuse.b);
	float radius = (-l[i].linear + std::sqrtf(l[i].linear * l[i].linear - 4 * l[i].quadratic * (l[i].constant - (256.0 / 5.0) * lightMax))) / (2 * l[i].quadratic);

	modelMat = glm::translate(glm::mat4(1.), l[i].position) * glm::scale(glm::mat4(1.), glm::vec3(radius));

	lightPassSphereShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	lightPassSphereShader.set3Float("light.position", l[i].position.x, l[i].position.y, l[i].position.z);
	lightPassSphereShader.set3Float("light.ambient", l[i].ambient.x, l[i].ambient.y, l[i].ambient.z);
	lightPassSphereShader.set3Float("light.diffuse", l[i].diffuse.x, l[i].diffuse.y, l[i].diffuse.z);
	lightPassSphereShader.set3Float("light.specular", l[i].specular.x, l[i].specular.y, l[i].specular.z);
	lightPassSphereShader.set1Float("light.constant", l[i].constant);
	lightPassSphereShader.set1Float("light.linear", l[i].linear);
	lightPassSphereShader.set1Float("light.quadratic", l[i].quadratic);
	sphereModel.draw(lightPassSphereShader);
}

void DeferredShading::forwardPass()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	lightCubeShader.use();
	lightCubeShader.setMatrix4f("view", false, glm::value_ptr(view));
	lightCubeShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	for (int i = 0; i < 4; ++i)
	{
		float lightMax = std::fmaxf(std::fmaxf(l[i].diffuse.r, l[i].diffuse.g), l[i].diffuse.b);
		float radius = (-l[i].linear + std::sqrtf(l[i].linear * l[i].linear - 4 * l[i].quadratic * (l[i].constant - (256.0 / 5.0) * lightMax))) / (2 * l[i].quadratic);

		modelMat = glm::translate(glm::mat4(1.), l[i].position);
		if (INPUT->keyboard[Button::NUM_0].isPressed)
		{
			modelMat = glm::translate(glm::mat4(1.), l[i].position) * glm::scale(glm::mat4(1.), glm::vec3(radius));
		}

		lightCubeShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
		lightCubeShader.set3Float("lightColor", l[i].diffuse.x, l[i].diffuse.y, l[i].diffuse.z);
		sphereModel.draw(lightCubeShader);
	}
}

void DeferredShading::run(float deltaTime)
{
	handleInput(deltaTime);

	geometryPass();

	dirLightPass();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	
	for (int i = 0; i < 4; ++i)
	{
		stencilPass(i);
		pointLightPass(i);
	}

	forwardPass();

	glfwSwapBuffers(WINDOW);
	glfwPollEvents();
}

void DeferredShading::handleInput(float deltaTime)
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

/*
lightCubeShader.use();
	lightCubeShader.setMatrix4f("view", false, glm::value_ptr(view));
	lightCubeShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	glBindVertexArray(vao);
	for (int i = 0; i < 4; ++i)
	{
		modelMat = glm::translate(glm::mat4(1.), l[i].position);
		lightCubeShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
		lightCubeShader.set3Float("lightColor", l[i].diffuse.x, l[i].diffuse.y, l[i].diffuse.z);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
*/