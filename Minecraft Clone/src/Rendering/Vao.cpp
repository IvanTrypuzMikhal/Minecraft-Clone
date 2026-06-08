#include "Vao.h"

Vao::Vao(unsigned int vboId, int stride, std::vector<VertexAttribute> attributes) {
	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	for (VertexAttribute va : attributes) {

		if (va.type == GL_UNSIGNED_INT || va.type == GL_INT) {
			glVertexAttribIPointer(va.index, va.size, va.type, stride * sizeof(uint32_t), (void*)(va.offset * sizeof(uint32_t)));
		}
		else {
			glVertexAttribPointer(va.index, va.size, va.type, GL_FALSE, stride * sizeof(float), (void*)(va.offset * sizeof(float)));
		}
		glEnableVertexAttribArray(va.index);
	}
}

Vao::~Vao() {
	glDeleteVertexArrays(1, &m_id);
}
unsigned int Vao::get() const{
	return m_id;
}

void Vao::use() const{
	glBindVertexArray(m_id);
}
