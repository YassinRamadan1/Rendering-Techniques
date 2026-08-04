#pragma once
#include<GLFW/glfw3.h>

struct Button
{
	unsigned char isPressed = 0;
	unsigned char isHeld = 0;
	unsigned char isReleased = 0;

	enum
	{
		A = 0,
		B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

		NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,

		SPACE, ESCAPE, ENTER,

		RIGHT, LEFT, DOWN, UP,

		SHIFT, BACKSPACE,

		PLUS_EQUAL,
		PERIOD_RIGHTARROW,
		MINUS_UNDERSCORE,
		COMMA_LEFTARROW,
		SEMICOLON,
		QUESTION_BACKSLASH,
		TILDE,
		QUOTES,
		SLASH,
		SQUAREBRACKETSOPEN,
		SQUAREBRACKETSCLOSE,

		BUTTONS_COUNT,
	};

    static constexpr int buttonMapping[BUTTONS_COUNT] =
    {
        // A-Z
        GLFW_KEY_A,
        GLFW_KEY_B,
        GLFW_KEY_C,
        GLFW_KEY_D,
        GLFW_KEY_E,
        GLFW_KEY_F,
        GLFW_KEY_G,
        GLFW_KEY_H,
        GLFW_KEY_I,
        GLFW_KEY_J,
        GLFW_KEY_K,
        GLFW_KEY_L,
        GLFW_KEY_M,
        GLFW_KEY_N,
        GLFW_KEY_O,
        GLFW_KEY_P,
        GLFW_KEY_Q,
        GLFW_KEY_R,
        GLFW_KEY_S,
        GLFW_KEY_T,
        GLFW_KEY_U,
        GLFW_KEY_V,
        GLFW_KEY_W,
        GLFW_KEY_X,
        GLFW_KEY_Y,
        GLFW_KEY_Z,

        // 0-9
        GLFW_KEY_0,
        GLFW_KEY_1,
        GLFW_KEY_2,
        GLFW_KEY_3,
        GLFW_KEY_4,
        GLFW_KEY_5,
        GLFW_KEY_6,
        GLFW_KEY_7,
        GLFW_KEY_8,
        GLFW_KEY_9,

        // Common keys
        GLFW_KEY_SPACE,
        GLFW_KEY_ESCAPE,
        GLFW_KEY_ENTER,

        // Arrows
        GLFW_KEY_RIGHT,
        GLFW_KEY_LEFT,
        GLFW_KEY_DOWN,
        GLFW_KEY_UP,

        // Modifiers
        GLFW_KEY_LEFT_SHIFT,   // or handle both shifts separately
        GLFW_KEY_BACKSPACE,

        // Symbols
        GLFW_KEY_EQUAL,           // = / +
        GLFW_KEY_PERIOD,          // . / >
        GLFW_KEY_MINUS,           // - / _
        GLFW_KEY_COMMA,           // , / <
        GLFW_KEY_SEMICOLON,       // ; / :
        GLFW_KEY_BACKSLASH,       // \ / |
        GLFW_KEY_GRAVE_ACCENT,    // ` / ~
        GLFW_KEY_APOSTROPHE,      // ' / "
        GLFW_KEY_SLASH,           // / / ?
        GLFW_KEY_LEFT_BRACKET,    // [
        GLFW_KEY_RIGHT_BRACKET    // ]
    };
};

struct Input
{
	float cursorX = 0;
	float cursorY = 0;
	float distanceTravelledX = 0;
	float distanceTravelledY = 0;
	Button leftMouseButton = {};
	Button rightMouseButton = {};
	Button keyboard[Button::BUTTONS_COUNT];
};

void processInput(GLFWwindow* window, Input& input);
