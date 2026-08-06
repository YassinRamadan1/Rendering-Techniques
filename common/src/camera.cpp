#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM)
{
	this->position = position;
	m_WorldUp = up;
	m_Yaw = yaw;
	m_Pitch = pitch;
	updateCameraVectors();
}

void Camera::processMovement(Camera_Movement direction, float dt)
{
	float sp = speed * dt;
	switch (direction)
	{
	case MOVE_FORWARD:
		position += sp * m_Front;
		break;
	case MOVE_BACKWARD:
		position -= sp * m_Front;
		break;
	case MOVE_LEFT:
		position -= sp * m_Right;
		break;
	case MOVE_RIGHT:
		position += sp * m_Right;
		break;
	}
}

void Camera::processTilting(Camera_Movement direction)
{
	glm::mat4 rotation_matrix = glm::mat4(1.0f);
	switch (direction)
	{
	case TILT_LEFT:
		rotation_matrix = glm::rotate(rotation_matrix, glm::radians(camera_roll_step_), m_Front);
		break;
	case TILT_RIGHT:
		rotation_matrix = glm::rotate(rotation_matrix, glm::radians(-camera_roll_step_), m_Front);
		break;
	}

	m_WorldUp = glm::vec3(rotation_matrix * glm::vec4(m_WorldUp, 0.0f));
}

void Camera::processMouseMovement(float x_offset, float y_offset)
{
	x_offset *= sensitivity;
	y_offset *= sensitivity;
	m_Yaw += x_offset;
	m_Pitch += y_offset;
	m_Pitch = m_Pitch > 89.0f ? 89.0f : m_Pitch < -89.0f ? -89.0f : m_Pitch;
	updateCameraVectors();
}

void Camera::processMouseScroll(float y_offset)
{
	zoom -= float(y_offset);
	zoom = zoom > 45.0f ? 45.0f : zoom < 1.0 ? 1.0 : zoom;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + m_Front, m_WorldUp);
}

void Camera::updateCameraVectors()
{
	m_Front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
	m_Front.y = sin(glm::radians(m_Pitch));
	m_Front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
}