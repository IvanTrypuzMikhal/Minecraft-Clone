#include "Window.h"
#include "AppContext.h"
#include <Gameplay/Input/InputManager.h>

Window::Window(int width, int height, const char* title) {

	if(!glfwInit()) throw std::exception("GLFW could not be initialized!");
	std::cerr << "- Window initialization successfull!\n";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	m_width = width;
	m_height = height;
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

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
	
void Window::frameBufferResizeCallback(GLFWwindow* win, int width, int height) {
	AppContext* appContext = static_cast<AppContext*>(glfwGetWindowUserPointer(win));
	glViewport(0, 0, width, height);
	appContext->window->setWidth(width);
	appContext->window->setHeight(height);
}

GLFWwindow* Window::getWindow() const{ 
	return m_window; 
}

bool Window::shouldClose() const{
	return glfwWindowShouldClose(m_window);
}

void Window::setWidth(int width) {
	m_width = width;
}
void Window::setHeight(int height) {
	m_height = height;
}

int Window::getWidth() const{
	return m_width;
}
int Window::getHeight() const {
	return m_height;
}