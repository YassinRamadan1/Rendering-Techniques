#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "input.h"

int SCREEN_WIDTH, SCREEN_HEIGHT;
int FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT;
Input INPUT;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	auto* window = glfwCreateWindow(800, 800, "testing", nullptr, nullptr);
	
	if (window == nullptr)
	{
		std::cout << "Failed to open a window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwGetFramebufferSize(window, &FRAMEBUFFER_WIDTH, &FRAMEBUFFER_HEIGHT);
	glViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	glClearColor(1.0, 0.5, 0.1, 1.);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window, INPUT);

		if (INPUT.keyboard[Button::ESCAPE].isPressed)
			glfwSetWindowShouldClose(window, 1);


		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	FRAMEBUFFER_WIDTH = width;
	FRAMEBUFFER_HEIGHT = height;
}