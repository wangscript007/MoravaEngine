#include "RendererFramebuffers.h"

#include "SceneFramebuffers.h"
#include "GeometryFactory.h"

#include <stdexcept>


RendererFramebuffers::RendererFramebuffers()
{
}

void RendererFramebuffers::Init(Scene* scene)
{
	SetUniforms();
	SetShaders();
	SetFramebuffers();
}

void RendererFramebuffers::SetUniforms()
{
}

void RendererFramebuffers::SetShaders()
{
	Shader* shaderFramebuffersScene = new Shader("Shaders/framebuffers_scene.vs", "Shaders/framebuffers_scene.fs");
	shaders.insert(std::make_pair("framebuffers_scene", shaderFramebuffersScene));
	printf("RendererFramebuffers: shaderFramebuffersScene compiled [programID=%d]\n", shaderFramebuffersScene->GetProgramID());

	Shader* shaderFramebuffersScreen = new Shader("Shaders/framebuffers_screen.vs", "Shaders/framebuffers_screen.fs");
	shaders.insert(std::make_pair("framebuffers_screen", shaderFramebuffersScreen));
	printf("RendererFramebuffers: shaderFramebuffersScreen compiled [programID=%d]\n", shaderFramebuffersScreen->GetProgramID());

	// shader configuration
	shaders["framebuffers_scene"]->Bind();
	shaders["framebuffers_scene"]->setInt("texture1", 0);

	shaders["framebuffers_screen"]->Bind();
	shaders["framebuffers_screen"]->setInt("screenTexture", 0);
}

void RendererFramebuffers::SetFramebuffers()
{
	// Framebuffer configuration
	// -- create a framebuffer (FBO)
	m_Framebuffer = new Framebuffer(SCR_WIDTH, SCR_HEIGHT);

	// -- generate a color texture attachment
	m_Framebuffer->CreateTextureAttachmentColor(SCR_WIDTH, SCR_HEIGHT);

	// -- create a renderbuffer object (RBO) to be used as a depth and stencil attachment to the framebuffer
	//      (we won't be sampling these)
	m_Framebuffer->CreateAttachmentDepthAndStencil(SCR_WIDTH, SCR_HEIGHT, AttachmentType::Renderbuffer);

	if (!m_Framebuffer->CheckStatus())
		throw std::runtime_error("ERROR: Framebuffer is not complete!");

	std::cout << "Framebuffer created successfully." << std::endl;

	// -- Unbind the framebuffer / back to default framebuffer
	m_Framebuffer->Unbind(SCR_WIDTH, SCR_HEIGHT);
}

void RendererFramebuffers::Render(float deltaTime, Window& mainWindow, Scene* scene, glm::mat4 projectionMatrix)
{
	RenderPass(mainWindow, scene, projectionMatrix);
}

void RendererFramebuffers::RenderPass(Window& mainWindow, Scene* scene, glm::mat4 projectionMatrix)
{
	glViewport(0, 0, (GLsizei)mainWindow.GetBufferWidth(), (GLsizei)mainWindow.GetBufferHeight());

	// Clear the window
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		// First Render Pass
		m_Framebuffer->Bind(SCR_WIDTH, SCR_HEIGHT);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)


		// -- make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		m_Framebuffer->Clear(); // we're not using the stencil buffer now
	}

	{
		std::string passType = "main";
		scene->Render(mainWindow, projectionMatrix, passType, shaders, uniforms);
	}

	{
		// Second Render Pass
		// -- now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		m_Framebuffer->Unbind((GLsizei)mainWindow.GetBufferWidth(), (GLsizei)mainWindow.GetBufferHeight());

		glDisable(GL_DEPTH_TEST);

		// -- clear all relevant buffers
		// -- set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaders["framebuffers_screen"]->Bind();
		SceneFramebuffers* sceneFramebuffers = (SceneFramebuffers*)scene;
		glBindVertexArray(GeometryFactory::Quad::GetVAO());

		// -- use the color attachment texture as the texture of the quad plane
		m_Framebuffer->GetTextureAttachmentColor(0)->Bind();

		glm::mat4 model;
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		shaders["framebuffers_screen"]->setMat4("model", model);
		shaders["framebuffers_screen"]->setInt("effect", sceneFramebuffers->GetEffectForFrame(0)); // diffuse (default)
		glDrawArrays(GL_TRIANGLES, 0, 6);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		shaders["framebuffers_screen"]->setMat4("model", model);
		shaders["framebuffers_screen"]->setInt("effect", sceneFramebuffers->GetEffectForFrame(1)); // inverse color
		glDrawArrays(GL_TRIANGLES, 0, 6);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		shaders["framebuffers_screen"]->setMat4("model", model);
		shaders["framebuffers_screen"]->setInt("effect", sceneFramebuffers->GetEffectForFrame(2)); // nightvision
		glDrawArrays(GL_TRIANGLES, 0, 6);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		shaders["framebuffers_screen"]->setMat4("model", model);
		shaders["framebuffers_screen"]->setInt("effect", sceneFramebuffers->GetEffectForFrame(3)); // kernel sharp
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

RendererFramebuffers::~RendererFramebuffers()
{
	delete m_Framebuffer;
}
