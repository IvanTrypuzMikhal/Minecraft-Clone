#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Vbo
{	
public:
	Vbo(float(&arr)[], int size);
	Vbo(std::vector<float>& mesh);
	Vbo();
	~Vbo();

	void upload(std::vector<float>& mesh) const;

	unsigned int get() const;

private:
	unsigned int m_id;
};
