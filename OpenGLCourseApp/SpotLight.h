#pragma once

#include "PointLight.h"
#include "DirectionalLight.h"


class SpotLight : public DirectionalLight, PointLight
{
public:
	SpotLight();
	SpotLight(GLuint shadowWidth, GLuint shadowHeight, GLfloat nearPlane, GLfloat farPlane,
		glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity,
		glm::vec3 position, glm::vec3 direction,
		GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat edge);
	void UseLight(GLint ambientColorLocation, GLint ambientIntensityLocation, GLint diffuseIntensityLocation,
		GLint positionLocation, GLint directionLocation,
		GLint constantLocation, GLint linearLocation, GLint exponentLocation,
		GLint edgeLocation);
	inline ShadowMap* GetShadowMap() { return PointLight::GetShadowMap(); };
	inline GLfloat GetFarPlane() { return PointLight::GetFarPlane(); };
	void SetFlash(glm::vec3 position, glm::vec3 direction);
	void Toggle() { m_Enabled = !m_Enabled; };
	~SpotLight();

private:
	GLfloat m_Edge;
	GLfloat m_EdgeProcessed;
	bool m_Enabled;

};
