#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct VertexAttribute {
	int index;
	int size;
	int offset;
};

class Vao
{
public:
	// Vertices are assumed to be of xyz
	Vao(int stride, std::vector<VertexAttribute>);
	~Vao();
	void use() const;
	unsigned int get() const;

private:
	unsigned int m_id;
};
