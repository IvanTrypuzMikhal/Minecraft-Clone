#include "Engine.h"



Engine::Engine() :
	m_window{ Window(Globals::HEIGHT, Globals::WIDTH, "Minecraft") },
	m_worldShaderProgram{ std::make_shared<ShaderProgram>("src/Rendering/Shaders/vertex.vert", "src/Rendering/Shaders/fragment.frag") },
	m_textShaderProgram{ std::make_shared<ShaderProgram>("src/Rendering/Shaders/text.vert", "src/Rendering/Shaders/text.frag") },
	m_texture{ std::make_shared<Texture>("src/Assets/Textures/textures.png", GL_RGBA) },
	m_time{ Time() },
	m_cubeSelection{ CubeSelection() },
	m_uiRenderer{ UIRenderer() },
	m_hud{ HUD(m_uiRenderer, m_window) },
	m_world{ std::make_unique<World>(m_worldShaderProgram) },
	m_player{ std::make_unique<Player>(m_world.get()) } {

	m_context = AppContext{ .window = &m_window, .player = m_player.get(), .camera = m_player->getCamera(), .world = m_world.get() };
	glfwSetWindowUserPointer(m_window.getWindow(), &m_context);
}


void Engine::run() {
	Camera* camera = m_player->getCamera();

	while (!m_window.shouldClose()) {

		processInput();
		update();
		render();

		m_time.update();
		glfwSwapBuffers(m_window.getWindow());
		glfwPollEvents();
	}
}


void Engine::update() {
	Camera* camera = m_player->getCamera();

	m_world->updateCameraPosition(camera->getCameraPosition());
	m_world->updateWorldState();
	
	m_player->update(m_time.getDelta());
}

void Engine::render() {
	Camera* camera = m_player->getCamera();

	// Clear the screen
	glClearColor(0.482f, 0.647f, 1.000f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up matrices
	m_worldShaderProgram->use();
	camera->setView(*m_worldShaderProgram);
	glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), static_cast<float>(m_window.getWidth()) / static_cast<float>(m_window.getHeight()), 0.05f, 500.0f);

	// Render the world
	m_texture->setTexture();
	m_world->renderWorld(projection);

	// Render the cube selection outline if a block is hit
	BlockHit hit;
	if (Raycaster::traceRay(m_world.get(), camera, Globals::INTERACTION_DISTANCE, hit)) {
		m_cubeSelection.renderOutline(hit.x, hit.y, hit.z, projection, camera->view());
	}

	// Render UI elements
	m_hud.render();
	// TODO: Horrendous performance reduction when rendering fuking text. 
	// Gotta change this. Will leave it for now. Just for testing purposes.
	//debugUI.renderText(world , player.getCamera(), window, time);
}

void Engine::processInput() {

	m_window.processInput();
	m_player->keyboardProcessInput(m_window.getWindow());

}