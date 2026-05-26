#include "Vbo.h"

Vbo::Vbo(float(&arr)[], int size, int stride, int offset) {
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, arr, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offset);
	glEnableVertexAttribArray(0);
}

Vbo::~Vbo() {
	glDeleteBuffers(1, &m_id);
}

unsigned int Vbo::get() const{
	return m_id;
}