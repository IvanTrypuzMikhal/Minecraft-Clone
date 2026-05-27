#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct AppContext;

class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();

	GLFWwindow* getWindow() const;
	
	bool shouldClose() const;
	
	static void frameBufferResizeCallback(GLFWwindow* win, int width, int height);
	static void mouseCursorCallback(GLFWwindow* win, double xpos, double ypos);
	static void mouseScrollCallback(GLFWwindow*, double xoffset, double yoffset);
	void processInput();

	void setWidth(int width);
	void setHeight(int height);

	int getWidth() const;
	int getHeight() const;

private:
	GLFWwindow* m_window;

	bool m_lastTabState = false;
	
	bool m_toggleVSync = true;

	int m_width;
	int m_height;
};
