#pragma once

#include "MeshUnity.h"

#include <glm/glm.hpp>

#include <vector>


class MeshData;

/**
 * Based on Procedural Landmass Generation series by Sebastian Lague
 */
class MeshGenerator
{
public:
	MeshGenerator();
	~MeshGenerator();

	static MeshData* GenerateTerrainMesh(float** heightMap, unsigned int width, unsigned int height);

};

class MeshData
{
public:
	MeshData(unsigned int meshWidth, unsigned int meshHeight);
	~MeshData();

	void AddTriangle(int a, int b, int c);
	MeshUnity* CreateMesh();

public:
	std::vector<glm::vec3>* m_Vertices;
	std::vector<int>* m_Triangles;
	std::vector<glm::vec2>* m_UVs;

	unsigned int m_VerticeIndex;
	unsigned int m_TriangleIndex;

};
