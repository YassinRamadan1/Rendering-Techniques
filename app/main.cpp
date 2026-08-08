#include <iostream>
#include <vector>
#include <chrono>
#include "utility.h"
#include "Outlining/outlining.h"
#include "NormalMapping/normal_mapping.h"

int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;
int FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT;
Input INPUT;
GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "testing", nullptr, nullptr);
	
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

	NormalMapping nm(SCREEN_WIDTH, SCREEN_HEIGHT, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, INPUT, window);

	float last = glfwGetTime();
	float current = last;
	while (!glfwWindowShouldClose(window))
	{
		current = glfwGetTime();

		nm.run(current - last);
		
		last = current;
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

unsigned int loadTexture2D(const char* path, unsigned int texture_slot, bool gamma)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels, format, internal_format;
	unsigned int id;

	unsigned char* image = stbi_load(path, &width, &height, &nrChannels, 0);

	if (!image) {

		std::cout << "Error! Couldn't load texture with path :" << path << '\n';
		return 0;
	}

	format = nrChannels == 3 ? GL_RGB : nrChannels == 4 ? GL_RGBA : GL_RED;
	if (gamma)
		internal_format = nrChannels == 3 ? GL_SRGB : nrChannels == 4 ? GL_SRGB_ALPHA : GL_RED;
	else
		internal_format = format;

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + texture_slot);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(image);

	return id;
}

unsigned int loadTextureCube(const char* path)
{
	stbi_set_flip_vertically_on_load(false);
	int width, height, nrChannels, format;
	unsigned int id;

	std::vector<std::string> faces
	{
		"/right.jpg",
		"/left.jpg",
		"/top.jpg",
		"/bottom.jpg",
		"/front.jpg",
		"/back.jpg",
	};

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	for (int i = 0; i < faces.size(); i++)
	{
		std::string location = path + faces[i];
		unsigned char* image = stbi_load(location.c_str(), &width, &height, &nrChannels, 0);

		if (!image) {

			std::cout << "Error! Couldn't load texture with path :" << location << '\n';
			return 0;
		}

		format = nrChannels == 3 ? GL_RGB : nrChannels == 4 ? GL_RGBA : GL_RED;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return id;
}