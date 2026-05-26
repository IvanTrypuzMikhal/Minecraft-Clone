#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); 
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath); 
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();
		
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch(std::ifstream::failure e) {
		throw std::exception("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}

	std::cerr << "- SHADER: File reading successful!\n";
	
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vert, frag;
	int success;
	char infolog[512];

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vShaderCode, NULL);
	glCompileShader(vert);

	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vert, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}
	else {
		std::cerr << "- SHADER: Vertex compilation successful!\n";
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fShaderCode, NULL);
	glCompileShader(frag);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}
	else {
		std::cerr << "- SHADER: Fragment compilation successful!\n";
	}

	m_id = glCreateProgram();
	glAttachShader(m_id, vert);
	glAttachShader(m_id, frag);
	glLinkProgram(m_id);

	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_id, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
	}
	else {
		std::cerr << "- SHADER: Program linking successful!\n";
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	std::cerr << "- SHADER: Cleanup successful!\n";
}

unsigned int ShaderProgram::get() {
	return m_id;
}

void ShaderProgram::use() {
	glUseProgram(m_id);
}

void ShaderProgram::deleteShader() {
	glDeleteProgram(m_id);
}