#include "normal_mapping.h"

NormalMapping::NormalMapping(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window)
	: SCREEN_WIDTH(scrWidth), SCREEN_HEIGHT(scrHeight), FRAMEBUFFER_WIDTH(fbWidth), FRAMEBUFFER_HEIGHT(fbHeight),
	INPUT(&input), WINDOW(window), model(std::string(RESOURCES_PATH) + "models/stonefloor/copplestone.gltf")
{
	std::string e;
	std::string s = std::string(TECHNIQUES_PATH);
	std::string vsPath = s + "NormalMapping/shaders/stonefloor_vs.shader";
	std::string fsPath = s + "NormalMapping/shaders/stonefloor_fs.shader";
	shader.createProgram(vsPath, fsPath, e);

	vsPath = s + "NormalMapping/shaders/stonefloorNormal_vs.shader";
	fsPath = s + "NormalMapping/shaders/stonefloorNormal_fs.shader";
	normalShader.createProgram(vsPath, fsPath, e);
	
	vsPath = s + "NormalMapping/shaders/lightCube_vs.shader";
	fsPath = s + "NormalMapping/shaders/lightCube_fs.shader";
	lightShader.createProgram(vsPath, fsPath, e);

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

	glClearColor(0.15, 0.25, 0.4, 1.);
	glEnable(GL_DEPTH_TEST);
}

void NormalMapping::run(float deltaTime)
{
	handleInput(deltaTime);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::vec3 lightPosition(0, 1., -3), lightColor(1., 1., 1.);
	modelMat = glm::translate(glm::mat4(1.), lightPosition);
	modelMat = modelMat * glm::scale(glm::mat4(1.), glm::vec3(1.));
	view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(camera.zoom), SCREEN_WIDTH * 1.F / SCREEN_HEIGHT, .1F, 100.F);

	// draw light cube
	lightShader.use();
	lightShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	lightShader.setMatrix4f("view", false, glm::value_ptr(view));
	lightShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	lightShader.set3Float("lightColor", lightColor.x, lightColor.y, lightColor.z);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glm::mat4 rotation = glm::rotate(glm::mat4(1.), glm::radians(float(-90.)), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1.), glm::radians(float(180.)), glm::vec3(0, 1, 0));
	modelMat = glm::translate(glm::mat4(1.), glm::vec3(1., -2., -3.));
	modelMat = modelMat * rotation;
	normalShader.use();
	normalShader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	normalShader.setMatrix4f("view", false, glm::value_ptr(view));
	normalShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	normalShader.set3Float("lightColor", lightColor.x, lightColor.y, lightColor.z);
	normalShader.set3Float("lightPosition", lightPosition.x, lightPosition.y, lightPosition.z);
	normalShader.set3Float("cameraPosition", camera.position.x, camera.position.y, camera.position.z);
	model.draw(normalShader);

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(-1., -2., -3.));
	modelMat = modelMat * rotation;
	shader.use();
	shader.setMatrix4f("model", false, glm::value_ptr(modelMat));
	shader.setMatrix4f("view", false, glm::value_ptr(view));
	shader.setMatrix4f("projection", false, glm::value_ptr(projection));
	shader.set3Float("lightColor", lightColor.x, lightColor.y, lightColor.z);
	shader.set3Float("lightPosition", lightPosition.x, lightPosition.y, lightPosition.z);
	shader.set3Float("cameraPosition", camera.position.x, camera.position.y, camera.position.z);
	model.draw(shader);

	glfwSwapBuffers(WINDOW);
	glfwPollEvents();
}

void NormalMapping::handleInput(float deltaTime)
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