#include "Window.h"
#include "ShaderProgram.h"
#include "Globals.h"

int main() {
	Window w(800, 800, "Minecraft");
	ShaderProgram shaderProgram("vertex.vert", "fragment.frag");

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!w.shouldClose()) {
		w.processInput();
		
		glClearColor(0.416f, 0.329f, 0.459f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);


		shaderProgram.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(w.getWindow());
		glfwWaitEvents();
	
	}


	return 0;
}