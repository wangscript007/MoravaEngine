#include "ParticleRendererInstanced.h"

#include "RendererBasic.h"
#include "Util.h"


ParticleRendererInstanced::ParticleRendererInstanced()
{
	m_Shader = new Shader("Shaders/ThinMatrix/particle_instanced.vs", "Shaders/ThinMatrix/particle_instanced.fs");
	printf("ParticleRenderer: m_Shader compiled [programID=%d]\n", m_Shader->GetProgramID());

	m_Mesh = new QuadInstanced();

	// loading positions in VAO
	static_cast<QuadInstanced*>(m_Mesh)->LoadToVAO();

	m_VBO_Data = new std::vector<float>();
	// static_cast<QuadInstanced*>(m_Mesh)->CreateEmptyVBO(INSTANCE_DATA_LENGTH * MAX_INSTANCES);
	// unsigned int VBO_Instanced = static_cast<QuadInstanced*>(m_Mesh)->GetVBOInstanced();

	// model-view matrix in attribute slots 1 to 4
	// static_cast<QuadInstanced*>(m_Mesh)->AddInstancedAttribute(m_Mesh->GetVAO(), VBO_Instanced, 1, 4, INSTANCE_DATA_LENGTH,  0);
	// static_cast<QuadInstanced*>(m_Mesh)->AddInstancedAttribute(m_Mesh->GetVAO(), VBO_Instanced, 2, 4, INSTANCE_DATA_LENGTH,  4);
	// static_cast<QuadInstanced*>(m_Mesh)->AddInstancedAttribute(m_Mesh->GetVAO(), VBO_Instanced, 3, 4, INSTANCE_DATA_LENGTH,  8);
	// static_cast<QuadInstanced*>(m_Mesh)->AddInstancedAttribute(m_Mesh->GetVAO(), VBO_Instanced, 4, 4, INSTANCE_DATA_LENGTH, 12);
	// texture offsets in attribute slot 5
	// static_cast<QuadInstanced*>(m_Mesh)->AddInstancedAttribute(m_Mesh->GetVAO(), VBO_Instanced, 5, 4, INSTANCE_DATA_LENGTH, 16);
	// blend factor in attribute slot 6
	// static_cast<QuadInstanced*>(m_Mesh)->AddInstancedAttribute(m_Mesh->GetVAO(), VBO_Instanced, 6, 1, INSTANCE_DATA_LENGTH, 20);

	// glBindAttribLocation(m_Shader->GetProgramID(), 0, "aPosition");
	// glBindAttribLocation(m_Shader->GetProgramID(), 1, "modelView");
	// glBindAttribLocation(m_Shader->GetProgramID(), 5, "texOffsets");
	// glBindAttribLocation(m_Shader->GetProgramID(), 6, "blendFactor");

	printf("ParticleRendererInstanced::ParticleRendererInstanced m_VAO = %i\n",           static_cast<QuadInstanced*>(m_Mesh)->GetVAO());
	printf("ParticleRendererInstanced::ParticleRendererInstanced m_VBO = %i\n",           static_cast<QuadInstanced*>(m_Mesh)->GetVBO());
	printf("ParticleRendererInstanced::ParticleRendererInstanced m_VBO_Instanced = %i\n", static_cast<QuadInstanced*>(m_Mesh)->GetVBOInstanced());
}

void ParticleRendererInstanced::RenderBefore()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_ONE
	glDepthMask(GL_FALSE);
}

