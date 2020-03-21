#include "ScenePBR.h"
#include "Terrain.h"
#include "ShaderMain.h"
#include "ShaderPBR.h"
#include "Renderer.h"
#include "Sphere.h"
#include "ImGuiWrapper.h"


ScenePBR::ScenePBR()
{
	sceneSettings.enableShadows = true;
	sceneSettings.enableOmniShadows = false;
	sceneSettings.enablePointLights = true;
	sceneSettings.enableSpotLights = true;
	sceneSettings.enableWaterEffects = false;
	sceneSettings.enableSkybox = true;
	sceneSettings.enableNormalMaps = true;
	sceneSettings.cameraPosition = glm::vec3(0.0f, 10.0f, 15.0f);
	sceneSettings.cameraStartYaw = -90.0f;
	sceneSettings.cameraMoveSpeed = 1.0f;
	sceneSettings.nearPlane = 0.01f;
	sceneSettings.farPlane = 400.0f;
	sceneSettings.ambientIntensity = 0.02f;
	sceneSettings.diffuseIntensity = 0.8f;
	sceneSettings.lightDirection = glm::vec3(1.2f, -14.0f, 1.2f);
	sceneSettings.lightProjectionMatrix = glm::ortho(-36.0f, 36.0f, -36.0f, 36.0f, 0.1f, 36.0f);
	sceneSettings.pLight_0_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.pLight_0_position = glm::vec3(0.0f, 20.0f, 0.0f);
	sceneSettings.pLight_0_diffuseIntensity = 0.0f;
	sceneSettings.pLight_1_color = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneSettings.pLight_1_position = glm::vec3(8.92f, 2.75f, -0.85f);
	sceneSettings.pLight_1_diffuseIntensity = 0.0f;
	sceneSettings.pLight_2_color = glm::vec3(0.0f, 0.0f, 1.0f);
	sceneSettings.pLight_2_position = glm::vec3(10.0f, 2.0f, 10.0f);
	sceneSettings.pLight_2_diffuseIntensity = 0.0f;
	sceneSettings.sLight_2_color = glm::vec3(0.0f, 1.0f, 1.0f);
	sceneSettings.sLight_2_position = glm::vec3(0.0f, 0.0f, 0.0f);
	sceneSettings.sLight_2_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	sceneSettings.sLight_2_ambientIntensity = 0.4f;
	sceneSettings.sLight_2_diffuseIntensity = 4.0f;
	sceneSettings.shadowMapWidth = 2048;
	sceneSettings.shadowMapHeight = 2048;
	sceneSettings.shadowSpeed = 0.1f;
	sceneSettings.waterHeight = 0.0f; // 1.0f 5.0f
	sceneSettings.waterWaveSpeed = 0.1f;

	SetSkybox();
	SetTextures();
	SetupModels();

	m_CameraPosition = glm::vec3(-9.0f, 24.0f, 6.0f);

	m_Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Metallic = 0.8f;
	m_Roughness = 0.2f;
	m_AmbientOcclusion = 1.0f;
	m_AmbientIntensity = 0.5f;

	m_LightColor_0 = glm::vec3(1.0f, 1.0f, 1.0f);
	m_LightColor_1 = glm::vec3(1.0f, 1.0f, 1.0f);
	m_LightColor_2 = glm::vec3(1.0f, 1.0f, 1.0f);
	m_LightColor_3 = glm::vec3(1.0f, 1.0f, 1.0f);

	m_LightPosOffset_0 = glm::vec3(-1.0f, 0.0f, 0.0f);
	m_LightPosOffset_1 = glm::vec3(1.0f, 0.0f, 0.0f);
	m_LightPosOffset_2 = glm::vec3(0.0f, -1.0f, 0.0f);
	m_LightPosOffset_3 = glm::vec3(0.0f, 1.0f, 0.0f);

	m_LightPositions[0] = m_CameraPosition + m_LightPosOffset_0;
	m_LightPositions[1] = m_CameraPosition + m_LightPosOffset_1;
	m_LightPositions[2] = m_CameraPosition + m_LightPosOffset_2;
	m_LightPositions[3] = m_CameraPosition + m_LightPosOffset_3;

	m_LightColors[0] = m_LightColor_0 * 255.0f;
	m_LightColors[1] = m_LightColor_1 * 255.0f;
	m_LightColors[2] = m_LightColor_2 * 255.0f;
	m_LightColors[3] = m_LightColor_3 * 255.0f;
}

void ScenePBR::SetSkybox()
{
	skyboxFaces.push_back("Textures/skybox_4/right.png");
	skyboxFaces.push_back("Textures/skybox_4/left.png");
	skyboxFaces.push_back("Textures/skybox_4/top.png");
	skyboxFaces.push_back("Textures/skybox_4/bottom.png");
	skyboxFaces.push_back("Textures/skybox_4/back.png");
	skyboxFaces.push_back("Textures/skybox_4/front.png");
	m_Skybox = new Skybox(skyboxFaces);
}

