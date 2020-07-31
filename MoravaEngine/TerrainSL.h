#pragma once

#include "TerrainVoxel.h"

#include "MapGenerator.h"


/**
 * Based on Procedural Landmass Generation series by Sebastian Lague
 */
class TerrainSL : public TerrainVoxel
{
public:
	TerrainSL();
	TerrainSL(MapGenerator::MapGenConf mapGenConf);
	~TerrainSL();

	virtual void Generate() override;

private:
	const char* m_HeightMapFilePath;
	const char* m_ColorMapFilePath;
	MapGenerator::DrawMode m_DrawMode;
	unsigned int m_Width;
	unsigned int m_Height;
	float m_NoiseScale;
	unsigned int m_Octaves;
	float m_Persistance;
	float m_Lacunarity;
	int m_Seed;
	glm::vec2 m_Offset;
	std::vector<MapGenerator::TerrainTypes> m_Regions;

	float m_Frequency;
	float m_Amplitude;
	MapGenerator* m_MapGenerator;
};
