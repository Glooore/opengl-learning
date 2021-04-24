#ifndef _CAMERA_H_
#define _CAMERA_H_

/* #include <GLFW/glfw.h> */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum camera_movement {FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN};

namespace {
	float YAW = -90.0f;
	float PITCH = 0.0f;

	float SPEED = 5.0f;
	float SENSITIVITY = 0.1f;
	float FOV = 45.0f;
} // namespace

class Camera
{
	private:
		void updateCameraVectors()
		{
			glm::vec3 front;
			front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
			front.y = sin(glm::radians(_pitch));
			front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

			_camera_front = glm::normalize(front);
			_camera_right = glm::normalize(glm::cross(_camera_front, _world_up));
			_camera_up = glm::normalize(glm::cross(_camera_right, _camera_front));
		}
	protected:
		glm::vec3 _camera_pos, _camera_up, _camera_front, _camera_right;
		glm::vec3 _world_up;
		float _fov;
		float _yaw, _pitch;
		float _lastX, _lastY;
		float _speed;
		float _mouse_sensitivity;
	public:
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW,
			float pitch = PITCH)
		{
			_camera_pos = position;
			_world_up = up;
			_camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
			_yaw = yaw;
			_pitch = pitch;
			_fov = FOV;
			_speed = SPEED;
			_mouse_sensitivity = SENSITIVITY;

			updateCameraVectors();
		}

		glm::mat4 getViewMatrix(void)
		{
			return glm::lookAt(_camera_pos,  _camera_pos + _camera_front, _camera_up);
		}

		float getFOV(void)
		{
			return _fov;
		}

		glm::vec3 getPosition(void)
		{
			return _camera_pos;
		}

		void processKeyboard(camera_movement direction, float delta_time)
		{
			float _velocity = _speed * delta_time;
			if (direction == FORWARD) _camera_pos += _camera_front * _velocity;
			if (direction == BACKWARD) _camera_pos -= _camera_front * _velocity;
			if (direction == LEFT) _camera_pos -= _camera_right * _velocity;
			if (direction == RIGHT) _camera_pos += _camera_right * _velocity;
			if (direction == UP) _camera_pos += _world_up * _velocity;
			if (direction == DOWN) _camera_pos -= _world_up * _velocity;
		}
		
		void processMouseMovement(float x_offset, float y_offset, 
				GLboolean constrain_pitch = true)
		{
			x_offset *= _mouse_sensitivity;
			y_offset *= _mouse_sensitivity;

			_yaw += x_offset;
			_pitch -= y_offset;

			if (constrain_pitch)
			{
				if (_pitch > 89.0f) _pitch = 89.0f;
				if (_pitch < -89.0f) _pitch = -89.0f;
			}
			updateCameraVectors();
		}
};




#endif
