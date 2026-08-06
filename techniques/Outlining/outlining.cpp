#include "outlining.h"

Outlining::Outlining(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window)
	: SCREEN_WIDTH(scrWidth), SCREEN_HEIGHT(scrHeight), FRAMEBUFFER_WIDTH(fbWidth), FRAMEBUFFER_HEIGHT(fbHeight),
	INPUT(&input), WINDOW(window)
{
	std::string e;
	std::string s = std::string(TECHNIQUES_PATH);
	std::string vsPath = s + "Outlining/shaders/cube_vs.shader";
	std::string fsPath = s + "Outlining/shaders/cube_fs.shader";
	shader.createProgram(vsPath, fsPath, e);

	vsPath = s + "Outlining/shaders/cubeBorder_vs.shader";
	fsPath = s + "Outlining/shaders/cubeBorder_fs.shader";
	borderShader.createProgram(vsPath, fsPath, e);
	
	timeToUpdate = 0.1;
	timer = timeToUpdate;
	model = glm::mat4(1.0);

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetFramebufferSize(WINDOW, &FRAMEBUFFER_WIDTH, &FRAMEBUFFER_HEIGHT);
	glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	glClearColor(0.15, 0.25, 0.4, 1.);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	textureSlot = 0;
	std::string texturePath = RESOURCES_PATH + std::string("textures/container2.png");
	textureId = loadTexture2D(texturePath.c_str(), textureSlot, true);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	shader.use();
}

void Outlining::run(float deltaTime)
{
	handleInput(deltaTime);

	model = glm::translate(glm::mat4(1.), glm::vec3(10., -1., -5.));
	view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(camera.zoom), SCREEN_WIDTH * 1.F / SCREEN_HEIGHT, 1.F, 100.F);

	shader.use();
	shader.setMatrix4f("model", false, glm::value_ptr(model));
	shader.setMatrix4f("view", false, glm::value_ptr(view));
	shader.setMatrix4f("projection", false, glm::value_ptr(projection));
	shader.set1Int("tex", textureSlot);

	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model2 = glm::translate(glm::mat4(1.), glm::vec3(11.5, -1.5, -5.));
	shader.setMatrix4f("model", false, glm::value_ptr(model2));
	glStencilFunc(GL_ALWAYS, 2, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glm::mat4 temp = glm::scale(glm::mat4(1.), glm::vec3(1.1));
	model = model * temp;

	borderShader.use();
	borderShader.setMatrix4f("view", false, glm::value_ptr(view));
	borderShader.setMatrix4f("projection", false, glm::value_ptr(projection));

	glStencilMask(0x00);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	borderShader.set3Float("borderColor", 1., 1., 1.);
	borderShader.setMatrix4f("model", false, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glStencilFunc(GL_NOTEQUAL, 2, 0xFF);
	model2 = model2 * temp;
	borderShader.set3Float("borderColor", 1., 0., 0.);
	borderShader.setMatrix4f("model", false, glm::value_ptr(model2));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glfwSwapBuffers(WINDOW);
	glfwPollEvents();
}

void Outlining::handleInput(float deltaTime)
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