#include "bloom.h"

Bloom::Bloom(int scrWidth, int scrHeight, int fbWidth, int fbHeight, Input& input, GLFWwindow* window)
	: SCREEN_WIDTH(scrWidth), SCREEN_HEIGHT(scrHeight), FRAMEBUFFER_WIDTH(fbWidth), FRAMEBUFFER_HEIGHT(fbHeight),
	INPUT(&input), WINDOW(window)
{
	for (int i = 0; i < 4; ++i)
	{
		l[i].specular = glm::vec3(15.0f);
		l[i].constant = 1.0f;
		l[i].linear = 0.09f;
		l[i].quadratic = .032f;
	}

	l[0].position = glm::vec3(3.0f, 0.0f, -8.0f);
	l[0].ambient = glm::vec3(0.0f);
	l[0].diffuse = glm::vec3(10.0f, 0.0f, 0.0f);

	l[1].position = glm::vec3(-5.0f, 0.0f, -8.0f);
	l[1].ambient = glm::vec3(0.0f);
	l[1].diffuse = glm::vec3(0.0f, 0.0f, 10.0f);

	l[2].position = glm::vec3(2.0f, 1.0f, -10.0f);
	l[2].ambient = glm::vec3(0.0f);
	l[2].diffuse = glm::vec3(0.0f, 12.0f, 0.0f);

	l[3].position = glm::vec3(-2.0f, 0.0f, -5.0f);
	l[3].ambient = glm::vec3(0.02f, 0.02f, 0.02f);
	l[3].diffuse = glm::vec3(15.0f, 15.0f, 15.0f);


	std::string e;
	std::string s = std::string(TECHNIQUES_PATH);
	std::string vsPath = s + "Bloom&HDR/shaders/cube_vs.shader";
	std::string fsPath = s + "Bloom&HDR/shaders/cube_fs.shader";
	cubeShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "Bloom&HDR/shaders/light_vs.shader";
	fsPath = s + "Bloom&HDR/shaders/light_fs.shader";
	lightShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "Bloom&HDR/shaders/plane_vs.shader";
	fsPath = s + "Bloom&HDR/shaders/plane_fs.shader";
	planeShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "Bloom&HDR/shaders/quad_vs.shader";
	fsPath = s + "Bloom&HDR/shaders/quad_fs.shader";
	quadShader.createProgram(vsPath, fsPath, e);

	vsPath = s + "Bloom&HDR/shaders/quad_vs.shader";
	fsPath = s + "Bloom&HDR/shaders/quadBlur_fs.shader";
	blurShader.createProgram(vsPath, fsPath, e);

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
		-1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		-1.0f, 0.0f, -2.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		 1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		 1.0f, 0.0f, -2.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		-1.0f, 0.0f, -2.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetFramebufferSize(WINDOW, &FRAMEBUFFER_WIDTH, &FRAMEBUFFER_HEIGHT);
	glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	glClearColor(0, 0, 0, 1.);
	glEnable(GL_DEPTH_TEST);

	textureSlotCube = 0;
	textureSlotPlane = 1;
	textureSlotCubeSpec = 2;
	std::string texturePathCube = RESOURCES_PATH + std::string("textures/container2.png");
	std::string texturePathPlane = RESOURCES_PATH + std::string("textures/plane.png");
	std::string texturePathCubeSpec = RESOURCES_PATH + std::string("textures/container2_specular.png");
	textureIdCube = loadTexture2D(texturePathCube.c_str(), textureSlotCube, true);
	textureIdPlane = loadTexture2D(texturePathPlane.c_str(), textureSlotPlane, true);
	textureIdCubeSpec = loadTexture2D(texturePathCubeSpec.c_str(), textureSlotCubeSpec, true);

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

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &colorAttachment0Tex);
	glBindTexture(GL_TEXTURE_2D, colorAttachment0Tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment0Tex, 0);

	glGenTextures(1, &colorAttachment1Tex);
	glBindTexture(GL_TEXTURE_2D, colorAttachment1Tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, colorAttachment1Tex, 0);

	unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glClearColor(0, 0, 0, 1.);

	glGenFramebuffers(1, &fbo2);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
	glGenTextures(1, &colorAttachment2Tex);
	glBindTexture(GL_TEXTURE_2D, colorAttachment2Tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment2Tex, 0);
	glGenFramebuffers(1, &fbo3);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo3);
	glGenTextures(1, &colorAttachment3Tex);
	glBindTexture(GL_TEXTURE_2D, colorAttachment3Tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment3Tex, 0);
}

