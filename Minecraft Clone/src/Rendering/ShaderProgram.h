#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


class ShaderProgram
{
public:
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	~ShaderProgram();
	[[nodiscard]] unsigned int get() const;
	void use() const;

	void setFloat(const char* location, float x) const;
	void setVec2(const char* location, float x, float y) const;
	void setVec2(const char* location, glm::vec2& vec) const;
	void setVec3(const char* location, float x, float y, float z) const;
	void setVec3(const char* location, glm::vec3& vec) const;
	void setMat4(const char* location, glm::mat4 mat) const;

private:
	unsigned int m_id;
};

