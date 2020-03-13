#pragma once

#include "glm/glm.hpp"

#include "Window.h"
#include "Scene.h"
#include "Shader.h"
#include "WaterManager.h"
#include "Cubemap.h"
#include "Cube.h"


class Renderer
{

public:

	static void Init();
	static void SetUniforms();
	static void SetShaders();
	static std::map<std::string, Shader*> GetShaders() { return shaders; };

	static void RenderOmniShadows(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Scene* scene, WaterManager* waterManager);
	static void RenderWaterEffects(WaterManager* waterManager, glm::mat4 projectionMatrix, Scene* scene, Camera* camera, float deltaTime);

	static void RenderPass(glm::mat4 projectionMatrix, Window& mainWindow, Scene* scene, Camera* camera, WaterManager* waterManager);
	static void RenderPassShadow(DirectionalLight* light, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Scene* scene, WaterManager* waterManager);
	static void RenderPassOmniShadow(PointLight* light, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Scene* scene, WaterManager* waterManager);
	static void RenderPassWaterReflection(WaterManager* waterManager, glm::mat4 projectionMatrix, Scene* scene, Camera* camera);
	static void RenderPassWaterRefraction(WaterManager* waterManager, glm::mat4 projectionMatrix, Scene* scene, Camera* camera);
	static void RenderEnvironmentCubemap(Window& mainWindow);
	static void RenderSimpleSkyboxJoey(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	static void EnableCulling();
	static void DisableCulling();

	static void Cleanup();

private:
	static std::map<std::string, Shader*> shaders;
	static std::map<std::string, GLint> uniforms;

	static glm::vec4 bgColor;

	static Cubemap* m_EnvironmentCubemap;
	static Cube* m_Cube1x1;
};
