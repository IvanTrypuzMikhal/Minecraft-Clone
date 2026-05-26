#include "Vao.h"


Vao::Vao() {
	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);
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