void ParticleRendererInstanced::RenderAfter()
{
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void ParticleRendererInstanced::Render(std::map<ParticleTexture*, std::vector<Particle*>*>* particleMap, glm::mat4 viewMatrix)
{
	m_Shader->Bind();
	m_Shader->setMat4("projection", RendererBasic::GetProjectionMatrix());
	m_Shader->setInt("albedoMap", 0);

	RenderBefore();

	for (auto it_map = particleMap->begin(); it_map != particleMap->end(); it_map++)
	{
		ParticleTexture* particleTexture = it_map->first;
		BindTexture(particleTexture);
		m_Shader->setFloat("numberOfRows", (float)particleTexture->GetNumberOfRows());

		m_Pointer = 0;
		std::vector<Particle*> particleVector = *it_map->second;
		m_VBO_Data->resize(particleVector.size() * INSTANCE_DATA_LENGTH);

		for (auto particle : particleVector)
		{
			UpdateModelViewMatrix(particle->GetPosition(), particle->GetRotation(), particle->GetScale(), viewMatrix, m_VBO_Data);
			UpdateTexCoordInfo(particle, m_VBO_Data);

			glBindVertexArray(static_cast<QuadInstanced*>(m_Mesh)->GetVAO());
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}

		static_cast<QuadInstanced*>(m_Mesh)->UpdateVBO(static_cast<QuadInstanced*>(m_Mesh)->GetVBOInstanced(), (unsigned int)m_VBO_Data->size(), m_VBO_Data);
		// static_cast<QuadInstanced*>(m_Mesh)->Render((unsigned int)particleVector.size());
	}

	RenderAfter();

	m_Shader->Unbind();
}

void ParticleRendererInstanced::UpdateModelViewMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::mat4 viewMatrix, std::vector<float>* vboData)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);

	modelMatrix[0][0] = viewMatrix[0][0];
	modelMatrix[0][1] = viewMatrix[1][0];
	modelMatrix[0][2] = viewMatrix[2][0];

	modelMatrix[1][0] = viewMatrix[0][1];
	modelMatrix[1][1] = viewMatrix[1][1];
	modelMatrix[1][2] = viewMatrix[2][1];

	modelMatrix[2][0] = viewMatrix[0][2];
	modelMatrix[2][1] = viewMatrix[1][2];
	modelMatrix[2][2] = viewMatrix[2][2];

	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);

	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

	StoreMatrixData(modelViewMatrix, vboData);

	m_Shader->Bind();
	m_Shader->setMat4("modelView", modelViewMatrix);

	// Debug uniforms
	m_Shader->setMat4("uModelView", modelViewMatrix);
	m_Shader->setVec4("uTexOffsets", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	m_Shader->setFloat("uBlendFactor", 0.5f);
	m_Shader->setVec3("uPosition", position);

	// printf("ParticleRendererInstanced::UpdateModelViewMatrix modelViewMatrix:\n");
	// Util::printMatrix(modelViewMatrix);
}

void ParticleRendererInstanced::StoreMatrixData(glm::mat4 matrix, std::vector<float>* vboData)
{
	vboData->at(m_Pointer++) = matrix[0][0];
	vboData->at(m_Pointer++) = matrix[0][1];
	vboData->at(m_Pointer++) = matrix[0][2];
	vboData->at(m_Pointer++) = matrix[0][3];

	vboData->at(m_Pointer++) = matrix[1][0];
	vboData->at(m_Pointer++) = matrix[1][1];
	vboData->at(m_Pointer++) = matrix[1][2];
	vboData->at(m_Pointer++) = matrix[1][3];

	vboData->at(m_Pointer++) = matrix[2][0];
	vboData->at(m_Pointer++) = matrix[2][1];
	vboData->at(m_Pointer++) = matrix[2][2];
	vboData->at(m_Pointer++) = matrix[2][3];

	vboData->at(m_Pointer++) = matrix[3][0];
	vboData->at(m_Pointer++) = matrix[3][1];
	vboData->at(m_Pointer++) = matrix[3][2];
	vboData->at(m_Pointer++) = matrix[3][3];
}

void ParticleRendererInstanced::UpdateTexCoordInfo(Particle* particle, std::vector<float>* vboData)
{
	vboData->at(m_Pointer++) = particle->GetTexOffset1().x;
	vboData->at(m_Pointer++) = particle->GetTexOffset1().y;
	vboData->at(m_Pointer++) = particle->GetTexOffset2().x;
	vboData->at(m_Pointer++) = particle->GetTexOffset2().y;
	vboData->at(m_Pointer++) = particle->GetBlend();

	//	printf("\nvboData:\n");
	//	for (int i = 0; i < vboData->size(); i++)
	//		printf("[%i] %.2ff ", i, vboData->at(i));
}

void ParticleRendererInstanced::CleanUp()
{
	delete m_VBO_Data;
	delete m_Mesh;
	delete m_Shader;
}

ParticleRendererInstanced::~ParticleRendererInstanced()
{
	CleanUp();
}
