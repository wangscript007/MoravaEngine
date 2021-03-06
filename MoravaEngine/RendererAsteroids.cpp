#include "RendererAsteroids.h"

#include "SceneAsteroids.h"
#include "ShaderInstanced.h"
#include "VertexInstanced.h"



RendererAsteroids::RendererAsteroids()
{
}

void RendererAsteroids::Init(Scene* scene)
{
	SetShaders();

	models = ((SceneAsteroids*)scene)->GetModels();

	modelMatrices = new glm::mat4[amount];
	srand((unsigned int)glfwGetTime()); // initialize random seed	
	float radius = 120.0;
	float offset = 25.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.2f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (float)(rand() % 20) / 100.0f + 0.05f;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (float)(rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	// configure instanced array
	// -------------------------
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < models["rock"]->GetMeshes().size(); i++)
	{
		unsigned int VAO = models["rock"]->GetMeshes()[i].GetVAO();
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(0 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RendererAsteroids::SetShaders()
{
	static const char* asteroidsVert = "Shaders/asteroids.vs";
	static const char* asteroidsFrag = "Shaders/asteroids.fs";
	ShaderInstanced* shaderAsteroids = new ShaderInstanced();
	shaderAsteroids->CreateFromFiles(asteroidsVert, asteroidsFrag);
	shaders.insert(std::make_pair("asteroids", shaderAsteroids));
	printf("Renderer: shaderAsteroids shader compiled [programID=%d]\n", shaderAsteroids->GetProgramID());

	static const char* planetVert = "Shaders/asteroids_planet.vs";
	static const char* planetFrag = "Shaders/asteroids.fs";
	ShaderInstanced* shaderPlanet = new ShaderInstanced();
	shaderPlanet->CreateFromFiles(planetVert, planetFrag);
	shaders.insert(std::make_pair("planet", shaderPlanet));
	printf("Renderer: shaderPlanet shader compiled [programID=%d]\n", shaderPlanet->GetProgramID());
}

void RendererAsteroids::Render(float deltaTime, Window& mainWindow, Scene* scene, glm::mat4 projectionMatrix)
{
	planetRotationY += 10.0f * deltaTime;

	RenderPass(mainWindow, scene, projectionMatrix);
}

void RendererAsteroids::RenderPass(Window& mainWindow, Scene* scene, glm::mat4 projectionMatrix)
{
	glViewport(0, 0, (GLsizei)mainWindow.GetBufferWidth(), (GLsizei)mainWindow.GetBufferHeight());

	// Clear the window
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Override the Projection matrix (update FOV)
	projectionMatrix = glm::perspective(glm::radians(((SceneAsteroids*)scene)->GetFOV()),
		(float)mainWindow.GetBufferWidth() / (float)mainWindow.GetBufferHeight(),
		scene->GetSettings().nearPlane, scene->GetSettings().farPlane);

	if (scene->GetSettings().enableSkybox)
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		float angleRadians = glm::radians((GLfloat)glfwGetTime());
		modelMatrix = glm::rotate(modelMatrix, angleRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		scene->GetSkybox()->Draw(modelMatrix, scene->GetCameraController()->CalculateViewMatrix(), projectionMatrix);
	}

	glm::mat4 view = scene->GetCameraController()->CalculateViewMatrix();

	// configure transformation matrices
	shaders["planet"]->Bind();
	shaders["planet"]->setMat4("projection", projectionMatrix);
	shaders["planet"]->setMat4("view", view);

	// draw planet
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -20.0f, 0.0f));
	model = glm::rotate(model, glm::radians(planetRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f));
	shaders["planet"]->setMat4("model", model);
	models["planet"]->Draw(shaders["planet"]);

	// draw meteorites
	shaders["asteroids"]->Bind();
	shaders["asteroids"]->setMat4("projection", projectionMatrix);
	shaders["asteroids"]->setMat4("view", view);
	shaders["asteroids"]->setInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, models["rock"]->GetTextures()[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
	for (unsigned int i = 0; i < models["rock"]->GetMeshes().size(); i++)
	{
		glBindVertexArray(models["rock"]->GetMeshes()[i].GetVAO());
		glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)models["rock"]->GetMeshes()[i].GetIndices().size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}

	std::string passType = "main";
	scene->Render(mainWindow, projectionMatrix, passType, shaders, uniforms);
}

RendererAsteroids::~RendererAsteroids()
{
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}
