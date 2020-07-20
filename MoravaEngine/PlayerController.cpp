#include "PlayerController.h"

#include "Log.h"
#include "CommonValues.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>


PlayerController::PlayerController()
{
	m_Player = nullptr;
	m_Terrain = nullptr;
}

PlayerController::PlayerController(Player* player)
{
	m_Player = player;
	m_Terrain = nullptr;
}

void PlayerController::SetTerrain(Terrain3D* terrain)
{
	m_Terrain = terrain;
}

PlayerController::~PlayerController()
{
}

void PlayerController::KeyControl(bool* keys, float deltaTime)
{
	glm::vec3 newPosition = m_Player->GetPosition();
	glm::vec3 oldPosition = newPosition;

	// Set gravity
	newPosition += glm::vec3(0.0f, -1.0f, 0.0f) * m_Gravity;
	m_MoveDirection = glm::vec3(0.0f, -1.0f, 0.0f);

	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		newPosition += m_Player->GetFront() * m_MoveSpeed;
		m_MoveDirection = m_Player->GetFront();
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		newPosition -= m_Player->GetFront() * m_MoveSpeed;
		m_MoveDirection = -m_Player->GetFront();
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		newPosition -= m_Player->GetRight() * m_MoveSpeed;
		m_MoveDirection = -m_Player->GetRight();
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		newPosition += m_Player->GetRight() * m_MoveSpeed;
		m_MoveDirection = m_Player->GetRight();
	}
	if (keys[GLFW_KEY_Q])
	{
		newPosition -= m_Player->GetUp() * m_MoveSpeed;
		m_MoveDirection = -m_Player->GetUp();
	}
	if (keys[GLFW_KEY_E] || keys[GLFW_KEY_SPACE])
	{
		newPosition += m_Player->GetUp() * m_JumpSpeed;
		m_MoveDirection = m_Player->GetUp();
	}

	bool bColliding = IsColliding(newPosition);
	// printf("PlayerController newPosition [ %.2ff %.2ff %.2ff ] minDistance = %.2ff\n", newPosition.x, newPosition.y, newPosition.z, minDistance);

	// Check the collision
	if (bColliding) {
		// Collision detected
		if (m_MoveDirection != m_MoveDirectionLast) {
			newPosition = glm::vec3(std::round(oldPosition.x), std::round(oldPosition.y), std::round(oldPosition.z));
			m_Player->SetPosition(newPosition);
			m_MoveDirectionLast = m_MoveDirection;
		}

		// printf("PlayerController COLLISION DETECTED Position: [ %.2ff %.2ff %.2ff ] m_MoveDirectionLast [ %.2ff %.2ff %.2ff ]\n",
		// 	oldPosition.x, oldPosition.y, oldPosition.z, m_MoveDirectionLast.x, m_MoveDirectionLast.y, m_MoveDirectionLast.z);
	}
	else {
		// Collision not detected
		m_Player->SetPosition(newPosition);
		// printf("PlayerController COLLISION NOT DETECTED Position: [ %.2ff %.2ff %.2ff ]\n", newPosition.x, newPosition.y, newPosition.z);
	}
}

bool PlayerController::IsColliding(glm::vec3 position)
{
	bool isColliding = false;

	constexpr float maxFloatValue = std::numeric_limits<float>::max();
	float minDistance = maxFloatValue;

	if (m_Terrain == nullptr) {
		Log::GetLogger()->error("Missing reference to Terrain3D!");
		return isColliding;
	}

	float tempDistance;
	glm::vec3 collidingPosition;

	for (glm::vec3 terrainPosition : m_Terrain->m_Positions)
	{
		tempDistance = glm::distance(position, terrainPosition);

		//	printf("GetTerrainMinimumDistance position [ %.2ff %.2ff %.2ff ] terrainPosition [ %.2ff %.2ff %.2ff ] tempDistance = %.2ff\n",
		//		position.x, position.y, position.z, terrainPosition.x, terrainPosition.y, terrainPosition.z, tempDistance);

		if (tempDistance <= m_DistanceAllowed) {
			minDistance = tempDistance;
			collidingPosition = terrainPosition;
			break;
		}

		if (tempDistance < minDistance) {
			minDistance = tempDistance;
			collidingPosition = terrainPosition;
		}
	}

	if (minDistance <= m_DistanceAllowed) {
		isColliding = true;

		// printf("Player position [ %.2ff %.2ff %.2ff ] collidingPosition [ %.2ff %.2ff %.2ff ]\n",
		// 	position.x, position.y, position.z, collidingPosition.x, collidingPosition.y, collidingPosition.z);
	}

	return isColliding;
}

void PlayerController::MouseControl(bool* buttons, float xChange, float yChange)
{
	if (buttons[GLFW_MOUSE_BUTTON_RIGHT]) {
		glm::vec3 oldRotation = m_Player->GetRotation();
		// glm::vec3 newRotationVec = glm::vec3(oldRotation.x - yChange * m_TurnSpeed, oldRotation.y + xChange * m_TurnSpeed, oldRotation.z);
		glm::vec3 newRotation = glm::vec3(oldRotation.x, oldRotation.y - xChange * m_TurnSpeed, oldRotation.z);
		printf("PlayerController::MouseControl oldRotationVec [%.2ff %.2ff %.2ff] newRotationVec [ %.2ff %.2ff %.2ff ]\n", 
			oldRotation.x, oldRotation.y, oldRotation.z, newRotation.x, newRotation.y, newRotation.z);
		m_Player->SetRotation(newRotation);

		Update();
	}
}

void PlayerController::MouseScrollControl(bool* keys, float deltaTime, float xOffset, float yOffset)
{
}

void PlayerController::Update()
{
	m_Player->Update();
}
