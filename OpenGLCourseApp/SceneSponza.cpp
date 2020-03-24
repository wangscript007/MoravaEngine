#include "SceneSponza.h"

#include "ImGuiWrapper.h"

#include "Renderer.h"


SceneSponza::SceneSponza()
{
	sceneSettings.enableShadows      = true;
	sceneSettings.enableOmniShadows  = true;
	sceneSettings.enablePointLights  = true;
	sceneSettings.enableSpotLights   = true;
	sceneSettings.enableWaterEffects = true;
	sceneSettings.enableSkybox       = true;
	sceneSettings.enableNormalMaps   = true;
	sceneSettings.cameraPosition = glm::vec3(-4.0f, 10.0f, -0.5f);
	sceneSettings.cameraStartYaw = 0.0f;
	sceneSettings.cameraMoveSpeed = 1.0f;
	sceneSettings.ambientIntensity = 0.2f;
	sceneSettings.diffuseIntensity = 1.0f;
	sceneSettings.lightDirection = glm::vec3(0.10f, -0.16f, 0.38f);
	sceneSettings.lightProjectionMatrix = glm::ortho(-36.0f, 36.0f, -36.0f, 36.0f, 0.1f, 36.0f);
	sceneSettings.pLight_0_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.pLight_0_position = glm::vec3(0.0f, 20.0f, 0.0f);
	sceneSettings.pLight_0_diffuseIntensity = 2.0f;
	sceneSettings.pLight_1_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.pLight_1_position = glm::vec3(8.92f, 2.75f, -0.85f);
	sceneSettings.pLight_1_diffuseIntensity = 2.0f;
	sceneSettings.pLight_2_color = glm::vec3(0.0f, 0.0f, 1.0f);
	sceneSettings.pLight_2_position = glm::vec3(10.0f, 2.0f, 10.0f);
	sceneSettings.pLight_2_diffuseIntensity = 2.0f;
	sceneSettings.shadowMapWidth = 2048;
	sceneSettings.shadowMapHeight = 2048;
	sceneSettings.shadowSpeed = 0.1f;
	sceneSettings.waterHeight = 0.2f;
	sceneSettings.waterWaveSpeed = 0.02f;

	SetSkybox();
	SetTextures();
	SetupModels();
}

void SceneSponza::SetSkybox()
{
	skyboxFaces.push_back("Textures/skybox_4/right.png");
	skyboxFaces.push_back("Textures/skybox_4/left.png");
	skyboxFaces.push_back("Textures/skybox_4/top.png");
	skyboxFaces.push_back("Textures/skybox_4/bottom.png");
	skyboxFaces.push_back("Textures/skybox_4/back.png");
	skyboxFaces.push_back("Textures/skybox_4/front.png");
	m_Skybox = new Skybox(skyboxFaces);
}

void SceneSponza::SetTextures()
{
}

void SceneSponza::SetupModels()
{
	Model* sponza = new Model();
	sponza->LoadModel("Models/sponza.obj");
	models.insert(std::make_pair("sponza", sponza));
}

