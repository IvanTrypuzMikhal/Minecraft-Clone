#include "Window.h"


Window::Window(int width, int height, const char* title) {

	if(!glfwInit()) throw std::exception("GLFW could not be initialized!");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_window == nullptr) {
		throw std::exception("Window creation failed!");
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::exception("GLAD could not be initialized");
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(m_window, Window::frameBufferResizeCallback);
}


void Window::processInput() {
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
		std::cout << "Window closed!" << std::endl;
	}
}



void Window::frameBufferResizeCallback(GLFWwindow* win, int width, int height) {
	glViewport(0, 0, width, height);
}

GLFWwindow* Window::getWindow() { return m_window; }

