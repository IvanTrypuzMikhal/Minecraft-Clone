#include "SkyBox.h"
#include <Rendering/ResourceManager.h>
#include <array>

std::array skyboxVertices = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


SkyBox::SkyBox() {
	m_vbo = std::make_unique<Vbo>(skyboxVertices);
	std::vector<VertexAttribute> attributes = { {.index = 0, .size = 3, .offset = 0 } };
	m_vao = std::make_unique<Vao>(m_vbo->get(), 3, attributes);
	m_shader = ResourceManager::getShaderProgram("skyboxShader");
}

void SkyBox::render(const glm::mat4& view, const glm::mat4& projection, float worldTime) {
    glDepthFunc(GL_LEQUAL);
    m_shader->use();
	m_shader->setMat4("view", view);
	m_shader->setMat4("projection", projection);
	glm::vec3 skyboxColor;
	glm::vec3 fogColor;
	getSkyboxColor(worldTime, skyboxColor, fogColor);
	//std::cout << "Skybox Color: (" << skyboxColor.r << ", " << skyboxColor.g << ", " << skyboxColor.b << ")\n";
	std::cout << "World Time: " << worldTime << "\n";
    m_shader->setVec3("skyboxColor", skyboxColor);
	m_shader->setVec3("fogColor", fogColor);
	glBindVertexArray(m_vao->get());
	glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}

void SkyBox::getSkyboxColor(float worldTime, glm::vec3& skyColor, glm::vec3& fogColor) const {
	float time = fmod(worldTime, 24000.0f);
    if (time < 11000.0f) {
        // Day
        skyColor = m_dayColor;
        fogColor = m_dayFog;
    }
    else if (time < 13000.0f) {
        // Sunset
        float t = (time - 11000.0f) / 2000.0f;
        skyColor = glm::mix(m_dayColor, m_nightColor, t);

		// For fog color, we want to transition from dayFog to sunsetFog and then to nightFog
        if (t < 0.5f)   fogColor = glm::mix(m_dayFog, m_sunsetFog, t * 2.0f);
        else            fogColor = glm::mix(m_sunsetFog, m_nightFog, (t - 0.5f) * 2.0f);
    }
    else if (time < 22000.0f) {
		// Night
        skyColor = m_nightColor;
        fogColor = m_nightFog;
    }
    else {
		// Transition from night to day 
        float t = (time - 22000.0f) / 2000.0f;
        skyColor = glm::mix(m_nightColor, m_dayColor, t);
        fogColor = glm::mix(m_nightFog, m_dayFog, t);
    }
}