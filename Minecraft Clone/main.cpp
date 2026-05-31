#include "pch.h"

int main() {
	Window window(Globals::HEIGHT, Globals::WIDTH, "Minecraft");
	Camera cam  = Camera();
	ShaderProgram shaderProgram("vertex.vert", "fragment.frag");
	ShaderProgram textShaderProgram("text.vert", "text.frag");
	Texture texture("textures.png", GL_RGBA);
	AppContext context{ &window, &cam };
	Time time;

	DebugUI debugUI;

	auto world = std::make_unique<World>(&shaderProgram);
	
	glfwSetWindowUserPointer(window.getWindow(), &context);


	while (!window.shouldClose()) {
		window.processInput();
		glm::mat4 view = cam.view();
		 
		glClearColor(0.482f, 0.647f, 1.000f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();

		cam.setView(shaderProgram);
		
		glm::mat4 projection = glm::perspective(glm::radians(cam.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 500.0f);
		
		texture.setTexture();
		
		world->updateCameraPosition(cam.getCameraPosition());
		world->updateWorldState();
		world->renderWorld(projection);

		debugUI.renderText(cam, window, time);

		time.update();

 		cam.keyboardProcessInput(window.getWindow(), time.getDelta());
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	return 0;
}