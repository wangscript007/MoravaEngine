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
	glm::vec3 m_Position = glm::vec3(0.0f);
	glm::vec3 m_Scale = glm::vec3(1.0f);

private:

public:
	AABB(glm::vec3 position, glm::vec3 scale);
	bool contains(glm::vec3 position, glm::vec3 scale);
	bool testAABBOverlap(AABB* a, AABB* b);
	void UpdatePosition(glm::vec3 position);
	void Draw(Shader* shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec4 color, Shader* shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	virtual ~AABB();

};
	
#endif
