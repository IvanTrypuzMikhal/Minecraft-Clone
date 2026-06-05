#include "Window.h"
#include "AppContext.h"

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
	glfwSetCursorPosCallback(m_window, Window::mouseCursorCallback);
	glfwSetScrollCallback(m_window, Window::mouseScrollCallback);
	glfwSetMouseButtonCallback(m_window, Window::mouseInputCallback);

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


void Window::processInput(){
	bool currentTabState = false;
	bool currentShiftState = false;

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
		std::cout << "Window closed!\n";
	}
	currentTabState = glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS;
	if (currentTabState && !m_lastTabState) { // rising edge only
		AppContext* appContext = static_cast<AppContext*>(glfwGetWindowUserPointer(m_window));
		if (glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPosCallback(m_window, nullptr);
			appContext->player->setControlsActive(false);
		}
		else {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			appContext->player->setFirstInput();
			appContext->player->setControlsActive(true);
			glfwSetCursorPosCallback(m_window, mouseCursorCallback);
		}
	}
	currentShiftState = glfwGetKey(m_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
	if (currentShiftState && !m_lastShiftState) {
		m_toggleVSync = !m_toggleVSync;
		glfwSwapInterval(m_toggleVSync ? 1 : 0);
		std::cout << m_toggleVSync << std::endl;
	}

	m_lastTabState = currentTabState;
	m_lastShiftState = currentShiftState;
}

void Window::frameBufferResizeCallback(GLFWwindow* win, int width, int height) {
	AppContext* appContext = static_cast<AppContext*>(glfwGetWindowUserPointer(win));
	glViewport(0, 0, width, height);
	appContext->window->setWidth(width);
	appContext->window->setHeight(height);
}

void Window::mouseCursorCallback(GLFWwindow* win, double xpos, double ypos) {
	AppContext* appContext = static_cast<AppContext*>(glfwGetWindowUserPointer(win));
	appContext->player->mouseProcessInput(xpos, ypos);
}

void Window::mouseScrollCallback(GLFWwindow* win, double xoffset, double yoffset) {
	AppContext* appContext = static_cast<AppContext*>(glfwGetWindowUserPointer(win));
	appContext->camera->scrollProcessInput(xoffset, yoffset);
}

void Window::mouseInputCallback(GLFWwindow* win, int button, int action, int mods) {
	AppContext* appContext = static_cast<AppContext*>(glfwGetWindowUserPointer(win));
	appContext->player->mouseButtonProcessInput(button, action, mods);
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