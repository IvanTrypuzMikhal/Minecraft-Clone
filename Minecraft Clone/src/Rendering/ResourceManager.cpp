#include "ResourceManager.h"

std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> ResourceManager::m_shaderPrograms;
std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::m_textures;


void ResourceManager::loadShaderProgram(const std::string& name, const char* vertexPath, const char* fragmentPath) {
	auto shaderProgram = std::make_shared<ShaderProgram>(vertexPath, fragmentPath);
	m_shaderPrograms[name] = shaderProgram;
}

void ResourceManager::loadTexture(const std::string& name, const char* filename, unsigned int format) {
	auto texture = std::make_shared<Texture>(filename, format);
	m_textures[name] = texture;
}

std::shared_ptr<ShaderProgram> ResourceManager::getShaderProgram(const std::string& name){
	auto it = m_shaderPrograms.find(name);
	if (it != m_shaderPrograms.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& name) {
	auto it = m_textures.find(name);
	if (it != m_textures.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}

void ResourceManager::clearResources() {
	m_shaderPrograms.clear();
	m_textures.clear();
}