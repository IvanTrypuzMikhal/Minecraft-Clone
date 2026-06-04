#include "pch.h"

int main() {
	Window window(Globals::HEIGHT, Globals::WIDTH, "Minecraft");
	Camera cam  = Camera();
	ShaderProgram shaderProgram("vertex.vert", "fragment.frag");
	ShaderProgram textShaderProgram("text.vert", "text.frag");
	Texture texture("textures.png", GL_RGBA);
	Time time;
	CubeSelection cubeSelection;
	UIRenderer uiRender;
	HUD hud(uiRender, window);
	DebugUI debugUI;

	auto world = std::make_unique<World>(&shaderProgram);
	
	AppContext context{ &window, &cam, world.get()};
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
		
		BlockHit hit;
		if (Raycaster::traceRay(world.get(), cam, Globals::INTERACTION_DISTANCE, hit)) {
			cubeSelection.renderOutline(hit.x, hit.y, hit.z, projection, cam.view());
		}

		hud.render();
		// TODO: Horrendous performance reduction when rendering fuking text. 
		// Gotta change this. Will leave it for now. Just for testing purposes.
		//debugUI.renderText(world , cam, window, time);

		time.update();

 		cam.keyboardProcessInput(window.getWindow(), time.getDelta());
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	return 0;
}