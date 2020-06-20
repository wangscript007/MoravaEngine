#pragma once

#include "Particle.h"
#include "ParticleRenderer.h"
#include "Camera.h"

#include <map>


class ParticleMaster
{
public:
	ParticleMaster();
	static void Init(bool instancedRendering, int maxInstances);
	static void Update(glm::vec3 cameraPosition);
	static void Render(glm::mat4 viewMatrix);
	static void addParticle(Particle* particle);
	static std::map<int, int> GetCounts();
	static void CleanUp();
	~ParticleMaster();

private:
	static std::map<ParticleTexture*, std::vector<Particle*>*> m_Particles;
	static ParticleRenderer* m_Renderer;
};
