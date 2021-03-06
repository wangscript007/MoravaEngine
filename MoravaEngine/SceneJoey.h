#pragma once

#include "Scene.h"
#include "LearnOpenGL/SphereJoey.h"
#include "MaterialWorkflowPBR.h"


const unsigned int SCENE_JOEY_LIGHT_COUNT = 4;

class SceneJoey : public Scene
{
public:
	SceneJoey();
	virtual void Update(float timestep, Window& mainWindow) override;
	virtual void UpdateImGui(float timestep, Window& mainWindow) override;
	virtual void Render(Window& mainWindow, glm::mat4 projectionMatrix, std::string passType,
		std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms) override;
	virtual void RenderWater(glm::mat4 projectionMatrix, std::string passType,
		std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms) override;
	virtual ~SceneJoey() override;

private:
	virtual void SetupTextures() override;
	virtual void SetupMaterials() override;
	virtual void SetupModels() override;
	void SetupGeometry();
	void SetupLights();
	virtual void SetSkybox() override;

public:
	glm::vec3 m_LightPositions[SCENE_JOEY_LIGHT_COUNT];
	glm::vec3 m_LightColors[SCENE_JOEY_LIGHT_COUNT];
	float m_EmissiveFactor;
	float m_MetalnessFactor;
	float m_RoughnessFactor;
	bool m_IsRotating;
	float m_RotationFactor;

private:
	glm::vec3 m_CameraPosition;
	glm::vec3 m_LightPositionOffset[SCENE_JOEY_LIGHT_COUNT];
	glm::vec3 m_LightColorsNormal[SCENE_JOEY_LIGHT_COUNT];

	MaterialWorkflowPBR* m_MaterialWorkflowPBR;
	SphereJoey* m_SphereJoey;
	float m_Timestep = 0.0f;

	float m_MaterialSpecular = 1.0f;
	float m_MaterialShininess = 256.0f;
};
