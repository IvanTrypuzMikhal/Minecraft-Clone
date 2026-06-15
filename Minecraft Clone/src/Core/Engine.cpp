#include "Engine.h"

Engine::Engine() {
	
	m_window = std::make_unique<Window>(Globals::HEIGHT, Globals::WIDTH, "Minecraft");

	ResourceManager::loadShaderProgram("worldShader", "src/Rendering/Shaders/vertex.vert", "src/Rendering/Shaders/fragment.frag");
	ResourceManager::loadShaderProgram("textShader", "src/Rendering/Shaders/text.vert", "src/Rendering/Shaders/text.frag");
	ResourceManager::loadShaderProgram("cubeSelectionShader", "src/Rendering/Shaders/cubeSelection.vert", "src/Rendering/Shaders/cubeSelection.frag");
	ResourceManager::loadShaderProgram("uiShader", "src/Rendering/Shaders/UIShader.vert", "src/Rendering/Shaders/UIShader.frag");
	ResourceManager::loadTexture("worldTexture", "src/Assets/Textures/textures.png", GL_RGBA);
	ResourceManager::loadTexture("uiTexture", "src/Assets/Textures/gui-atlas.png", GL_RGBA);

	InputManager::Init(m_window->getWindow());

	m_time = Time();
	m_cubeSelection = std::make_unique<CubeSelection>();
	m_uiRenderer = std::make_unique<UIRenderer>();
	m_hud = std::make_unique<HUD>(*m_uiRenderer, *m_window);
	m_world = std::make_unique<World>(ResourceManager::getShaderProgram("worldShader"));
	m_player = std::make_unique<Player>(m_world.get());


	m_context = AppContext{ .window = m_window.get(), .player = m_player.get(), .camera = m_player->getCamera(), .world = m_world.get()};
	glfwSetWindowUserPointer(m_window->getWindow(), &m_context);

}


void Engine::run() {
	Camera* camera = m_player->getCamera();

	while (!m_window->shouldClose()) {

		processInput();
		update();
		render();

		m_time.update();
		glfwSwapBuffers(m_window->getWindow());
		glfwPollEvents();
	}
}

void Engine::stateUpdate() {
	if (m_lastTabState == false && InputManager::KeyPressed(InputManager::GameState::PAUSE_MENU)) {
		m_tabPressed = true;
		m_lastTabState = true;
		glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}
	else if (m_lastTabState == true && InputManager::KeyPressed(InputManager::GameState::PAUSE_MENU)) {
		m_tabPressed = false;
		m_lastTabState = false;
		glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (InputManager::KeyPressed(InputManager::GameState::CLOSE_CURRENT_MENU)) {
		m_tabPressed = false;
		m_lastTabState = false;
		glfwSetWindowShouldClose(m_window->getWindow(), true);
	}
}

void Engine::update() {
	Camera* camera = m_player->getCamera();

	m_world->updateCameraPosition(camera->getCameraPosition());
	m_world->updateWorldState();
	
	m_player->update(m_time.getDelta());

	stateUpdate();
}

void Engine::render() {
	Camera* camera = m_player->getCamera();

	// Clear the screen
	glClearColor(0.482f, 0.647f, 1.000f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up matrices
	ResourceManager::getShaderProgram("worldShader")->use();
	camera->setView(*ResourceManager::getShaderProgram("worldShader"));
	glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), static_cast<float>(m_window->getWidth()) / static_cast<float>(m_window->getHeight()), 0.05f, 500.0f);

	// Render the world
	ResourceManager::getTexture("worldTexture")->setTexture();
	m_world->renderWorld(projection);

	// Render the cube selection outline if a block is hit
	BlockHit hit;
	if (Raycaster::traceRay(m_world.get(), camera, Globals::INTERACTION_DISTANCE, hit)) {
		m_cubeSelection->renderOutline(hit.x, hit.y, hit.z, projection, camera->view());
	}

	// Render UI elements
	m_hud->render();
	// TODO: Horrendous performance reduction when rendering fuking text. 
	// Gotta change this. Will leave it for now. Just for testing purposes.
	//debugUI.renderText(world , player.getCamera(), window, time);
}

void Engine::processInput() {
	InputManager::Update();

	if (!m_tabPressed) {
		m_player->keyboardProcessInput();
		m_player->mouseButtonProcessInput();
		m_player->mouseProcessInput();
		m_player->getCamera()->scrollProcessInput();
	}
	
}