#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class ShaderProgram
{
public:
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	
	unsigned int get();
	void use();
	void deleteShader();

private:
	unsigned int m_id;
};

