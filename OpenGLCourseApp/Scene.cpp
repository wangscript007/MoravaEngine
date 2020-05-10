#include "Scene.h"

#include "MeshData.h"
#include "Tile2D.h"


SceneSettings Scene::sceneSettings;

Scene::Scene()
{
	sceneSettings.enableShadows      = false;
	sceneSettings.enableOmniShadows  = false;
	sceneSettings.enablePointLights  = false;
	sceneSettings.enableSpotLights   = false;
	sceneSettings.enableWaterEffects = false;
	sceneSettings.enableSkybox       = false;
	sceneSettings.enableNormalMaps   = false;
	sceneSettings.enableCulling      = false;
	sceneSettings.cameraPosition = glm::vec3(0.0f, 6.0f, 20.0f);
	sceneSettings.cameraStartYaw = -90.0f;
	sceneSettings.cameraStartPitch = 0.0f;
	sceneSettings.cameraMoveSpeed = 4.0f;
	sceneSettings.nearPlane = 0.01f;
	sceneSettings.farPlane = 200.0f;

	// Directional light
	sceneSettings.dirLightEnabled = true;
	sceneSettings.dirLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.ambientIntensity = 0.2f;
	sceneSettings.diffuseIntensity = 0.8f;
	sceneSettings.lightDirection = glm::vec3(3.0f, -9.0f, -3.0f);
	sceneSettings.lightProjectionMatrix = glm::ortho(-16.0f, 16.0f, -16.0f, 16.0f, 0.1f, 32.0f);

	// Point lights
	sceneSettings.pLight_0_enabled = true;
	sceneSettings.pLight_0_color = glm::vec3(1.0f, 0.0f, 1.0f);
	sceneSettings.pLight_0_position = glm::vec3(0.0f, 20.0f, 0.0f);
	sceneSettings.pLight_0_ambientIntensity = 0.5f;
	sceneSettings.pLight_0_diffuseIntensity = 0.8f;

	sceneSettings.pLight_1_enabled = true;
	sceneSettings.pLight_1_color = glm::vec3(1.0f, 0.0f, 0.0f);
	sceneSettings.pLight_1_position = glm::vec3(-2.0f, 9.6f, 0.0f);
	sceneSettings.pLight_1_diffuseIntensity = 6.0f;
	sceneSettings.pLight_1_ambientIntensity = 0.5f;
	sceneSettings.pLight_1_diffuseIntensity = 0.8f;

	sceneSettings.pLight_2_enabled = true;
	sceneSettings.pLight_2_color = glm::vec3(0.8f, 0.8f, 0.5f);
	sceneSettings.pLight_2_position = glm::vec3(-2.0f, 4.0f, 0.0f);
	sceneSettings.pLight_2_ambientIntensity = 0.5f;
	sceneSettings.pLight_2_diffuseIntensity = 0.8f;

	sceneSettings.pLight_3_enabled = true;
	sceneSettings.pLight_3_color = glm::vec3(0.8f, 0.8f, 0.5f);
	sceneSettings.pLight_3_position = glm::vec3(-2.0f, 4.0f, 0.0f);
	sceneSettings.pLight_3_ambientIntensity = 0.5f;
	sceneSettings.pLight_3_diffuseIntensity = 0.8f;


	// Spot lights
	sceneSettings.sLight_0_enabled = true;
	sceneSettings.sLight_0_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.sLight_0_position = glm::vec3(0.0f, 0.0f, 0.0f);
	sceneSettings.sLight_0_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	sceneSettings.sLight_0_edge = 35.0f;

	sceneSettings.sLight_1_enabled = true;
	sceneSettings.sLight_1_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.sLight_1_position = glm::vec3(0.0f, 0.0f, 0.0f);
	sceneSettings.sLight_1_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	sceneSettings.sLight_1_edge = 35.0f;

	sceneSettings.sLight_2_enabled = true;
	sceneSettings.sLight_2_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.sLight_2_position = glm::vec3(0.0f, 0.0f, 0.0f);
	sceneSettings.sLight_2_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	sceneSettings.sLight_2_edge = 35.0f;

	sceneSettings.sLight_3_enabled = true;
	sceneSettings.sLight_3_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.sLight_3_position = glm::vec3(0.0f, 0.0f, 0.0f);
	sceneSettings.sLight_3_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	sceneSettings.sLight_3_edge = 35.0f;

	sceneSettings.shadowMapWidth = 2048;
	sceneSettings.shadowMapHeight = 2048;
	sceneSettings.shadowSpeed = 0.4f;
	sceneSettings.waterHeight = 1.6f;
	sceneSettings.waterWaveSpeed = 0.005f;

	shadowMapWidth = 1024;
	shadowMapHeight = 1024;

	m_WireframeEnabled = false;

	SetLightManager();
	SetTextures();
	SetTextureSlots();
	SetupMaterials();
	SetupMeshes();
}