void SceneSponza::Update(float timestep)
{
	// Shadow rotation
	glm::vec3 lightDirection = m_LightManager->directionalLight.GetDirection();
	glm::vec3 lightColor = m_LightManager->directionalLight.GetColor();
	float lightRadius = abs(lightDirection.x);
	float lightAngle = timestep * sceneSettings.shadowSpeed;
	lightDirection.x = (float)cos(lightAngle) * lightRadius;
	lightDirection.z = (float)sin(lightAngle) * lightRadius;
	m_LightManager->directionalLight.SetDirection(lightDirection);

	glm::vec3 PL0_Position = m_LightManager->pointLights[0].GetPosition();
	glm::vec3 PL0_Color = m_LightManager->pointLights[0].GetColor();
	float PL0_AmbIntensity = m_LightManager->pointLights[0].GetAmbientIntensity();
	float PL0_DiffIntensity = m_LightManager->pointLights[0].GetDiffuseIntensity();
	glm::vec3 PL1_Position = m_LightManager->pointLights[1].GetPosition();
	glm::vec3 PL1_Color = m_LightManager->pointLights[1].GetColor();
	float PL1_AmbIntensity = m_LightManager->pointLights[1].GetAmbientIntensity();
	float PL1_DiffIntensity = m_LightManager->pointLights[1].GetDiffuseIntensity();
	glm::vec3 PL2_Position = m_LightManager->pointLights[2].GetPosition();
	glm::vec3 PL2_Color = m_LightManager->pointLights[2].GetColor();
	float PL2_AmbIntensity = m_LightManager->pointLights[2].GetAmbientIntensity();
	float PL2_DiffIntensity = m_LightManager->pointLights[2].GetDiffuseIntensity();

	ImGui::SliderFloat("Water level",         &sceneSettings.waterHeight, 0.0f, 20.0f);

	ImGui::ColorEdit3("DirLight Color",       glm::value_ptr(lightColor));
	ImGui::SliderFloat3("DirLight Direction", glm::value_ptr(lightDirection), -100.0f, 100.0f);

	ImGui::ColorEdit3("PL0 Color",            glm::value_ptr(PL0_Color));
	ImGui::SliderFloat3("PL0 Position",       glm::value_ptr(PL0_Position), -20.0f, 20.0f);
	ImGui::SliderFloat("PL0 Amb Intensity",   &PL0_AmbIntensity, -20.0f, 20.0f);
	ImGui::SliderFloat("PL0 Diff Intensity",  &PL0_DiffIntensity, -20.0f, 20.0f);

	ImGui::ColorEdit3("PL1 Color",            glm::value_ptr(PL1_Color));
	ImGui::SliderFloat3("PL1 Position",       glm::value_ptr(PL1_Position), -20.0f, 20.0f);
	ImGui::SliderFloat("PL1 Amb Intensity",   &PL1_AmbIntensity, -20.0f, 20.0f);
	ImGui::SliderFloat("PL1 Diff Intensity",  &PL1_DiffIntensity, -20.0f, 20.0f);

	ImGui::ColorEdit3("PL2 Color",            glm::value_ptr(PL2_Color));
	ImGui::SliderFloat3("PL2 Position",       glm::value_ptr(PL2_Position), -20.0f, 20.0f);
	ImGui::SliderFloat("PL2 Amb Intensity",   &PL2_AmbIntensity, -20.0f, 20.0f);
	ImGui::SliderFloat("PL2 Diff Intensity",  &PL2_DiffIntensity, -20.0f, 20.0f);

	m_WaterManager->SetWaterHeight(sceneSettings.waterHeight);
	m_LightManager->directionalLight.SetDirection(lightDirection);
	m_LightManager->directionalLight.SetColor(lightColor);
	m_LightManager->pointLights[0].SetPosition(PL0_Position);
	m_LightManager->pointLights[0].SetColor(PL0_Color);
	m_LightManager->pointLights[0].SetAmbientIntensity(PL0_AmbIntensity);
	m_LightManager->pointLights[0].SetDiffuseIntensity(PL0_DiffIntensity);
	m_LightManager->pointLights[1].SetPosition(PL1_Position);
	m_LightManager->pointLights[1].SetColor(PL1_Color);
	m_LightManager->pointLights[1].SetAmbientIntensity(PL1_AmbIntensity);
	m_LightManager->pointLights[1].SetDiffuseIntensity(PL1_DiffIntensity);
	m_LightManager->pointLights[2].SetPosition(PL2_Position);
	m_LightManager->pointLights[2].SetColor(PL2_Color);
	m_LightManager->pointLights[2].SetAmbientIntensity(PL2_AmbIntensity);
	m_LightManager->pointLights[2].SetDiffuseIntensity(PL2_DiffIntensity);
}

void SceneSponza::Render(glm::mat4 projectionMatrix, std::string passType,
	std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms)
{
	/* Sponza scene */
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.008f));
	glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
	materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
	models["sponza"]->RenderModel(textureSlots["diffuse"], textureSlots["normal"], sceneSettings.enableNormalMaps);

	if (passType == "main")
	{
		/* ShadowMap display */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 16.0f, -0.25f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));

		shaders["main"]->setInt("theTexture", textureSlots["shadow"]);
		shaders["main"]->setInt("normalMap", textureSlots["shadow"]);

		materials["dull"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
		meshes["quad"]->Render();
	}
}

void SceneSponza::RenderWater(glm::mat4 projectionMatrix, std::string passType,
	std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms)
{
	if (!sceneSettings.enableWaterEffects) return;

	Shader* shaderWater = shaders["water"];

	/* Water Tile */
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, m_WaterManager->GetWaterHeight(), 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(12.0f, 0.0f, 8.0f));

	shaderWater->Bind();
	shaderWater->setMat4("model", model);

	m_WaterManager->GetReflectionFramebuffer()->GetColorAttachment()->Bind(textureSlots["reflection"]);
	m_WaterManager->GetRefractionFramebuffer()->GetColorAttachment()->Bind(textureSlots["refraction"]);
	m_WaterManager->GetRefractionFramebuffer()->GetDepthAttachment()->Bind(textureSlots["depth"]);

	textures["normalMapDefault"]->Bind(textureSlots["normal"]);
	textures["waterDuDv"]->Bind(textureSlots["DuDv"]);
	shaderWater->setInt("reflectionTexture", textureSlots["reflection"]);
	shaderWater->setVec3("lightColor", LightManager::directionalLight.GetColor());
	materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
	meshes["water"]->Render();
}

SceneSponza::~SceneSponza()
{
}
