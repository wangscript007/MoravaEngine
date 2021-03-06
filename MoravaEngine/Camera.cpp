#include "Camera.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/gtx/compatibility.hpp"
#include "glm/gtc/matrix_transform.hpp"


Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f)
{
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
	m_Position = position;
	m_WorldUp = up;
	m_Yaw = yaw;
	m_Pitch = pitch;
	m_Roll = 0.0f;
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);

	Update();
}

void Camera::Update()
{
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

Camera::~Camera()
{
}
