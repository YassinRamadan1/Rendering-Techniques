#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: camera_speed_(SPEED), m_sensitivity_(SENSITIVITY), camera_zoom_(ZOOM)
{
	camera_position_ = position;
	world_up_ = up;
	camera_yaw_ = yaw;
	camera_pitch_ = pitch;
	updateCameraVectors();
}

void Camera::processMovement(Camera_Movement direction, float dt)
{
	float velocity = camera_speed_ * dt;

	if (direction == MOVE_FORWARD)
		camera_position_ += velocity * camera_front_;
	if (direction == MOVE_BACKWARD)
		camera_position_ -= velocity * camera_front_;
	if (direction == MOVE_LEFT)
		camera_position_ -= velocity * camera_right_;
	if (direction == MOVE_RIGHT)
		camera_position_ += velocity * camera_right_;
}

void Camera::processTilting(Camera_Movement direction)
{
	glm::mat4 rotation_matrix = glm::mat4(1.0f);
	if (direction == TILT_LEFT)
		rotation_matrix = glm::rotate(rotation_matrix, glm::radians(camera_roll_step_), camera_front_);
	if (direction == TILT_RIGHT)
		rotation_matrix = glm::rotate(rotation_matrix, glm::radians(-camera_roll_step_), camera_front_);
	world_up_ = glm::vec3(rotation_matrix * glm::vec4(world_up_, 0.0f));
}

void Camera::processMouseMovement(float x_offset, float y_offset)
{
	x_offset *= m_sensitivity_;
	y_offset *= m_sensitivity_;
	camera_yaw_ += x_offset;
	camera_pitch_ += y_offset;
	camera_pitch_ = camera_pitch_ > 89.0f ? 89.0f : camera_pitch_ < -89.0f ? -89.0f : camera_pitch_;
	updateCameraVectors();
}

void Camera::processMouseScroll(float y_offset)
{
	camera_zoom_ -= float(y_offset);
	camera_zoom_ = camera_zoom_ > 45.0f ? 45.0f : camera_zoom_ < 1.0 ? 1.0 : camera_zoom_;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(camera_position_, camera_position_ + camera_front_, world_up_); //MylookAt(m_Position, m_Position + m_Front, m_WorldUp);
}

void Camera::updateCameraVectors()
{
	camera_front_.x = cos(glm::radians(camera_pitch_)) * cos(glm::radians(camera_yaw_));
	camera_front_.y = sin(glm::radians(camera_pitch_));
	camera_front_.z = cos(glm::radians(camera_pitch_)) * sin(glm::radians(camera_yaw_));
	camera_right_ = glm::normalize(glm::cross(camera_front_, world_up_));
}