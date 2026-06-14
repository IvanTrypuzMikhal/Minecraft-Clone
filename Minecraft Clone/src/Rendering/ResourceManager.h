#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "ShaderProgram.h"
#include "Texture.h"


class ResourceManager
{
public:

	static void loadShaderProgram(const std::string& name, const char* vertexPath, const char* fragmentPath);
	static void loadTexture(const std::string& name, const char* filename, unsigned int format);

	static std::shared_ptr<ShaderProgram> getShaderProgram(const std::string& name);
	static std::shared_ptr<Texture> getTexture(const std::string& name);

	static void clearResources();
private:
	static std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_shaderPrograms;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
};
