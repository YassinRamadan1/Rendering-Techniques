#include "input.h"

void processInput(GLFWwindow* window, Input& input)
{
	int keyState;
	for (int i = 0; i < Button::BUTTONS_COUNT; ++i)
	{
		input.keyboard[i].isPressed = 0;
		input.keyboard[i].isHeld = 0;
		input.keyboard[i].isReleased = 0;
		keyState = glfwGetKey(window, Button::buttonMapping[i]);
		if (keyState == GLFW_PRESS)
		{
			input.keyboard[i].isPressed = 1;
		}
		if (keyState == GLFW_REPEAT)
		{
			input.keyboard[i].isHeld = 1;
		}
		if (keyState == GLFW_RELEASE)
		{
			input.keyboard[i].isReleased = 1;
		}
	}

	keyState = glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT);
	if (keyState == GLFW_PRESS)
	{
		input.leftMouseButton.isPressed = 1;
	}
	if (keyState == GLFW_REPEAT)
	{
		input.leftMouseButton.isHeld = 1;
	}
	if (keyState == GLFW_RELEASE)
	{
		input.leftMouseButton.isReleased = 1;
	}

	keyState = glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (keyState == GLFW_PRESS)
	{
		input.rightMouseButton.isPressed = 1;
	}
	if (keyState == GLFW_REPEAT)
	{
		input.rightMouseButton.isHeld = 1;
	}
	if (keyState == GLFW_RELEASE)
	{
		input.rightMouseButton.isReleased = 1;
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	input.distanceTravelledX = xpos - input.cursorX;
	input.distanceTravelledY = ypos - input.cursorY;
	input.cursorX = xpos;
	input.cursorY = ypos;
}