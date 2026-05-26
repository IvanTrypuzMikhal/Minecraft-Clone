#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
public:
	Window(int width, int height, const char* title);
	GLFWwindow* getWindow();
	static void frameBufferResizeCallback(GLFWwindow* win, int width, int height);
	void processInput();

private:
	GLFWwindow* m_window;

};
