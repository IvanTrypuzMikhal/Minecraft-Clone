#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();

	GLFWwindow* getWindow() const;
	
	bool shouldClose() const;
	
	static void frameBufferResizeCallback(GLFWwindow* win, int width, int height);
	void processInput() const;

private:
	GLFWwindow* m_window;

};
