#include "Vbo.h"
#include <array>

Vbo::Vbo() {
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}



Vbo::Vbo(int size) {
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
}

Vbo::Vbo(std::vector<float>& mesh){
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), &mesh[0], GL_STATIC_DRAW);
}

Vbo::Vbo(std::vector<uint32_t>& mesh) {
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(uint32_t), &mesh[0], GL_STATIC_DRAW);
}

void Vbo::upload(std::vector<float>& mesh) const {
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	if (!mesh.empty()) {
		glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), mesh.data(), GL_STATIC_DRAW);
	}
}

void Vbo::upload(std::vector<uint32_t>& mesh) {
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	if (!mesh.empty()) {
		glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(uint32_t), mesh.data(), GL_STATIC_DRAW);
	}
}

Vbo::~Vbo() {
	glDeleteBuffers(1, &m_id);
}

unsigned int Vbo::get() const{
	return m_id;
}