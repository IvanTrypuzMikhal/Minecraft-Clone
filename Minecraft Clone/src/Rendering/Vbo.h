#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Vbo
{	
public:
	template<typename T, std::size_t SIZE>
	
	Vbo(const std::array<T, SIZE>& arr) {
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T) * SIZE, arr.data(), GL_STATIC_DRAW);
	}

	Vbo(float* mesh, int size);
	Vbo(std::vector<float>& mesh);
	Vbo(std::vector<uint32_t>& mesh);
	Vbo(int size);
	Vbo();
	~Vbo();

	void upload(std::vector<float>& mesh) const;
	void upload(std::vector<uint32_t>& mesh);

	[[nodiscard]] unsigned int get() const;

private:
	unsigned int m_id;
};
