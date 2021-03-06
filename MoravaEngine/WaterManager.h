#pragma once

#include "glm/glm.hpp"

#include "FramebufferWater.h"


class WaterManager
{
public:
	WaterManager();
	WaterManager(int width, int height, float waterHeight, float waveSpeed);
	void SwitchToDefaultFramebuffer();
	inline int GetFramebufferWidth() const { return m_Width; };
	inline int GetFramebufferHeight() const { return m_Height; };
	inline FramebufferWater* GetReflectionFramebuffer() const { return reflectionFB; };
	inline FramebufferWater* GetRefractionFramebuffer() const { return refractionFB; };
	inline float GetWaterHeight() const { return m_WaterHeight; };
	inline void SetWaterHeight(float waterHeight) { m_WaterHeight = waterHeight; };
	inline float GetWaterMoveFactor() const { return m_MoveFactor; };
	inline void SetWaterMoveFactor(float moveFactor) { m_MoveFactor = moveFactor; };
	inline glm::vec4 GetWaterColor() const { return m_WaterColor; };
	inline void SetWaterColor(glm::vec4 waterColor) { m_WaterColor = waterColor; };
	~WaterManager();

public:
	static float m_WaveSpeed;

private:
	FramebufferWater* reflectionFB;
	FramebufferWater* refractionFB;

	int m_Width;
	int m_Height;

	float m_WaterHeight;
	float m_MoveFactor;

	glm::vec4 m_WaterColor;

};
