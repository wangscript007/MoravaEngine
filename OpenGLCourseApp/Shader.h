#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"


class Shader
{
public:
	Shader();
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);
	std::string ReadFile(const char* fileLocation);
	void Validate();
	void Bind();
	void Unbind();
	void ClearShader();
	~Shader();

	GLuint GetProgramID();
	GLint GetModelLocation();
	GLint GetViewLocation();
	GLint GetProjectionLocation();

	GLuint GetUniformLocationNearPlane();
	GLuint GetUniformLocationFarPlane();

	GLint GetUniformLocationNormalMap();
	GLint GetUniformLocationDepthMap();
	GLint GetUniformLocationLightPosition();

	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetLightPosition(glm::vec3 lightPosition);
	void SetViewMatrix(glm::mat4* viewMatrix);
	void SetProjectionMatrix(glm::mat4* projectionMatrix);

	void SetTexture(GLuint textureUnit);
	void SetNormalMap(GLuint textureUnit);
	void SetDepthMap(GLuint textureUnit);

	// Directional light shadow map
	void SetDirectionalLightTransform(glm::mat4* transform);
	void SetDirectionalShadowMap(GLuint textureUnit);

	// Omni shadow maps
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

protected:
	void CompileProgram();
	virtual void GetUniformLocations();

private:
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(GLuint programID, const char* shaderCode, GLenum shaderType);

	const char* GetShaderTypeNameFromEnum(const GLenum shaderType);

protected:
	GLuint programID = -1;
	GLint uniformLightPosition = -1;
	GLint uniformTexture = -1;
	GLint uniformNormalMap = -1;
	GLint uniformDepthMap = -1;

private:
	GLint shaderID = -1;

	bool m_Validated = false;

	// Locations of uniform variables
	GLint uniformModel = -1;
	GLint uniformView = -1;
	GLint uniformProjection = -1;
	GLint uniformNearPlane = -1;
	GLint uniformFarPlane = -1;

	// directional light shadow map
	GLint uniformDirectionalShadowMap = -1;
	GLint uniformDirectionalLightTransform = -1;

	// omni shadow map
	GLint uniformLightMatrices[6];

	struct
	{
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];
};
