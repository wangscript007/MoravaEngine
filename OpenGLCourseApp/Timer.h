#pragma once


/**
 * A singleton class
 * 
 */
class Timer
{
public:
	Timer();
	Timer(float targetFPS);
	Timer(float targetFPS, float targetUpdateRate);
	static Timer* Get();
	inline void SetTargetFPS(float targetFPS) { m_TargetFPS = targetFPS; };
	inline void SetTargetUpdateRate(float targetUpdateRate) { m_TargetUpdateRate = targetUpdateRate; };
	inline const float GetRealFPS() const { return m_RealFPS; };
	inline const float GetRealUpdateRate() const { return m_RealUpdateRate; };
	inline const float GetDeltaTime() const { return m_DeltaTime; };
	inline const float GetCurrentTimestamp() const { return m_CurrentTimestamp; };
	void Update();
	bool CanRender() { return m_CanRender; };
	bool CanUpdate() { return m_CanUpdate; };
	~Timer();

private:
	static Timer* s_Instance;

	float m_CurrentTimestamp;

	// Render
	float m_TargetFPS;
	float m_RealFPS;
	float m_LastFrameTimestamp;
	float m_DeltaTime;
	bool m_CanRender;

	// Update
	float m_TargetUpdateRate;
	float m_RealUpdateRate;
	float m_LastUpdateTimestamp;
	float m_DeltaTimeUpdate;
	bool m_CanUpdate;
};
