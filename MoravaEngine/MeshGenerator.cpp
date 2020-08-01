#include "MeshGenerator.h"


MeshGenerator::MeshGenerator()
{
}

MeshGenerator::~MeshGenerator()
{
}

MeshData* MeshGenerator::GenerateTerrainMesh(float** heightMap, unsigned int width, unsigned int height)
{
	float topLeftX = (width - 1) / 2.0f;
	float topLeftZ = (height - 1) / 2.0f;

	MeshData* meshData = new MeshData(width, height);
	unsigned int vertexIndex = 0;

	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {

			meshData->m_Vertices.push_back(glm::vec3(topLeftX + x, *heightMap[x, y], topLeftZ - y));
			meshData->m_UVs.push_back(glm::vec2(x / (float)width, y / (float)height));
			meshData->m_Normals.push_back(glm::vec3(0.0f));

			if (x < width - 1 && y < height - 1) {
				meshData->AddTriangle(vertexIndex, vertexIndex + width + 1, vertexIndex + width);
				meshData->AddTriangle(vertexIndex + width + 1, vertexIndex, vertexIndex + 1);
			}

			vertexIndex++;
		}
	}

	return meshData;
}

MeshData::MeshData(unsigned int meshWidth, unsigned int meshHeight)
{
	m_VerticeIndex = meshWidth * meshHeight;
	m_Vertices = std::vector<glm::vec3>();
	// m_Vertices->resize(m_VerticeIndex);

	m_UVs = std::vector<glm::vec2>();
	// m_UVs->resize(m_VerticeIndex);

	m_TriangleIndex = (meshWidth - 1) * (meshHeight - 1) * 6;
	m_Triangles = std::vector<int>();
	// m_Triangles->resize(m_TriangleIndex);
}

MeshData::~MeshData()
{
}

void MeshData::AddTriangle(int a, int b, int c)
{
	m_Triangles.push_back(a);
	m_Triangles.push_back(b);
	m_Triangles.push_back(c);
	m_TriangleIndex += 3;
}

MeshUnity* MeshData::CreateMesh()
{
	MeshUnity* mesh = new MeshUnity();
	mesh->vertices = &m_Vertices;
	mesh->triangles = &m_Triangles;
	mesh->uv = &m_UVs;
	mesh->normals = &m_Normals;
	mesh->RecalculateNormals();
	mesh->Generate(glm::vec3(1.0f));
	return mesh;
}
