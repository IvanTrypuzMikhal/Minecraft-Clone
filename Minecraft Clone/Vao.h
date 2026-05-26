#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Vao
{
public:
	Vao();
	~Vao();
	void use() const;
	unsigned int get() const;

private:
	unsigned int m_id;
};
