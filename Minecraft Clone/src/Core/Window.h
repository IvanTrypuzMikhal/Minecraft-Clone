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

	[[nodiscard]] GLFWwindow* getWindow() const;
	
	[[nodiscard]] bool shouldClose() const;
	
	static void frameBufferResizeCallback(GLFWwindow* win, int width, int height);

	void setWidth(int width);
	void setHeight(int height);

	[[nodiscard]] int getWidth() const;
	[[nodiscard]] int getHeight() const;

private:
	GLFWwindow* m_window;

	bool m_lastTabState = false;
	bool m_lastShiftState = false;
	
	bool m_toggleVSync = true;

	int m_width;
	int m_height;
};
