#include "Vbo.h"

Vbo::Vbo(float(&arr)[], int size) {
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, arr, GL_STATIC_DRAW);
}

Vbo::~Vbo() {
	glDeleteBuffers(1, &m_id);
}

unsigned int Vbo::get() const{
	return m_id;
}