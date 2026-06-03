#include "UIRenderer.h"

UIRenderer::UIRenderer(): m_texture("gui-atlas.png", GL_RGBA), m_shaderProgram("UIShader.vert", "UIShader.frag") {
    std::vector<VertexAttribute> block{ {0, 2, 0, GL_FLOAT }, {1, 2, 2, GL_FLOAT} };
    m_vbo = std::make_unique<Vbo>();
    m_vao = std::make_unique<Vao>(m_vbo->get(), 4, block);
}

void UIRenderer::begin() {
	m_batch.clear();
}

void UIRenderer::flush(const Window* window) {
    if (m_batch.empty()) return;
    m_vbo->upload(m_batch);
    m_shaderProgram.use();
    m_texture.setTexture();
    m_shaderProgram.setMat4("projection",glm::ortho(0.0f, (float)window->getWidth(), (float)window->getHeight(), 0.0f));
    m_vao->use();
    glDrawArrays(GL_TRIANGLES, 0, m_batch.size() / 4);
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
