#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct VertexAttribute {
	int index;
	int size;
	int offset;
	GLenum type = GL_FLOAT;
};

class Vao
{
public:
	// Vertices are assumed to be of xyz
	Vao(unsigned int vboId, int stride, std::vector<VertexAttribute>);
	Vao() = default;
	~Vao();
	void use() const;
	unsigned int get() const;

private:
	unsigned int m_id;
};
