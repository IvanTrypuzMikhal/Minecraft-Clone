#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <glm/glm.hpp>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vao.h"
#include "Vbo.h"
#include "ShaderProgram.h"

struct Character {
	unsigned int TextureID;  
	glm::ivec2   Size;       
	glm::ivec2   Bearing;    
	unsigned int Advance;    
};

class Text
{
public:
	Text(const char* font, int fontHeight, ShaderProgram& s, int fontWidth = 0);
	~Text() = default;


	void renderText(std::string text, float x, float y, float scale, glm::vec3 color, float width, float height);

private:
	FT_Library m_ft;
	FT_Face m_face;

	std::map<char, Character> m_characters;

	std::unique_ptr<Vao> m_vao;
	std::unique_ptr<Vbo> m_vbo;

	ShaderProgram& m_s;
};