void ScenePBR::SetTextures()
{
	// PBR sphere
	textures.insert(std::make_pair("rustedIronAlbedo", new Texture("Textures/PBR/rusted_iron/albedo.png")));
	textures.insert(std::make_pair("rustedIronNormal", new Texture("Textures/PBR/rusted_iron/normal.png")));
	textures.insert(std::make_pair("rustedIronMetallic", new Texture("Textures/PBR/rusted_iron/metallic.png")));
	textures.insert(std::make_pair("rustedIronRoughness", new Texture("Textures/PBR/rusted_iron/roughness.png")));
	textures.insert(std::make_pair("rustedIronAmbientOcclusion", new Texture("Textures/PBR/rusted_iron/ao.png")));

	// PBR gold
	textures.insert(std::make_pair("goldAlbedo", new Texture("Textures/PBR/gold/albedo.png")));
	textures.insert(std::make_pair("goldNormal", new Texture("Textures/PBR/gold/normal.png")));
	textures.insert(std::make_pair("goldMetallic", new Texture("Textures/PBR/gold/metallic.png")));
	textures.insert(std::make_pair("goldRoughness", new Texture("Textures/PBR/gold/roughness.png")));
	textures.insert(std::make_pair("goldAmbientOcclusion", new Texture("Textures/PBR/gold/ao.png")));

	// PBR cerberus
	if (m_CerberusEnabled)
	{
		textures.insert(std::make_pair("cerberusAlbedo", new Texture("Textures/PBR/Cerberus/Cerberus_A.tga")));
		textures.insert(std::make_pair("cerberusNormal", new Texture("Textures/PBR/Cerberus/Cerberus_N.tga")));
		textures.insert(std::make_pair("cerberusMetallic", new Texture("Textures/PBR/Cerberus/Cerberus_M.tga")));
		textures.insert(std::make_pair("cerberusRoughness", new Texture("Textures/PBR/Cerberus/Cerberus_R.tga")));
		textures.insert(std::make_pair("cerberusAmbOcclusion", new Texture("Textures/PBR/Cerberus/Cerberus_AO.tga")));
	}
}

void ScenePBR::SetupModels()
{
	Sphere* sphere = new Sphere();
	sphere->CreateMesh();
	meshes.insert(std::make_pair("sphere", sphere));

	if (m_CerberusEnabled)
	{
		Model* cerberus = new Model();
		cerberus->LoadModel("Models/Cerberus_LP.FBX");
		models.insert(std::make_pair("cerberus", cerberus));
	}
}

void ScenePBR::Update(float timestep)
{
	m_CameraPosition = m_Camera->GetPosition();

	ImGui::ColorEdit3("Albedo", glm::value_ptr(m_Albedo));
	ImGui::SliderFloat("Metallic", &m_Metallic, 0.0f, 1.0f);
	ImGui::SliderFloat("Roughness", &m_Roughness, 0.0f, 1.0f);
	ImGui::SliderFloat("Ambient Occlusion", &m_AmbientOcclusion, 0.0f, 5.0f);

	ImGui::SliderFloat("Ambient Intensity", &m_AmbientIntensity, 0.0f, 1.0f);

	ImGui::ColorEdit3("Light Color 0", glm::value_ptr(m_LightColor_0));
	ImGui::SliderFloat3("Light Pos Offset 0", glm::value_ptr(m_LightPosOffset_0), -60.0f, 60.0f);

	ImGui::ColorEdit3("Light Color 1", glm::value_ptr(m_LightColor_1));
	ImGui::SliderFloat3("Light Pos Offset 1", glm::value_ptr(m_LightPosOffset_1), -60.0f, 60.0f);

	ImGui::ColorEdit3("Light Color 2", glm::value_ptr(m_LightColor_2));
	ImGui::SliderFloat3("Light Pos Offset 2", glm::value_ptr(m_LightPosOffset_2), -60.0f, 60.0f);

	ImGui::ColorEdit3("Light Color 3", glm::value_ptr(m_LightColor_3));
	ImGui::SliderFloat3("Light Pos Offset 3", glm::value_ptr(m_LightPosOffset_3), -60.0f, 60.0f);

	m_LightPositions[0] = m_CameraPosition + m_LightPosOffset_0;
	m_LightPositions[1] = m_CameraPosition + m_LightPosOffset_1;
	m_LightPositions[2] = m_CameraPosition + m_LightPosOffset_2;
	m_LightPositions[3] = m_CameraPosition + m_LightPosOffset_3;

	m_LightColors[0] = m_LightColor_0 * 255.0f;
	m_LightColors[1] = m_LightColor_1 * 255.0f;
	m_LightColors[2] = m_LightColor_2 * 255.0f;
	m_LightColors[3] = m_LightColor_3 * 255.0f;
}

