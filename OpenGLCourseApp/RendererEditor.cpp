#include "RendererEditor.h"

#include "SceneParticles.h"
#include "MousePicker.h"
#include "GeometryFactory.h"

#include <stdexcept>


RendererEditor::RendererEditor()
{
}

void RendererEditor::Init(Scene* scene)
{
	SetUniforms();
	SetShaders();
}

void RendererEditor::SetUniforms()
{
}

void RendererEditor::SetShaders()
{
    Shader* shaderEditorObject = new Shader("Shaders/editor_object.vs", "Shaders/editor_object.fs");
    shaders.insert(std::make_pair("editor_object", shaderEditorObject));
    printf("RendererEditor: shaderEditorObject compiled [programID=%d]\n", shaderEditorObject->GetProgramID());

    Shader* shaderEditorObjectPBR = new Shader("Shaders/editor_object.vs", "Shaders/PBR/editor_object_pbr.fs");
    shaders.insert(std::make_pair("editor_object_pbr", shaderEditorObjectPBR));
    printf("RendererEditor: shaderEditorObjectPBR compiled [programID=%d]\n", shaderEditorObjectPBR->GetProgramID());

    Shader* shaderBasic = new Shader("Shaders/basic.vs", "Shaders/basic.fs");
    shaders.insert(std::make_pair("basic", shaderBasic));
    printf("RendererEditor: shaderBasic compiled [programID=%d]\n", shaderBasic->GetProgramID());

    Shader* shaderFramebuffersScene = new Shader("Shaders/framebuffers_scene.vs", "Shaders/framebuffers_scene.fs");
    shaders.insert(std::make_pair("framebuffers_scene", shaderFramebuffersScene));
    printf("RendererEditor: shaderFramebuffersScene compiled [programID=%d]\n", shaderFramebuffersScene->GetProgramID());

    Shader* shaderBackground = new Shader("Shaders/learnopengl/2.2.2.background.vs", "Shaders/learnopengl/2.2.2.background.fs");
    shaders.insert(std::make_pair("background", shaderBackground));
    printf("RendererEditor: shaderBackground compiled [programID=%d]\n", shaderBackground->GetProgramID());

    shaderEditorObjectPBR->Bind();
    shaderEditorObjectPBR->setInt("irradianceMap", 0);
    shaderEditorObjectPBR->setInt("prefilterMap",  1);
    shaderEditorObjectPBR->setInt("brdfLUT",       2);
    shaderEditorObjectPBR->setInt("albedoMap",     3);
    shaderEditorObjectPBR->setInt("normalMap",     4);
    shaderEditorObjectPBR->setInt("metallicMap",   5);
    shaderEditorObjectPBR->setInt("roughnessMap",  6);
    shaderEditorObjectPBR->setInt("aoMap",         7);
}

void RendererEditor::Render(float deltaTime, Window& mainWindow, Scene* scene, glm::mat4 projectionMatrix)
{
	RenderPass(mainWindow, scene, projectionMatrix);
}

