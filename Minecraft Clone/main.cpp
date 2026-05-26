#include "pch.h"


float last_delta = 0.0;
float new_delta = 0.0;

VertexAttribute position{ 0, 3, 0 };
VertexAttribute texture{ 1, 2, 3 };
std::vector<VertexAttribute> grassBlock{position, texture};

int main() {
	Window window(Globals::height, Globals::width, "Minecraft");
	Camera cam  = Camera();
	ShaderProgram shaderProgram("vertex.vert", "fragment.frag");
	Texture texture("grass-block.png", GL_RGBA);
	Vbo vbo(Globals::vertices, sizeof(Globals::vertices));
	Vao vao(5, grassBlock);
	AppContext context{ &window, &cam };

	glfwSetWindowUserPointer(window.getWindow(), &context);

	
	while (!window.shouldClose()) {
		window.processInput();
		glm::mat4 view = cam.view();

		glClearColor(0.416f, 0.329f, 0.459f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(cam.getFov()), (float)Globals::width / Globals::height, 0.1f, 100.0f);
		cam.setView(shaderProgram);
		shaderProgram.setMat4("model", glm::mat4(1));
		shaderProgram.setMat4("projection", projection);

		texture.setTexture();
		vao.use();
		shaderProgram.use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		last_delta = new_delta;
		new_delta = (float) glfwGetTime();
		
		cam.keyboardProcessInput(window.getWindow(), new_delta - last_delta);
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	return 0;
}