void Scene::SetTextureSlots()
{
	textureSlots.insert(std::make_pair("diffuse",    1));
	textureSlots.insert(std::make_pair("normal",     2));
	textureSlots.insert(std::make_pair("shadow",     3));
	textureSlots.insert(std::make_pair("omniShadow", 4));
	textureSlots.insert(std::make_pair("reflection", 5));
	textureSlots.insert(std::make_pair("refraction", 6));
	textureSlots.insert(std::make_pair("depth",      7));
	textureSlots.insert(std::make_pair("DuDv",       8));

	// PBR
	textureSlots.insert(std::make_pair("albedo",     1));
	textureSlots.insert(std::make_pair("normal",     2));
	textureSlots.insert(std::make_pair("metallic",   3));
	textureSlots.insert(std::make_pair("roughness",  4));
	textureSlots.insert(std::make_pair("ao",         5));

	// environment cubemap
	textureSlots.insert(std::make_pair("equirectangularMap", 6));
	textureSlots.insert(std::make_pair("environmentMap", 7));
}

void Scene::SetupMaterials()
{
	materials.insert(std::make_pair("shiny", new Material(1.0f, 128.0f)));
	materials.insert(std::make_pair("dull", new Material(1.0f, 64.0f)));
	materials.insert(std::make_pair("superShiny", new Material(1.0f, 1024.0f)));
	materials.insert(std::make_pair("superDull", new Material(1.0f, 16.0f)));
}

void Scene::SetupMeshes()
{
}

void Scene::SetTextures()
{
	textures.insert(std::make_pair("normalMapDefault", new Texture("Textures/normal_map_default.png")));
	textures.insert(std::make_pair("shadowMapDefault", new Texture("Textures/shadow_map_default.png")));
	textures.insert(std::make_pair("waterDuDv", new Texture("Textures/water/waterDuDv.png")));
	textures.insert(std::make_pair("waterNormal", new Texture("Textures/water/waterNormal.png")));
}

void Scene::SetCamera()
{
	m_Camera = new Camera(sceneSettings.cameraPosition, glm::vec3(0.0f, 1.0f, 0.0f),
		sceneSettings.cameraStartYaw, sceneSettings.cameraStartPitch, sceneSettings.cameraMoveSpeed, 0.1f);
}

void Scene::SetLightManager()
{
	m_LightManager = new LightManager(sceneSettings);
}

void Scene::SetWaterManager(int width, int height)
{
	// Water framebuffers
	m_WaterManager = new WaterManager(width, height, sceneSettings.waterHeight, sceneSettings.waterWaveSpeed);
}

Scene::~Scene()
{
	delete m_Skybox;
	delete m_Camera;
	delete m_LightManager;
	delete m_WaterManager;

	for (auto& texture : textures)
		delete texture.second;

	for (auto& mesh : meshes)
		delete mesh.second;

	for (auto& material : materials)
		delete material.second;

	for (auto& model : models)
		delete model.second;

	skyboxFaces.clear();
	textures.clear();
	textureSlots.clear();
	materials.clear();
	meshes.clear();
	models.clear();

}