void RendererEditor::RenderPass(Window& mainWindow, Scene* scene, glm::mat4 projectionMatrix)
{
	glViewport(0, 0, (GLsizei)mainWindow.GetBufferWidth(), (GLsizei)mainWindow.GetBufferHeight());

	// Clear the window
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    // set depth function to less than AND equal for skybox depth trick.
    glDepthFunc(GL_LEQUAL);
    // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    SetDefaultFramebuffer((unsigned int)mainWindow.GetBufferWidth(), (unsigned int)mainWindow.GetBufferHeight());

    // Override the Projection matrix (update FOV)
    if (mainWindow.GetBufferWidth() > 0 && mainWindow.GetBufferHeight() > 0)
    {
        projectionMatrix = glm::perspective(glm::radians(scene->GetFOV()),
            (float)mainWindow.GetBufferWidth() / (float)mainWindow.GetBufferHeight(),
            scene->GetSettings().nearPlane, scene->GetSettings().farPlane);
    }

    EnableTransparency();
    EnableCulling();

    /**** Begin editor_object ****/
    Shader* shaderEditor = shaders["editor_object"];

    shaderEditor->Bind();
    shaderEditor->setMat4("projection", projectionMatrix);
    shaderEditor->setMat4("view", scene->GetCamera()->CalculateViewMatrix());

    // Directional Light
    shaderEditor->setBool( "directionalLight.base.enabled", scene->GetLightManager()->directionalLight.GetEnabled());
    shaderEditor->setVec3( "directionalLight.base.color", scene->GetLightManager()->directionalLight.GetColor());
    shaderEditor->setFloat("directionalLight.base.ambientIntensity", scene->GetLightManager()->directionalLight.GetAmbientIntensity());
    shaderEditor->setFloat("directionalLight.base.diffuseIntensity", scene->GetLightManager()->directionalLight.GetDiffuseIntensity());
    shaderEditor->setVec3( "directionalLight.direction", scene->GetLightManager()->directionalLight.GetDirection());

    // Point Lights
    for (unsigned int i = 0; i < scene->GetLightManager()->pointLightCount; i++)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.enabled", i);
        // printf("PointLight[%d] enabled: %d\n", i, scene->GetLightManager()->pointLights[i].GetEnabled());
        shaderEditor->setBool(locBuff, scene->GetLightManager()->pointLights[i].GetEnabled());

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        shaderEditor->setVec3(locBuff, scene->GetLightManager()->pointLights[i].GetColor());

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->pointLights[i].GetAmbientIntensity());

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->pointLights[i].GetDiffuseIntensity());

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        shaderEditor->setVec3(locBuff, scene->GetLightManager()->pointLights[i].GetPosition());

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->pointLights[i].GetConstant());

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->pointLights[i].GetLinear());

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->pointLights[i].GetExponent());
    }

    shaderEditor->setInt("pointLightCount", scene->GetLightManager()->pointLightCount);

    // Spot Lights
    for (unsigned int i = 0; i < scene->GetLightManager()->spotLightCount; i++)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.enabled", i);
        shaderEditor->setBool(locBuff, scene->GetLightManager()->spotLights[i].GetBasePL()->GetEnabled());

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
        shaderEditor->setVec3(locBuff, scene->GetLightManager()->spotLights[i].GetBasePL()->GetColor());

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->spotLights[i].GetBasePL()->GetAmbientIntensity());

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->spotLights[i].GetBasePL()->GetDiffuseIntensity());

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
        shaderEditor->setVec3(locBuff, scene->GetLightManager()->spotLights[i].GetBasePL()->GetPosition());

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->spotLights[i].GetBasePL()->GetConstant());

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->spotLights[i].GetBasePL()->GetLinear());

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->spotLights[i].GetBasePL()->GetExponent());

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        shaderEditor->setVec3(locBuff, scene->GetLightManager()->spotLights[i].GetDirection());

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        shaderEditor->setFloat(locBuff, scene->GetLightManager()->spotLights[i].GetEdge());
    }

    shaderEditor->setInt("spotLightCount", scene->GetLightManager()->spotLightCount);

    // Eye position / camera direction
    shaderEditor->setVec3("eyePosition", scene->GetCamera()->GetPosition());
    /**** End editor_object ****/

    /**** Begin editor_object_pbr ****/
    Shader* shaderEditorPBR = shaders["editor_object_pbr"];

    // Init shaderEditorPBR
    // initialize static shader uniforms before rendering
    shaderEditorPBR->Bind();

    shaderEditorPBR->setMat4("projection", projectionMatrix);
    shaderEditorPBR->setMat4("view", scene->GetCamera()->CalculateViewMatrix());
    shaderEditorPBR->setVec3("cameraPosition", scene->GetCamera()->GetPosition());

    // directional light
    shaderEditorPBR->setBool( "directionalLight.base.enabled", scene->GetLightManager()->directionalLight.GetEnabled());
    shaderEditorPBR->setVec3( "directionalLight.base.color", scene->GetLightManager()->directionalLight.GetColor());
    shaderEditorPBR->setFloat("directionalLight.base.ambientIntensity", scene->GetLightManager()->directionalLight.GetAmbientIntensity());
    shaderEditorPBR->setFloat("directionalLight.base.diffuseIntensity", scene->GetLightManager()->directionalLight.GetDiffuseIntensity());
    shaderEditorPBR->setVec3( "directionalLight.direction", scene->GetLightManager()->directionalLight.GetDirection());

    // printf("Exponent = %.2ff Linear = %.2ff Constant = %.2ff\n", *m_PointLightExponent, *m_PointLightLinear, *m_PointLightConstant);

    // point lights
    unsigned int lightIndex = 0;
    for (unsigned int i = 0; i < scene->GetLightManager()->pointLightCount; ++i)
    {
        lightIndex = 0 + i; // offset for point lights
        shaderEditorPBR->setBool( "pointSpotLights[" + std::to_string(lightIndex) + "].enabled", scene->GetLightManager()->pointLights[i].GetEnabled());
        shaderEditorPBR->setVec3( "pointSpotLights[" + std::to_string(lightIndex) + "].position", scene->GetLightManager()->pointLights[i].GetPosition());
        shaderEditorPBR->setVec3( "pointSpotLights[" + std::to_string(lightIndex) + "].color", scene->GetLightManager()->pointLights[i].GetColor());
        shaderEditorPBR->setFloat("pointSpotLights[" + std::to_string(lightIndex) + "].exponent", scene->GetLightManager()->pointLights[i].GetExponent());
        shaderEditorPBR->setFloat("pointSpotLights[" + std::to_string(lightIndex) + "].linear", scene->GetLightManager()->pointLights[i].GetLinear());
        shaderEditorPBR->setFloat("pointSpotLights[" + std::to_string(lightIndex) + "].constant", scene->GetLightManager()->pointLights[i].GetConstant());
    }

    for (unsigned int i = 0; i < scene->GetLightManager()->spotLightCount; ++i)
    {
        lightIndex = 4 + i; // offset for point lights
        shaderEditorPBR->setBool( "pointSpotLights[" + std::to_string(lightIndex) + "].enabled", scene->GetLightManager()->spotLights[i].GetBasePL()->GetEnabled());
        shaderEditorPBR->setVec3( "pointSpotLights[" + std::to_string(lightIndex) + "].position", scene->GetLightManager()->spotLights[i].GetBasePL()->GetPosition());
        shaderEditorPBR->setVec3( "pointSpotLights[" + std::to_string(lightIndex) + "].color", scene->GetLightManager()->spotLights[i].GetBasePL()->GetColor());
        shaderEditorPBR->setFloat("pointSpotLights[" + std::to_string(lightIndex) + "].exponent", scene->GetLightManager()->spotLights[i].GetBasePL()->GetExponent());
        shaderEditorPBR->setFloat("pointSpotLights[" + std::to_string(lightIndex) + "].linear", scene->GetLightManager()->spotLights[i].GetBasePL()->GetLinear());
        shaderEditorPBR->setFloat("pointSpotLights[" + std::to_string(lightIndex) + "].constant", scene->GetLightManager()->spotLights[i].GetBasePL()->GetConstant());
    }

    /**** End editor_object_pbr ****/

    if (scene->GetSettings().enableSkybox)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        float angleRadians = glm::radians((GLfloat)glfwGetTime());
        modelMatrix = glm::rotate(modelMatrix, angleRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        scene->GetSkybox()->Draw(modelMatrix, scene->GetCamera()->CalculateViewMatrix(), projectionMatrix);
    }

    std::string passType = "main";
    scene->Render(projectionMatrix, passType, shaders, uniforms);
}

RendererEditor::~RendererEditor()
{
}