void Bloom::run(float deltaTime)
{
	handleInput(deltaTime);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	renderScene();

	int numberOfPasses = 5;
	if (INPUT->keyboard[Button::NUM_0].isPressed)
		numberOfPasses = 10;
	bool firstPass = true;
	blurShader.use();
	while (numberOfPasses--)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, firstPass ? colorAttachment1Tex : colorAttachment3Tex);
		blurShader.set2Float("resolution", FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
		blurShader.set1Int("tex", 0);
		blurShader.set1Int("horizontal", 1);
		glBindVertexArray(quad_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo3);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorAttachment2Tex);
		blurShader.set2Float("resolution", FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
		blurShader.set1Int("tex", 0);
		blurShader.set1Int("horizontal", 0); 
		glBindVertexArray(quad_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		firstPass = false;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	quadShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorAttachment0Tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorAttachment3Tex);
	quadShader.set2Float("resolution", FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	quadShader.set1Int("tex1", 0);
	quadShader.set1Int("tex2", 1);
	quadShader.set1Float("exposure", 0.18);
	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(WINDOW);
	glfwPollEvents();
}

void Bloom::renderScene()
{
	model = glm::translate(glm::mat4(1.), glm::vec3(0, -3, 5)) * glm::scale(glm::mat4(1.), glm::vec3(10.));
	view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(camera.zoom), SCREEN_WIDTH * 1.F / SCREEN_HEIGHT, 0.1F, 100.F);

	planeShader.use();
	planeShader.setMatrix4f("model", false, glm::value_ptr(model));
	planeShader.setMatrix4f("view", false, glm::value_ptr(view));
	planeShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	glActiveTexture(GL_TEXTURE0 + textureSlotPlane);
	glBindTexture(GL_TEXTURE_2D, textureIdPlane);
	planeShader.set1Int("tex", textureSlotPlane);
	planeShader.set3Float("cameraPos", camera.position.x, camera.position.y, camera.position.z);
	
	planeShader.set3Float("light[0].position", l[0].position.x, l[0].position.y, l[0].position.z);
	planeShader.set3Float("light[0].ambient", l[0].ambient.x, l[0].ambient.y, l[0].ambient.z);
	planeShader.set3Float("light[0].diffuse", l[0].diffuse.x, l[0].diffuse.y, l[0].diffuse.z);
	planeShader.set3Float("light[0].specular", l[0].specular.x, l[0].specular.y, l[0].specular.z);
	planeShader.set1Float("light[0].constant", l[0].constant);
	planeShader.set1Float("light[0].linear", l[0].linear);
	planeShader.set1Float("light[0].quadratic", l[0].quadratic);

	planeShader.set3Float("light[1].position", l[1].position.x, l[1].position.y, l[1].position.z);
	planeShader.set3Float("light[1].ambient", l[1].ambient.x, l[1].ambient.y, l[1].ambient.z);
	planeShader.set3Float("light[1].diffuse", l[1].diffuse.x, l[1].diffuse.y, l[1].diffuse.z);
	planeShader.set3Float("light[1].specular", l[1].specular.x, l[1].specular.y, l[1].specular.z);
	planeShader.set1Float("light[1].constant", l[1].constant);
	planeShader.set1Float("light[1].linear", l[1].linear);
	planeShader.set1Float("light[1].quadratic", l[1].quadratic);

	planeShader.set3Float("light[2].position", l[2].position.x, l[2].position.y, l[2].position.z);
	planeShader.set3Float("light[2].ambient", l[2].ambient.x, l[2].ambient.y, l[2].ambient.z);
	planeShader.set3Float("light[2].diffuse", l[2].diffuse.x, l[2].diffuse.y, l[2].diffuse.z);
	planeShader.set3Float("light[2].specular", l[2].specular.x, l[2].specular.y, l[2].specular.z);
	planeShader.set1Float("light[2].constant", l[2].constant);
	planeShader.set1Float("light[2].linear", l[2].linear);
	planeShader.set1Float("light[2].quadratic", l[2].quadratic);

	planeShader.set3Float("light[3].position", l[3].position.x, l[3].position.y, l[3].position.z);
	planeShader.set3Float("light[3].ambient", l[3].ambient.x, l[3].ambient.y, l[3].ambient.z);
	planeShader.set3Float("light[3].diffuse", l[3].diffuse.x, l[3].diffuse.y, l[3].diffuse.z);
	planeShader.set3Float("light[3].specular", l[3].specular.x, l[3].specular.y, l[3].specular.z);
	planeShader.set1Float("light[3].constant", l[3].constant);
	planeShader.set1Float("light[3].linear", l[3].linear);
	planeShader.set1Float("light[3].quadratic", l[3].quadratic);

	glBindVertexArray(plane_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	model = glm::mat4(1.);
	lightShader.use();
	lightShader.setMatrix4f("view", false, glm::value_ptr(view));
	lightShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	glBindVertexArray(cube_vao);
	for (int i = 0; i < 4; ++i)
	{
		model = glm::translate(glm::mat4(1.), l[i].position);
		lightShader.setMatrix4f("model", false, glm::value_ptr(model));
		lightShader.set3Float("lightColor", l[i].diffuse.r, l[i].diffuse.g, l[i].diffuse.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	model = glm::translate(glm::mat4(1.), glm::vec3(0., -2.45, -9.));
	cubeShader.use();
	cubeShader.setMatrix4f("model", false, glm::value_ptr(model));
	cubeShader.setMatrix4f("view", false, glm::value_ptr(view));
	cubeShader.setMatrix4f("projection", false, glm::value_ptr(projection));
	glActiveTexture(GL_TEXTURE0 + textureSlotCube);
	glBindTexture(GL_TEXTURE_2D, textureIdCube);
	glActiveTexture(GL_TEXTURE0 + textureSlotCubeSpec);
	glBindTexture(GL_TEXTURE_2D, textureIdCubeSpec);
	cubeShader.set1Int("tex", textureSlotCube);
	cubeShader.set1Int("spec", textureSlotCubeSpec);
	cubeShader.set3Float("cameraPos", camera.position.x, camera.position.y, camera.position.z);
	
	cubeShader.set3Float("light[0].position", l[0].position.x, l[0].position.y, l[0].position.z);
	cubeShader.set3Float("light[0].ambient", l[0].ambient.x, l[0].ambient.y, l[0].ambient.z);
	cubeShader.set3Float("light[0].diffuse", l[0].diffuse.x, l[0].diffuse.y, l[0].diffuse.z);
	cubeShader.set3Float("light[0].specular", l[0].specular.x, l[0].specular.y, l[0].specular.z);
	cubeShader.set1Float("light[0].constant", l[0].constant);
	cubeShader.set1Float("light[0].linear", l[0].linear);
	cubeShader.set1Float("light[0].quadratic", l[0].quadratic);

	cubeShader.set3Float("light[1].position", l[1].position.x, l[1].position.y, l[1].position.z);
	cubeShader.set3Float("light[1].ambient", l[1].ambient.x, l[1].ambient.y, l[1].ambient.z);
	cubeShader.set3Float("light[1].diffuse", l[1].diffuse.x, l[1].diffuse.y, l[1].diffuse.z);
	cubeShader.set3Float("light[1].specular", l[1].specular.x, l[1].specular.y, l[1].specular.z);
	cubeShader.set1Float("light[1].constant", l[1].constant);
	cubeShader.set1Float("light[1].linear", l[1].linear);
	cubeShader.set1Float("light[1].quadratic", l[1].quadratic);

	cubeShader.set3Float("light[2].position", l[2].position.x, l[2].position.y, l[2].position.z);
	cubeShader.set3Float("light[2].ambient", l[2].ambient.x, l[2].ambient.y, l[2].ambient.z);
	cubeShader.set3Float("light[2].diffuse", l[2].diffuse.x, l[2].diffuse.y, l[2].diffuse.z);
	cubeShader.set3Float("light[2].specular", l[2].specular.x, l[2].specular.y, l[2].specular.z);
	cubeShader.set1Float("light[2].constant", l[2].constant);
	cubeShader.set1Float("light[2].linear", l[2].linear);
	cubeShader.set1Float("light[2].quadratic", l[2].quadratic);

	cubeShader.set3Float("light[3].position", l[3].position.x, l[3].position.y, l[3].position.z);
	cubeShader.set3Float("light[3].ambient", l[3].ambient.x, l[3].ambient.y, l[3].ambient.z);
	cubeShader.set3Float("light[3].diffuse", l[3].diffuse.x, l[3].diffuse.y, l[3].diffuse.z);
	cubeShader.set3Float("light[3].specular", l[3].specular.x, l[3].specular.y, l[3].specular.z);
	cubeShader.set1Float("light[3].constant", l[3].constant);
	cubeShader.set1Float("light[3].linear", l[3].linear);
	cubeShader.set1Float("light[3].quadratic", l[3].quadratic);
	
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::translate(glm::mat4(1.), glm::vec3(2., -2.45, -7.));
	cubeShader.setMatrix4f("model", false, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::translate(glm::mat4(1.), glm::vec3(-2., -2.45, -11.));
	cubeShader.setMatrix4f("model", false, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::translate(glm::mat4(1.), glm::vec3(-1., -2.45, -6.));
	cubeShader.setMatrix4f("model", false, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::translate(glm::mat4(1.), glm::vec3(6, -2.45, -10.));
	cubeShader.setMatrix4f("model", false, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);


	model = glm::translate(glm::mat4(1.), glm::vec3(-7., -2.45, -10.));
	cubeShader.setMatrix4f("model", false, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Bloom::handleInput(float deltaTime)
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