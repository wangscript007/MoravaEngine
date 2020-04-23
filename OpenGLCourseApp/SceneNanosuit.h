#pragma once

#include "Scene.h"

#include "LearnOpenGL/ModelJoey.h"


class SceneNanosuit : public Scene
{

public:
	SceneNanosuit();
	virtual void Update(float timestep, Window& mainWindow) override;
	virtual void UpdateImGui(float timestep, Window& mainWindow, std::map<const char*, float> profilerResults) override;
	virtual void Render(glm::mat4 projectionMatrix, std::string passType,
		std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms) override;
	inline std::map<std::string, ModelJoey*> GetModels() const { return models; };
	virtual ~SceneNanosuit() override;

private:
	virtual void SetSkybox() override;
	virtual void SetTextures() override;
	virtual void SetupMeshes() override;
	virtual void SetupModels() override;

	std::map<std::string, ModelJoey*> models;
};
