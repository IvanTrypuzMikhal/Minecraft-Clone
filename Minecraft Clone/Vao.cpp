#include "Vao.h"


Vao::Vao(int stride, std::vector<VertexAttribute> attributes) {
	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);
	for (VertexAttribute va : attributes) {
		glVertexAttribPointer(va.index, va.size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(va.offset*sizeof(float)));
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
