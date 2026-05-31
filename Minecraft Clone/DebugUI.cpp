#include "DebugUI.h"

void DebugUI::renderText(const Camera& cam, const Window& window, const Time& time) {
	glm::vec3 camPosition = cam.getCameraPosition();
	float pitch = cam.getPitch();
	float yaw = cam.getYaw();
	float fov = cam.getFov();

	int height = window.getHeight();
	int width = window.getWidth();

	int chunkX = static_cast<int>(std::floor(camPosition.x / 16.0f));
	int chunkZ = static_cast<int>(std::floor(camPosition.z / 16.0f));

	int fps = time.getFps();

	std::string camPositionString	= std::format("Camera x: {:.2f} y: {:.2f} z: {:.2f}", camPosition.x, camPosition.y, camPosition.z);
	std::string pitchString			= std::format("Pitch: {:.2f}", pitch);
	std::string yawString			= std::format("Yaw: {:.2f}", yaw);
	std::string fovString			= std::format("FOV: {:.2f}", fov);
	std::string ratioString			= std::format("Window: {} x {}", width, height);
	std::string chunkString			= std::format("Chunk: {}, {}", chunkX, chunkZ);
	std::string fpsString			= std::format("FPS: {}", fps);

	m_textRenderer.renderText(fpsString, 10.0f, height - 30.f, 0.7f, glm::vec3(1.0f), width, height);
	m_textRenderer.renderText(fpsString, 12.0f, height - 32.f, 0.7f, glm::vec3(0.25f), width, height);

	m_textRenderer.renderText(camPositionString, 10.0f, height - 50.f, 0.7f, glm::vec3(1.0f), width, height);
	m_textRenderer.renderText(camPositionString, 12.0f, height - 52.f, 0.7f, glm::vec3(0.25f), width, height);

	m_textRenderer.renderText(pitchString, 10.0f, height - 70.f, 0.7f, glm::vec3(1.0f), width, height);
	m_textRenderer.renderText(pitchString, 12.0f, height - 72.f, 0.7f, glm::vec3(0.25f), width, height);

	m_textRenderer.renderText(yawString, 10.0f, height - 90.f, 0.7f, glm::vec3(1.0f), width, height);
	m_textRenderer.renderText(yawString, 12.0f, height - 92.f, 0.7f, glm::vec3(0.25f), width, height);

	m_textRenderer.renderText(fovString, 10.0f, height - 110.f, 0.7f, glm::vec3(1.0f), width, height);
	m_textRenderer.renderText(fovString, 12.0f, height - 112.f, 0.7f, glm::vec3(0.25f), width, height);

	m_textRenderer.renderText(ratioString, 10.0f, height - 130.f, 0.7f, glm::vec3(1.0f), width, height);
	m_textRenderer.renderText(ratioString, 12.0f, height - 132.f, 0.7f, glm::vec3(0.25f), width, height);

	m_textRenderer.renderText(chunkString, 10.0f, height - 150.f, 0.7f, glm::vec3(1.0f), width, height);
	m_textRenderer.renderText(chunkString, 12.0f, height - 152.f, 0.7f, glm::vec3(0.25f), width, height);

}
