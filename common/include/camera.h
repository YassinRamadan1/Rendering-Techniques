#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
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

	glm::vec3 camera_front_;
	glm::vec3 camera_right_;
	glm::vec3 world_up_;
	float camera_yaw_;
	float camera_pitch_;
public:

	float camera_speed_;
	float m_sensitivity_;
	float camera_zoom_;
	float camera_roll_step_ = 2.0f;

	glm::vec3 camera_position_;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	void processMovement(Camera_Movement direction, float dt);

	void processTilting(Camera_Movement direction);

	void processMouseMovement(float x_offset, float y_offset);

	void processMouseScroll(float y_offset);

	glm::mat4 getViewMatrix();

private:

	void updateCameraVectors();
};


