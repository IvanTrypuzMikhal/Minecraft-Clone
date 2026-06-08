#include "pch.h"

int main() {
	Window window(Globals::HEIGHT, Globals::WIDTH, "Minecraft");
	ShaderProgram shaderProgram("src/Rendering/Shaders/vertex.vert", "src/Rendering/Shaders/fragment.frag");
	ShaderProgram textShaderProgram("src/Rendering/Shaders/text.vert", "src/Rendering/Shaders/text.frag");
	Texture texture("src/Assets/Textures/textures.png", GL_RGBA);
	Time time;
	CubeSelection cubeSelection;
	UIRenderer uiRender;
	HUD hud(uiRender, window);
	DebugUI debugUI;

	auto world = std::make_unique<World>(&shaderProgram);
	Player player(world.get());
	Camera* cam = player.getCamera();



	AppContext context{.window = &window, .player = &player , .camera = cam, .world = world.get()};
	glfwSetWindowUserPointer(window.getWindow(), &context);

	while (!window.shouldClose()) {
		window.processInput();
		glm::mat4 view = cam->view();
		 
		glClearColor(0.482f, 0.647f, 1.000f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		player.keyboardProcessInput(window.getWindow());


		shaderProgram.use();

		cam->setView(shaderProgram);
		
		glm::mat4 projection = glm::perspective(glm::radians(cam->getFov()), static_cast<float>(window.getWidth()) / static_cast<float>(window.getHeight()), 0.05f, 500.0f);
		
		texture.setTexture();
		
		world->updateCameraPosition(cam->getCameraPosition());
		world->updateWorldState();
		world->renderWorld(projection);
		
		
		BlockHit hit;
		if (Raycaster::traceRay(world.get(), cam, Globals::INTERACTION_DISTANCE, hit)) {
			cubeSelection.renderOutline(hit.x, hit.y, hit.z, projection, cam->view());
		}
		
		hud.render();
		// TODO: Horrendous performance reduction when rendering fuking text. 
		// Gotta change this. Will leave it for now. Just for testing purposes.
		//debugUI.renderText(world , player.getCamera(), window, time);

		player.update(time.getDelta());

		time.update();
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	return 0;
}