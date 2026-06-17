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

std::array celestialBodyVertices = {
    // pos XY                UV
    -1.0f,  1.0f,  0.0f,    0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f,    0.0f, 0.0f,
     1.0f, -1.0f,  0.0f,    1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f,    0.0f, 1.0f,
     1.0f, -1.0f,  0.0f,    1.0f, 0.0f,
     1.0f,  1.0f,  0.0f,    1.0f, 1.0f
};



SkyBox::SkyBox() {
	m_vbo = std::make_unique<Vbo>(skyboxVertices);
	std::vector<VertexAttribute> attributes = { {.index = 0, .size = 3, .offset = 0 } };
	m_vao = std::make_unique<Vao>(m_vbo->get(), 3, attributes);
	m_shader = ResourceManager::getShaderProgram("skyboxShader");
    
	std::vector<VertexAttribute> celestialAttributes = { {.index = 0, .size = 3, .offset = 0 }, {.index = 1, .size = 2, .offset = 3} };
    m_sun.m_vbo = std::make_unique<Vbo>(celestialBodyVertices);
    m_sun.m_vao = std::make_unique<Vao>(m_sun.m_vbo->get(), 5, celestialAttributes);
    m_sun.m_shader = ResourceManager::getShaderProgram("celestialBodyShader");
    m_sun.m_texture = ResourceManager::getTexture("sunTexture");

    m_sun_glow.m_vbo = std::make_unique<Vbo>(celestialBodyVertices);
    m_sun_glow.m_vao = std::make_unique<Vao>(m_sun.m_vbo->get(), 5, celestialAttributes);
    m_sun_glow.m_shader = ResourceManager::getShaderProgram("celestialBodyShader");
    m_sun_glow.m_texture = ResourceManager::getTexture("sunGlow");


    m_moon.m_vbo = std::make_unique<Vbo>(celestialBodyVertices);
    m_moon.m_vao = std::make_unique<Vao>(m_sun.m_vbo->get(), 5, celestialAttributes);
    m_moon.m_shader = ResourceManager::getShaderProgram("celestialBodyShader");
    m_moon.m_texture = ResourceManager::getTexture("moonTexture");

    m_moon_glow.m_vbo = std::make_unique<Vbo>(celestialBodyVertices);
    m_moon_glow.m_vao = std::make_unique<Vao>(m_sun.m_vbo->get(), 5, celestialAttributes);
    m_moon_glow.m_shader = ResourceManager::getShaderProgram("celestialBodyShader");
    m_moon_glow.m_texture = ResourceManager::getTexture("moonGlow");
}
// This is bad performance wise but i don't have the knowledge to create the glow in the shader yet.
// Temp solution for now.
void SkyBox::render(const glm::mat4& view, const glm::mat4& projection, float worldTime) {
    glDepthFunc(GL_LEQUAL);
    m_shader->use();
	m_shader->setMat4("view", view);
	m_shader->setMat4("projection", projection);
	glm::vec3 skyboxColor;
	glm::vec3 fogColor;
	getSkyboxColor(worldTime, skyboxColor, fogColor);
    m_shader->setVec3("skyboxColor", skyboxColor);
	m_shader->setVec3("fogColor", fogColor);
	glBindVertexArray(m_vao->get());
	glDrawArrays(GL_TRIANGLES, 0, 36);

    // I'll have to use 2 textures and different blend functions to render becouse i dont know how to make the black background of the sun glow texture transparent and leave the glow.
    glEnable(GL_BLEND);
    int faseActual = getMoonPhase(worldTime);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_sun_glow.m_shader->use();
    m_sun_glow.m_shader->setBool("isGlow", true);
    m_sun_glow.m_shader->setBool("isMoon", false); 
    renderCelestialBody(m_sun_glow, view, projection, worldTime);

    m_moon_glow.m_shader->use();
    m_moon_glow.m_shader->setBool("isGlow", true);
    m_moon_glow.m_shader->setBool("isMoon", true);
    m_moon_glow.m_shader->setInt("moonPhase", faseActual); 
    renderCelestialBody(m_moon_glow, view, projection, worldTime);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_sun.m_shader->use();
    m_sun.m_shader->setBool("isGlow", false);
    m_sun.m_shader->setBool("isMoon", false); 
    renderCelestialBody(m_sun, view, projection, worldTime);

    m_moon.m_shader->use();
    m_moon.m_shader->setBool("isGlow", false);
    m_moon.m_shader->setBool("isMoon", true);
    m_moon.m_shader->setInt("moonPhase", faseActual);
    renderCelestialBody(m_moon, view, projection, worldTime);

    glDisable(GL_BLEND);
    glDepthFunc(GL_LESS);
}

void SkyBox::renderCelestialBody(const CelestialBody& body, const glm::mat4& view, const glm::mat4& projection, float worldTime) {

	float currentTime = fmod(worldTime, 24000.0f);
    float degrees = (currentTime / 24000.0f) * 360.0f;

	if (body.m_texture == m_moon.m_texture || body.m_texture == m_moon_glow.m_texture) {
		degrees += 180.0f;
	}

    glm::mat4 model = glm::mat4(1.0f);
	
    model = glm::rotate(model, glm::radians(degrees), glm::vec3(1.0f, 0.0f, 0.0f)); 
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -100.0f));
	model = glm::scale(model, glm::vec3(30.0f));

    body.m_shader->use();
    body.m_shader->setMat4("view", view);
    body.m_shader->setMat4("projection", projection);
    body.m_shader->setMat4("model", model);
    body.m_texture->setTexture();
    glBindVertexArray(body.m_vao->get());
    glDrawArrays(GL_TRIANGLES, 0, 6);
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

int SkyBox::getMoonPhase(float worldTime) const {
    int totalDays = static_cast<int>(worldTime / 24000.0f);
	int phase = totalDays % 8;
	return phase;
}
