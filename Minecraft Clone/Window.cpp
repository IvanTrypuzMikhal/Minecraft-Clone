#include "Window.h"

Window::Window(int width, int height, const char* title) {

	if(!glfwInit()) throw std::exception("GLFW could not be initialized!");
	std::cerr << "- Window initialization successfull!\n";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_window == nullptr) {
		throw std::exception("ERROR::Window creation failed!");
	}
	std::cerr << "- Window creation successfull!\n";

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::exception("ERROR::GLAD could not be initialized");
	}
	std::cerr << "- GLAD initialization successful!\n";


	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(m_window, Window::frameBufferResizeCallback);
	glEnable(GL_DEPTH_TEST);
}

Window::~Window() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}


void Window::processInput() const{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
		std::cout << "Window closed!\n";
	}
}

void Window::frameBufferResizeCallback(GLFWwindow* win, int width, int height) {
	glViewport(0, 0, width, height);
}


GLFWwindow* Window::getWindow() const{ 
	return m_window; 
}

bool Window::shouldClose() const{
	return glfwWindowShouldClose(m_window);
}

