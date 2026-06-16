#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const char* filename, unsigned int format) {
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

	if (!data) {
		throw std::exception("TEXTURE::FAILED::TO::LOAD");
	}
	std::cerr << "Texture loaded: " << filename << " (" << width << "x" << height << ", " << nrChannels << " channels)" << std::endl;
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

unsigned int Texture::getTexture() const { return m_id; }

void Texture::setTexture() const{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_id);
}