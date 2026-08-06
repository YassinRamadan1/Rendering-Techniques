#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement
{
	MOVE_FORWARD, MOVE_BACKWARD, MOVE_LEFT, MOVE_RIGHT, TILT_RIGHT, TILT_LEFT
};



class Camera
{
	static constexpr float YAW = -90.0f;
	static constexpr float PITCH = 0.0f;
	static constexpr float ROLL = 0.0f;
	static constexpr float SENSITIVITY = 0.05f;
	static constexpr float SPEED = 2.5f;
	static constexpr float ZOOM = 45.0f;

	glm::vec3 m_Front;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	float m_Yaw;
	float m_Pitch;
public:

	float speed;
	float sensitivity;
	float zoom;
	float camera_roll_step_ = 2.0f;

	glm::vec3 position;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	void processMovement(Camera_Movement direction, float dt);

	void processTilting(Camera_Movement direction);

	void processMouseMovement(float x_offset, float y_offset);

	void processMouseScroll(float y_offset);

	glm::mat4 getViewMatrix();

private:

	void updateCameraVectors();
};


