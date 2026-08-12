#include "deferred_shading.h"

DeferredShading::DeferredShading(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window)
	: SCREEN_WIDTH(scrWidth), SCREEN_HEIGHT(scrHeight), FRAMEBUFFER_WIDTH(fbWidth), FRAMEBUFFER_HEIGHT(fbHeight),
	INPUT(&input), WINDOW(window), model(std::string(RESOURCES_PATH) + "models/backpack/backpack.obj")
{
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
	std::string vsPath = s + "DeferredShading/shaders/ligthPass_vs.shader";
	std::string fsPath = s + "DeferredShading/shaders/ligthPass_fs.shader";
	lightPassShader.createProgram(vsPath, fsPath, e);

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

void DeferredShading::run(float deltaTime)
{
	handleInput(deltaTime);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lightPassShader.use();
	lightPassShader.set1Float("shininessCoeffecient", 32.);
	lightPassShader.set2Float("resolution", FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	lightPassShader.set3Float("cameraPosition", camera.position.x, camera.position.y, camera.position.z);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorAttachments[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorAttachments[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, colorAttachments[2]);
	lightPassShader.set1Int("texture_position", 0);
	lightPassShader.set1Int("texture_normal", 1);
	lightPassShader.set1Int("texture_color", 2);

	lightPassShader.set3Float("light[0].position", l[0].position.x, l[0].position.y, l[0].position.z);
	lightPassShader.set3Float("light[0].ambient", l[0].ambient.x, l[0].ambient.y, l[0].ambient.z);
	lightPassShader.set3Float("light[0].diffuse", l[0].diffuse.x, l[0].diffuse.y, l[0].diffuse.z);
	lightPassShader.set3Float("light[0].specular", l[0].specular.x, l[0].specular.y, l[0].specular.z);
	lightPassShader.set1Float("light[0].constant", l[0].constant);
	lightPassShader.set1Float("light[0].linear", l[0].linear);
	lightPassShader.set1Float("light[0].quadratic", l[0].quadratic);

	lightPassShader.set3Float("light[1].position", l[1].position.x, l[1].position.y, l[1].position.z);
	lightPassShader.set3Float("light[1].ambient", l[1].ambient.x, l[1].ambient.y, l[1].ambient.z);
	lightPassShader.set3Float("light[1].diffuse", l[1].diffuse.x, l[1].diffuse.y, l[1].diffuse.z);
	lightPassShader.set3Float("light[1].specular", l[1].specular.x, l[1].specular.y, l[1].specular.z);
	lightPassShader.set1Float("light[1].constant", l[1].constant);
	lightPassShader.set1Float("light[1].linear", l[1].linear);
	lightPassShader.set1Float("light[1].quadratic", l[1].quadratic);

	lightPassShader.set3Float("light[2].position", l[2].position.x, l[2].position.y, l[2].position.z);
	lightPassShader.set3Float("light[2].ambient", l[2].ambient.x, l[2].ambient.y, l[2].ambient.z);
	lightPassShader.set3Float("light[2].diffuse", l[2].diffuse.x, l[2].diffuse.y, l[2].diffuse.z);
	lightPassShader.set3Float("light[2].specular", l[2].specular.x, l[2].specular.y, l[2].specular.z);
	lightPassShader.set1Float("light[2].constant", l[2].constant);
	lightPassShader.set1Float("light[2].linear", l[2].linear);
	lightPassShader.set1Float("light[2].quadratic", l[2].quadratic);

	lightPassShader.set3Float("light[3].position", l[3].position.x, l[3].position.y, l[3].position.z);
	lightPassShader.set3Float("light[3].ambient", l[3].ambient.x, l[3].ambient.y, l[3].ambient.z);
	lightPassShader.set3Float("light[3].diffuse", l[3].diffuse.x, l[3].diffuse.y, l[3].diffuse.z);
	lightPassShader.set3Float("light[3].specular", l[3].specular.x, l[3].specular.y, l[3].specular.z);
	lightPassShader.set1Float("light[3].constant", l[3].constant);
	lightPassShader.set1Float("light[3].linear", l[3].linear);
	lightPassShader.set1Float("light[3].quadratic", l[3].quadratic);
	
	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

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
	modelMat = glm::mat4(1.);
	view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(camera.zoom), SCREEN_WIDTH * 1.F / SCREEN_HEIGHT, .1F, 100.F);

	lightShader.use();
	lightShader.setMatrix4f("view", false, glm::value_ptr(view));
	lightShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	glBindVertexArray(vao);
	for (int i = 0; i < 4; ++i)
	{
		modelMat = glm::translate(glm::mat4(1.), l[i].position);
		lightShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
		lightShader.set3Float("lightColor", l[i].diffuse.x, l[i].diffuse.y, l[i].diffuse.z);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	shader.use();
	shader.setMatrix4f("view", false, glm::value_ptr(view));
	shader.setMatrix4f("projection", false, glm::value_ptr(projection));
	shader.set1Float("material.shininessCoeffecient", 32);
	shader.set1Int("material.texture_specular1", textureSlotSpec);
	shader.set3Float("cameraPosition", camera.position.x, camera.position.y, camera.position.z);

	shader.set3Float("light[0].position", l[0].position.x, l[0].position.y, l[0].position.z);
	shader.set3Float("light[0].ambient", l[0].ambient.x, l[0].ambient.y, l[0].ambient.z);
	shader.set3Float("light[0].diffuse", l[0].diffuse.x, l[0].diffuse.y, l[0].diffuse.z);
	shader.set3Float("light[0].specular", l[0].specular.x, l[0].specular.y, l[0].specular.z);
	shader.set1Float("light[0].constant", l[0].constant);
	shader.set1Float("light[0].linear", l[0].linear);
	shader.set1Float("light[0].quadratic", l[0].quadratic);

	shader.set3Float("light[1].position", l[1].position.x, l[1].position.y, l[1].position.z);
	shader.set3Float("light[1].ambient", l[1].ambient.x, l[1].ambient.y, l[1].ambient.z);
	shader.set3Float("light[1].diffuse", l[1].diffuse.x, l[1].diffuse.y, l[1].diffuse.z);
	shader.set3Float("light[1].specular", l[1].specular.x, l[1].specular.y, l[1].specular.z);
	shader.set1Float("light[1].constant", l[1].constant);
	shader.set1Float("light[1].linear", l[1].linear);
	shader.set1Float("light[1].quadratic", l[1].quadratic);

	shader.set3Float("light[2].position", l[2].position.x, l[2].position.y, l[2].position.z);
	shader.set3Float("light[2].ambient", l[2].ambient.x, l[2].ambient.y, l[2].ambient.z);
	shader.set3Float("light[2].diffuse", l[2].diffuse.x, l[2].diffuse.y, l[2].diffuse.z);
	shader.set3Float("light[2].specular", l[2].specular.x, l[2].specular.y, l[2].specular.z);
	shader.set1Float("light[2].constant", l[2].constant);
	shader.set1Float("light[2].linear", l[2].linear);
	shader.set1Float("light[2].quadratic", l[2].quadratic);

	shader.set3Float("light[3].position", l[3].position.x, l[3].position.y, l[3].position.z);
	shader.set3Float("light[3].ambient", l[3].ambient.x, l[3].ambient.y, l[3].ambient.z);
	shader.set3Float("light[3].diffuse", l[3].diffuse.x, l[3].diffuse.y, l[3].diffuse.z);
	shader.set3Float("light[3].specular", l[3].specular.x, l[3].specular.y, l[3].specular.z);
	shader.set1Float("light[3].constant", l[3].constant);
	shader.set1Float("light[3].linear", l[3].linear);
	shader.set1Float("light[3].quadratic", l[3].quadratic);

	glm::mat4 rotation = glm::rotate(glm::mat4(1.), glm::radians(float(-90.)), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1.), glm::radians(float(180.)), glm::vec3(0, 1, 0));

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(-4., -2., -9.));
	shader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	model.draw(shader);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(3., -2., -9.));
	shader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	model.draw(shader);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(0., -2., -10.));
	shader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	model.draw(shader);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(0., 2., -16.));
	shader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	model.draw(shader);
*/