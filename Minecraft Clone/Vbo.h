#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Vbo
{
public:
	Vbo(float(&arr)[], int size);
	~Vbo();
	unsigned int get() const;

private:
	unsigned int m_id;
};
