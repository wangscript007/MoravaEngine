#include "QuadInstanced.h"

#include "GL/glew.h"


QuadInstanced::QuadInstanced()
{
	float quadVertices[] = {
		// positions           // texture Coords
		-0.5f, -0.5f, 0.0f,    0.0f, 1.0f,   // bottom left
		 0.5f, -0.5f, 0.0f,    1.0f, 1.0f,   // bottom right
		-0.5f,  0.5f, 0.0f,    0.0f, 0.0f,   // top left
		 0.5f,  0.5f, 0.0f,    1.0f, 0.0f,   // top right
	};

	// setup plane VAO
	unsigned int m_VBO = 0;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void QuadInstanced::Render()
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

QuadInstanced::~QuadInstanced()
{
}
