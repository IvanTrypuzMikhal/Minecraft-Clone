#include "Time.h"


void Time::update() {
    const auto currentTime = static_cast<float>(glfwGetTime());
    m_deltaTime = currentTime - m_lastTime;
    m_lastTime = currentTime;

    m_counter++;
    m_seconds += m_deltaTime; 
	m_tickAccumulator += m_deltaTime;

    if (m_seconds >= 1.0f) {
        m_fps = m_counter;
        m_seconds -= 1.0f; 
        m_counter = 0;
    }
}

bool Time::isTickReady() {
    if (m_tickAccumulator >= Globals::TICK_RATE) {
        m_tickAccumulator -= Globals::TICK_RATE;
		return true;
	}
	return false;
}

float Time::getDelta() const {
    return m_deltaTime;
}

int Time::getFps() const {
    return m_fps;
}

float Time::getTickAcumulator() const{
    return m_tickAccumulator;
}

float Time::getWorldTime() const {
	return m_worldTime;
}

void Time::updateWorldTime() {
    m_worldTime += 1;
}