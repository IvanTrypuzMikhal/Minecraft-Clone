#include "pch.h"


float last_delta = 0.0;
float new_delta = 0.0;


int main() {
	Window window(Globals::height, Globals::width, "Minecraft");
	Camera cam  = Camera();
	ShaderProgram shaderProgram("vertex.vert", "fragment.frag");
	Texture texture("grass-block.png", GL_RGBA);
	AppContext context{ &window, &cam };

	auto chunk = std::make_unique<Chunk>(&texture, &shaderProgram);
	
	glfwSetWindowUserPointer(window.getWindow(), &context);

	
	while (!window.shouldClose()) {
		window.processInput();
		glm::mat4 view = cam.view();
		 
		glClearColor(0.416f, 0.329f, 0.459f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.setView(shaderProgram);

		glm::mat4 projection = glm::perspective(glm::radians(cam.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1);
		
		
		chunk->render(projection, model);
		

		last_delta = new_delta;
		new_delta = (float) glfwGetTime();
		
		cam.keyboardProcessInput(window.getWindow(), new_delta - last_delta);
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	return 0;
}