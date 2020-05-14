#ifndef _AABB_H
#define _AABB_H

#include <iostream>
#include <glm/glm.hpp>

#include "Shader.h"


/**
 * Taken from the strifeEngine project
 */
class AABB
{
public:
	AABB(glm::vec3 position, glm::vec3 rotation, glm::vec3 scaleObject);
	bool Contains(glm::vec3 position, glm::vec3 scale);
	bool TestAABBOverlap(AABB* a, AABB* b);
	void UpdatePosition(glm::vec3 position);
	void Update(glm::vec3 position, glm::vec3 rotation, glm::vec3 scaleObject);
	void Draw(Shader* shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	glm::vec3 GetMin() const;
	glm::vec3 GetMax() const;
	virtual ~AABB();

	static bool IntersectRayAab(glm::vec3 origin, glm::vec3 dir, glm::vec3 min, glm::vec3 max, glm::vec2 result);

public:
	glm::vec3 m_Position = glm::vec3(0.0f);
	glm::vec3 m_Rotation = glm::vec3(0.0f);
	glm::vec3 m_ScaleObject = glm::vec3(1.0f);
	glm::vec3 m_ScaleAABB = glm::vec3(1.0f);
	glm::vec4 m_Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	glm::vec3 m_BoundMin;
	glm::vec3 m_BoundMax;

private:
	std::vector<glm::vec3> m_OriginVectors;

	std::vector<float> m_Vertices;
	std::vector<unsigned int> m_Indices;
	float m_UnitSize = 0.5f;
	float m_Offset = 0.02f;
	size_t m_VertexStride = 3 + 4;

};

#endif
