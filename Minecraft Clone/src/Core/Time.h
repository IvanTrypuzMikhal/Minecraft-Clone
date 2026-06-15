#pragma once
#include <GLFW/glfw3.h>
#include <Core/Globals.h>

// Work on it later
class Time
{
public:
	Time() = default;
	~Time() = default;

	void update();
	bool isTickReady();
	void updateWorldTime();
	[[nodiscard]] float getDelta() const;
	[[nodiscard]] int getFps() const;
	[[nodiscard]] float getTickAcumulator() const;
	[[nodiscard]] float getWorldTime() const;


private:
	float m_lastTime{ 0.0f };
	float m_deltaTime{ 0.0f };
	int m_fps{ 0 };
	int m_counter{ 0 };
	float m_seconds{ 0.0f };
	float m_tickAccumulator{ 0.0f };

	float m_worldTime{ 0.0f };
};
