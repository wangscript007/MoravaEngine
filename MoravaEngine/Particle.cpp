#include "Particle.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Timer.h"
#include "ParticleMaster.h"


Particle::Particle()
{
	m_Texture = nullptr;

	m_Position = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);
	m_Scale = glm::vec3(1.0f);

	m_Velocity = glm::vec3(0.0f);
	m_Gravity = 0.0f;
	m_LifeLength = 0.0f;

	m_WorldGravity = -1.0f;
}

Particle::Particle(ParticleTexture* texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 velocity, float gravity, float lifeLength)
	: Particle()
{
	m_Texture = texture;

	m_Position = position;
	m_Rotation = rotation;
	m_Scale = scale;

	m_Velocity = velocity;
	m_Gravity = gravity;
	m_LifeLength = lifeLength;

	ParticleMaster::addParticle(this);
}

bool Particle::Update()
{
	m_Velocity.y += (m_WorldGravity + m_Gravity) * Timer::Get()->GetDeltaTime();
	glm::vec3 change = glm::vec3(m_Velocity);
	change *= Timer::Get()->GetDeltaTime();
	m_Position += change;
	m_ElapsedTime += Timer::Get()->GetDeltaTime();

	// printf("Particle::Update() m_Velocity.y = %.2ff m_WorldGravity = %.2ff, m_Gravity = %.2ff deltaTime = %.2ff\n",
	// 	m_Velocity.y, m_WorldGravity, m_Gravity, Timer::Get()->GetDeltaTime());

	return m_ElapsedTime < m_LifeLength;
}

Particle::~Particle()
{
}
