#include "Window.h"


int main() {

	Window w(800, 800, "Minecraft");


	while (!glfwWindowShouldClose(w.getWindow())) {
		
		w.processInput();
		
		glClearColor(0.416f, 0.329f, 0.459f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(w.getWindow());
		glfwWaitEvents();
	
	}


	return 0;
}