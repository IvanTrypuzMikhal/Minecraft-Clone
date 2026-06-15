#pragma once
#include <glm/glm.hpp>
#include <Rendering/Vao.h>
#include <Rendering/Vbo.h>
#include <Rendering/ShaderProgram.h>

class SkyBox
{
public:
    SkyBox();
    ~SkyBox() = default;

	void render(const glm::mat4& view, const glm::mat4& projection, float worldTime);
	void getSkyboxColor(float worldTime, glm::vec3& skyColor, glm::vec3& fogColor) const;

private:
	std::unique_ptr<Vao> m_vao;
	std::unique_ptr<Vbo> m_vbo;
	std::shared_ptr<ShaderProgram> m_shader;

	glm::vec3 m_dayColor = glm::vec3(0.4706f, 0.6549f, 1.000f);
	glm::vec3 m_nightColor = glm::vec3(0.0431f, 0.0549f, 0.0784f);

	glm::vec3 m_dayFog = glm::vec3(0.7529f, 0.8471f, 1.0000f);
	glm::vec3 m_nightFog = glm::vec3(0.0431f, 0.0549f, 0.0784f);
	glm::vec3 m_sunsetFog = glm::vec3(0.3500f, 0.1500f, 0.1800f);
};
