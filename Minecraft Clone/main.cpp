#include "pch.h"


float last_delta = 0.0;
float new_delta = 0.0;


int main() {
	Window window(Globals::HEIGHT, Globals::WIDTH, "Minecraft");
	Camera cam  = Camera();
	ShaderProgram shaderProgram("vertex.vert", "fragment.frag");
	Texture texture("textures.png", GL_RGBA);
	AppContext context{ &window, &cam };

	auto world = std::make_unique<World>(&shaderProgram);
	
	glfwSetWindowUserPointer(window.getWindow(), &context);


	while (!window.shouldClose()) {
		window.processInput();
		glm::mat4 view = cam.view();
		 
		glClearColor(0.482f, 0.647f, 1.000f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.setView(shaderProgram);

		glm::mat4 projection = glm::perspective(glm::radians(cam.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 500.0f);
		
		shaderProgram.use();
		texture.setTexture();
		
		world->updateCameraPosition(cam.getCameraPosition());
		world->updateWorldState();
		world->renderWorld(projection);
		

		last_delta = new_delta;
		new_delta = (float) glfwGetTime();
		
		cam.keyboardProcessInput(window.getWindow(), new_delta - last_delta);
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	return 0;
}