#include "direct_lighting.h"

DirectLighting::DirectLighting(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window)
	: SCREEN_WIDTH(scrWidth), SCREEN_HEIGHT(scrHeight), FRAMEBUFFER_WIDTH(fbWidth), FRAMEBUFFER_HEIGHT(fbHeight),
	INPUT(&input), WINDOW(window), model(std::string(RESOURCES_PATH) + "models/sphere/sphere.glb")
{

	lightPositions[0] = glm::vec3(0.0, 10.0, 0.0);
	lightPositions[1] = glm::vec3(-5.0, 2.0, -3.0);
	lightPositions[2] = glm::vec3(5.0, 1.5, 3.0);
	lightPositions[3] = glm::vec3(0.0, 4.0, 8.0);

	lightColors[0] = glm::vec3(8.0, 8.0, 8.0);
	lightColors[1] = glm::vec3(6.0, 4.0, 2.0);
	lightColors[2] = glm::vec3(1.0, 6.0, 8.0);
	lightColors[3] = glm::vec3(10.0, 0.0, 0.0);

	std::string texAlbedo = std::string(RESOURCES_PATH) + "textures/pbr/plastic/albedo.png";
	std::string texNormal = std::string(RESOURCES_PATH) + "textures/pbr/plastic/normal.png";
	std::string texMetalness = std::string(RESOURCES_PATH) + "textures/pbr/plastic/metalness.png";
	std::string texRoughness = std::string(RESOURCES_PATH) + "textures/pbr/plastic/roughness.png";
	std::string texAO = std::string(RESOURCES_PATH) + "textures/pbr/plastic/ao.png";
	textureSlotAlbedo = 5;
	textureSlotNormal = 6;
	textureSlotMetalness = 7;
	textureSlotRoughness = 8;
	textureSlotAO = 9;
	textureIdAlbedo = loadTexture2D(texAlbedo.c_str(), textureSlotAlbedo, false);
	textureIdNormal = loadTexture2D(texNormal.c_str(), textureSlotNormal, false);
	textureIdMetalness = loadTexture2D(texMetalness.c_str(), textureSlotMetalness, false);
	textureIdRoughness = loadTexture2D(texRoughness.c_str(), textureSlotRoughness, false);
	textureIdAO = loadTexture2D(texAO.c_str(), textureSlotAO, false);

	std::string e;
	std::string s = std::string(TECHNIQUES_PATH);
	std::string vsPath = s + "PBR/DirectLighting/shaders/pbr_vs.shader";
	std::string fsPath = s + "PBR/DirectLighting/shaders/pbr_fs.shader";
	shader.createProgram(vsPath, fsPath, e);

	vsPath = s + "PBR/DirectLighting/shaders/light_vs.shader";
	fsPath = s + "PBR/DirectLighting/shaders/light_fs.shader";
	lightShader.createProgram(vsPath, fsPath, e);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetFramebufferSize(WINDOW, &FRAMEBUFFER_WIDTH, &FRAMEBUFFER_HEIGHT);
	glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	glClearColor(0.15, 0.25, 0.4, 1.);
	glEnable(GL_DEPTH_TEST);
}

void DirectLighting::run(float deltaTime)
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
		model.draw(lightShader);
	}

	modelMat = glm::translate(glm::mat4(1.), glm::vec3(0., -2., -3.));

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

	shader.set1Int("tex_albedo", textureSlotAlbedo);
	shader.set1Int("tex_normal", textureSlotNormal);
	shader.set1Int("tex_metalness", textureSlotMetalness);
	shader.set1Int("tex_roughness", textureSlotRoughness);
	shader.set1Int("tex_ao", textureSlotAO);
	model.draw(shader);

	glfwSwapBuffers(WINDOW);
	glfwPollEvents();
}

void DirectLighting::handleInput(float deltaTime)
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