void ScenePBR::Render(glm::mat4 projectionMatrix, std::string passType,
	std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms)
{
	ShaderPBR* shaderPBR = static_cast<ShaderPBR*>(shaders["pbr"]);

	shaderPBR->Bind();

	shaderPBR->SetLightPositions(m_LightPositions, 4);
	shaderPBR->SetLightColors(m_LightColors, 4);

	// Model matrix
	glm::mat4 model;

	for (int v = 0; v <= 5; ++v)
	{
		for (int h = 0; h <= 5; ++h)
		{
			float x = ((float)h - 2.0f) * 2.5f;
			float y = ((float)v - 2.0f) * 2.5f + 8.0f;
			float z = ((float)v - 2.0f) * 2.5f;

			/* Sphere model */
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(x, 6.0f, z));
			model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.0f));
			glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));

			shaderPBR->SetAlbedo(m_Albedo);
			shaderPBR->SetMetallic(((float)v / 5.0f + m_Metallic) / 2.0f);
			shaderPBR->SetRoughness(((float)h / 5.0f + m_Roughness) / 2.0f);
			shaderPBR->SetAmbientOcclusion(m_AmbientOcclusion);
			shaderPBR->SetAmbientIntensity(m_AmbientIntensity);

			if ((v + h) % 2 == 0)
			{
				textures["rustedIronAlbedo"]->Bind(textureSlots["albedo"]);
				textures["rustedIronNormal"]->Bind(textureSlots["normal"]);
				textures["rustedIronMetallic"]->Bind(textureSlots["metallic"]);
				textures["rustedIronRoughness"]->Bind(textureSlots["roughness"]);
				textures["rustedIronAmbientOcclusion"]->Bind(textureSlots["ao"]);
			}
			else
			{
				textures["goldAlbedo"]->Bind(textureSlots["albedo"]);
				textures["goldNormal"]->Bind(textureSlots["normal"]);
				textures["goldMetallic"]->Bind(textureSlots["metallic"]);
				textures["goldRoughness"]->Bind(textureSlots["roughness"]);
				textures["goldAmbientOcclusion"]->Bind(textureSlots["ao"]);
			}

			shaderPBR->SetAlbedoMap(textureSlots["albedo"]);
			shaderPBR->SetNormalMap(textureSlots["normal"]);
			shaderPBR->SetMetallicMap(textureSlots["metallic"]);
			shaderPBR->SetRoughnessMap(textureSlots["roughness"]);
			shaderPBR->SetAmbientOcclusionMap(textureSlots["ao"]);

			meshes["sphere"]->RenderMesh();

		}
	}

	/* Cerberus model */
	if (m_CerberusEnabled)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));

		shaderPBR->SetAlbedo(m_Albedo);
		shaderPBR->SetMetallic(m_Metallic);
		shaderPBR->SetRoughness(m_Roughness);
		shaderPBR->SetAmbientOcclusion(m_AmbientOcclusion);
		shaderPBR->SetAmbientIntensity(m_AmbientIntensity);

		textures["cerberusAlbedo"]->Bind(textureSlots["albedo"]);
		textures["cerberusNormal"]->Bind(textureSlots["normal"]);
		textures["cerberusMetallic"]->Bind(textureSlots["metallic"]);
		textures["cerberusRoughness"]->Bind(textureSlots["roughness"]);
		textures["cerberusAmbOcclusion"]->Bind(textureSlots["ao"]);

		shaderPBR->SetAlbedoMap(textureSlots["albedo"]);
		shaderPBR->SetNormalMap(textureSlots["normal"]);
		shaderPBR->SetMetallicMap(textureSlots["metallic"]);
		shaderPBR->SetRoughnessMap(textureSlots["roughness"]);
		shaderPBR->SetAmbientOcclusionMap(textureSlots["ao"]);

		models["cerberus"]->RenderModelPBR();
	}
}

void ScenePBR::RenderWater(glm::mat4 projectionMatrix, std::string passType,
	std::map<std::string, Shader*> shaders, std::map<std::string, GLint> uniforms)
{
	if (!sceneSettings.enableWaterEffects) return;

	Shader* shaderWater = shaders["water"];

	// Model matrix
	glm::mat4 model;

	/* Water Tile */
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, m_WaterManager->GetWaterHeight(), 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(32.0f, 1.0f, 32.0f));

	glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, glm::value_ptr(model));
	m_WaterManager->GetReflectionFramebuffer()->GetColorAttachment()->Bind(textureSlots["reflection"]);
	m_WaterManager->GetRefractionFramebuffer()->GetColorAttachment()->Bind(textureSlots["refraction"]);
	m_WaterManager->GetRefractionFramebuffer()->GetDepthAttachment()->Bind(textureSlots["depth"]);
	shaderWater->setInt("reflectionTexture", textureSlots["reflection"]);
	textures["normalMapDefault"]->Bind(textureSlots["normal"]);
	textures["waterDuDv"]->Bind(textureSlots["DuDv"]);
	shaderWater->setVec3("lightColor", LightManager::directionalLight.GetColor());
	materials["superShiny"]->UseMaterial(uniforms["specularIntensity"], uniforms["shininess"]);
	meshes["water"]->RenderMesh();
}

ScenePBR::~ScenePBR()
{
}
