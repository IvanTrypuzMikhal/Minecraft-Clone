#include "UIRenderer.h"
#include <Rendering/ResourceManager.h>

UIRenderer::UIRenderer() : m_texture{ ResourceManager::getTexture("uiTexture") }, m_shaderProgram { ResourceManager::getShaderProgram("uiShader")} {
    std::vector<VertexAttribute> block{ {0, 2, 0, GL_FLOAT }, {1, 2, 2, GL_FLOAT} };
    m_vbo = std::make_unique<Vbo>();
    m_vao = std::make_unique<Vao>(m_vbo->get(), 4, block);
}

void UIRenderer::begin() {
	m_batch.clear();
}

void UIRenderer::flush(const Window* window) {
    glDisable(GL_DEPTH_TEST);
    if (m_batch.empty()) return;
    m_vbo->upload(m_batch);
    m_shaderProgram->use();
    m_texture->setTexture();
    m_shaderProgram->setMat4("projection",glm::ortho(0.0f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.0f));
    m_vao->use();
    glDrawArrays(GL_TRIANGLES, 0, m_batch.size() / 4);
    glEnable(GL_DEPTH_TEST);
}

void UIRenderer::fushInverted(const Window* window) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
    flush(window);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UIRenderer::drawQuad(float x, float y, float w, float h, float u0, float v0, float u1, float v1) {

    m_batch.insert(m_batch.end(), {
        x,     y,     u0, v0,
        x + w, y,     u1, v0,
        x,     y + h, u0, v1,});
    m_batch.insert(m_batch.end(), {
        x + w, y,     u1, v0,
        x + w, y + h, u1, v1,
        x,     y + h, u0, v1,});
}
