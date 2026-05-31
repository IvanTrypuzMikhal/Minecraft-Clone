#include "Time.h"


void Time::update() {
    float currentTime = (float)glfwGetTime();
    m_deltaTime = currentTime - m_lastTime;
    m_lastTime = currentTime;

    m_counter++;
    m_seconds += m_deltaTime; 

    if (m_seconds >= 1.0f) {
        m_fps = m_counter;
        m_seconds -= 1.0f; 
        m_counter = 0;
    }
}

float Time::getDelta() const {
    return m_deltaTime;
}

int Time::getFps() const {
    return m_fps;
}