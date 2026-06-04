#pragma once
#include <GLFW/glfw3.h>
// Work on it later
class Time
{
public:
	Time() = default;
	~Time() = default;

	void update();
	float getDelta() const;
	int getFps() const;


private:
	float m_lastTime{ 0.0f };
	float m_deltaTime{ 0.0f };
	int m_fps{ 0 };
	int m_counter{ 0 };
	float m_seconds{ 0.0f };
};
