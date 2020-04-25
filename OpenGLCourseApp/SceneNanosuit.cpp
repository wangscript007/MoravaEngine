#include "SceneNanosuit.h"

#include "ImGuiWrapper.h"

#include "RendererNanosuit.h"
#include "LearnOpenGL/ModelJoey.h"
#include "Sphere.h"


SceneNanosuit::SceneNanosuit()
{
	sceneSettings.cameraPosition = glm::vec3(0.0f, 10.5f, 10.0f);
	sceneSettings.cameraStartYaw = -90.0f;
	sceneSettings.cameraMoveSpeed = 1.0f;
	sceneSettings.ambientIntensity = 0.4f;
	sceneSettings.diffuseIntensity = 0.8f;
	sceneSettings.lightDirection = glm::vec3(3.0f, -9.0f, -3.0f);

	SetCamera();
	SetSkybox();
	SetTextures();
	SetupMeshes();
	SetupModels();

	m_LightOnCamera = true;
	m_IsRotating = true;
	m_RotationSpeed = 10.0f;
	m_DefaultNanosuitUniforms = false;
	m_LightSourceVisible = false;
	m_BgColor = glm::vec3(0.0f, 0.0f, 0.0f);

	InitNanosuitUniforms();
}

void SceneNanosuit::InitNanosuitUniforms()
{
	if (nanosuitUniforms == nullptr)
		nanosuitUniforms = new NanosuitUniforms;

	nanosuitUniforms->viewPos = m_Camera->GetPosition();
	nanosuitUniforms->enableNormalMap = true;

	nanosuitUniforms->material.diffuse =   0; // sampler2D / Texture slot
	nanosuitUniforms->material.specular =  1; // sampler2D / Texture slot
	nanosuitUniforms->material.normalMap = 2; // sampler2D / Texture slot
	nanosuitUniforms->material.shininess = 32.0f;

	nanosuitUniforms->light.position = m_Camera->GetPosition();
	nanosuitUniforms->light.direction = m_Camera->GetFront();
	nanosuitUniforms->light.cutOff = glm::cos(glm::radians(12.5f));
	nanosuitUniforms->light.outerCutOff = glm::cos(glm::radians(17.5f));

	nanosuitUniforms->light.ambient = glm::vec3(2.0f, 2.0f, 2.0f);
	nanosuitUniforms->light.diffuse = glm::vec3(3.0f, 3.0f, 3.0f);
	nanosuitUniforms->light.specular = glm::vec3(2.0f, 2.0f, 2.0f);

	nanosuitUniforms->light.constant = 1.0f;
	nanosuitUniforms->light.linear = 0.09f;
	nanosuitUniforms->light.quadratic = 0.032f;
}

void SceneNanosuit::SetSkybox()
{
}

void SceneNanosuit::SetTextures()
{
	textures.insert(std::make_pair("plain", new Texture("Textures/plain.png")));
}

void SceneNanosuit::SetupMeshes()
{
}

void SceneNanosuit::SetupModels()
{
	ModelJoey* nanosuit = new ModelJoey("Models/nanosuit.obj", "Textures/nanosuit");
	models.insert(std::make_pair("nanosuit", nanosuit));

	Sphere* sphere = new Sphere();
	sphere->Create();
	meshes.insert(std::make_pair("sphere", sphere));
}

void SceneNanosuit::Update(float timestep, Window& mainWindow)
{
}

void SceneNanosuit::UpdateImGui(float timestep, Window& mainWindow, std::map<const char*, float> profilerResults)
{
	ImGui::Begin("Nanosuit Shader Parameters:");

	ImGui::Checkbox("Set Defaults", &m_DefaultNanosuitUniforms);
	ImGui::Checkbox("Enable Normal Map", &nanosuitUniforms->enableNormalMap);
	ImGui::Checkbox("Light on Camera", &m_LightOnCamera);
	ImGui::Checkbox("Light Source Visible", &m_LightSourceVisible);
	ImGui::Checkbox("Is Rotating", &m_IsRotating);
	ImGui::SliderFloat("Rotating Speed", &m_RotationSpeed, -500.0f, 500.0f);
	ImGui::ColorEdit3("Background Color", glm::value_ptr(m_BgColor));
	ImGui::Separator();
	ImGui::Separator();
	ImGui::SliderFloat3("ViewPos", glm::value_ptr(nanosuitUniforms->viewPos), -100.0f, 100.0f);
	ImGui::Separator();
	ImGui::Separator();
	ImGui::SliderInt("Material.diffuse",      &nanosuitUniforms->material.diffuse,   0, 3);
	ImGui::SliderInt("Material.specular",     &nanosuitUniforms->material.specular,  0, 3);
	ImGui::SliderInt("Material.normalMap",    &nanosuitUniforms->material.normalMap, 0, 3);
	ImGui::SliderFloat("Material.shininess",  &nanosuitUniforms->material.shininess, 0, 512);
	ImGui::Separator();
	ImGui::Separator();
	ImGui::SliderFloat3("Light.position",   glm::value_ptr(nanosuitUniforms->light.position),  -20.0f, 20.0f);
	ImGui::SliderFloat3("Light.direction",  glm::value_ptr(nanosuitUniforms->light.direction), -1.0f, 1.0f);
	ImGui::SliderFloat("Light.cutOff",      &nanosuitUniforms->light.cutOff,      -1.0f, 1.0f);
	ImGui::SliderFloat("Light.outerCutOff", &nanosuitUniforms->light.outerCutOff, -1.0f, 1.0f);
	ImGui::Separator();
	ImGui::ColorEdit3("Light.ambient", glm::value_ptr(nanosuitUniforms->light.ambient));
	ImGui::ColorEdit3("Light.diffuse", glm::value_ptr(nanosuitUniforms->light.diffuse));
	ImGui::ColorEdit3("Light.specular", glm::value_ptr(nanosuitUniforms->light.specular));
	ImGui::Separator();
	ImGui::SliderFloat("Light.constant",  &nanosuitUniforms->light.constant,  -10.0f, 10.0f);
	ImGui::SliderFloat("Light.linear",    &nanosuitUniforms->light.linear,    -1.0f, 10.0f);
	ImGui::SliderFloat("Light.quadratic", &nanosuitUniforms->light.quadratic, 0.0f, 1.0f);

	ImGui::End();
}

void SceneNanosuit::Render(glm::mat4 projectionMatrix, std::string passType,
	std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms)
{
}

SceneNanosuit::~SceneNanosuit()
{
	for (auto& model : models)
		delete model.second;

	models.clear();

	delete nanosuitUniforms;
}
