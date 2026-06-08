#pragma once
#include <glad/glad.h>

class Texture
{
public:
	Texture(const char* filename, unsigned int format);
	[[nodiscard]] unsigned int getTexture() const;
	void setTexture() const;

private:
	unsigned int m_id;
};
