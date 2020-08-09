#pragma once

#include "Scene.h"

#include "RenderInstanced.h"
#include "Player.h"
#include "PlayerController.h"
#include "Raycast.h"
#include "MapGenerator.h"
#include "TerrainMarchingCubes.h"


class SceneMarchingCubes : public Scene
{

public:
	SceneMarchingCubes();
	virtual ~SceneMarchingCubes() override;

	virtual void Update(float timestep, Window& mainWindow) override;
	virtual void UpdateImGui(float timestep, Window& mainWindow) override;
	virtual void Render(Window& mainWindow, glm::mat4 projectionMatrix, std::string passType,
		std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms) override;
	void UpdateCooldown(float timestep, Window& mainWindow);
	void Release();
	inline Raycast* GetRaycast() const { return m_Raycast; };
	std::vector<glm::vec3> GetRayIntersectPositions(float timestep, Camera* camera);
	void Dig(bool* keys, float timestep);
	void CastRay(bool* keys, bool* buttons, float timestep);
	bool IsRayIntersectPosition(glm::vec3 position);
	void OnClick(bool* keys, bool* buttons, float timestep);
	bool IsPositionVacant(glm::ivec3 queryPosition);
	void AddVoxel();
	void DeleteVoxel();

private:
	virtual void SetCamera() override;
	virtual void SetupTextures() override;
	virtual void SetupTextureSlots() override;
	virtual void SetupMeshes() override;
	void CheckMapRebuildRequirements();

	RenderInstanced* m_RenderInstanced;

	EventCooldown m_UpdateCooldown;
	EventCooldown m_DigCooldown;
	EventCooldown m_RayIntersectCooldown;
	EventCooldown m_RayCastCooldown;
	EventCooldown m_OnClickCooldown;

	Player* m_Player;
	PlayerController* m_PlayerController;
	bool m_TerrainSettingsChanged;
	Pivot* m_PivotScene;
	float m_DigDistance;
	Raycast* m_Raycast;

	// Scene settings
	bool m_DrawGizmos;
	bool m_RenderPlayer;
	bool m_RenderTerrainVoxels;
	bool m_RenderTerrainMarchingCubes;

	bool m_UnlockRotation;
	bool m_UnlockRotationPrev;

	// mouse cursor intersection
	std::vector<glm::vec3> m_IntersectPositionVector;
	glm::vec3 m_IntersectPosition;
	bool m_DeleteMode; // add or delete
	int m_IntersectPositionIndex;

	glm::vec4 m_CubeColor;
	unsigned int m_DeleteVoxelCodeGLFW;

	TerrainMarchingCubes* m_TerrainMarchingCubes;

	MapGenerator::MapGenConf m_MapGenConf;
	MapGenerator::MapGenConf m_MapGenConfPrev;

	int m_HeightMapMultiplier;
	int m_HeightMapMultiplierPrev;

	float m_SeaLevel;
	float m_SeaLevelPrev;

	int m_LevelOfDetail;
	int m_LevelOfDetailPrev;

	bool m_IsRequiredMapUpdate;
	bool m_IsRequiredMapRebuild;

	std::vector<std::string> m_ActiveRenderPasses;

	int m_VoxelsModified;
};
