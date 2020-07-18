#pragma once

#include "Scene.h"

#include "Terrain3D.h"
#include "RenderInstanced.h"


class SceneVoxelTerrain : public Scene
{

public:
	SceneVoxelTerrain();
	virtual ~SceneVoxelTerrain() override;

	virtual void Update(float timestep, Window& mainWindow) override;
	virtual void UpdateImGui(float timestep, Window& mainWindow) override;
	virtual void Render(Window& mainWindow, glm::mat4 projectionMatrix, std::string passType,
		std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms) override;
	void UpdateCooldown(float timestep, Window& mainWindow);
	void Release();

private:
	virtual void SetupTextures() override;
	virtual void SetupTextureSlots() override;
	virtual void SetupMeshes() override;

	Terrain3D* m_Terrain3D;
	glm::mat4 m_Transform;
	RenderInstanced* m_RenderInstanced;

	glm::vec3 m_TerrainScale;
	float m_TerrainNoiseFactor;

	EventCooldown m_UpdateCooldown;

};
