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

	Cube grassBlock(&texture, &shaderProgram);

	glfwSetWindowUserPointer(window.getWindow(), &context);

	
	while (!window.shouldClose()) {
		window.processInput();
		glm::mat4 view = cam.view();

		glClearColor(0.416f, 0.329f, 0.459f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.setView(shaderProgram);

		glm::mat4 projection = glm::perspective(glm::radians(cam.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);

		for(int y = 0; y < 100; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * 0.4f, y * -0.4f, z * 0.4f));
					grassBlock.renderCube(projection, model);
				}
			}
		}
		vao.use();
		

		last_delta = new_delta;
		new_delta = (float) glfwGetTime();
		
		cam.keyboardProcessInput(window.getWindow(), new_delta - last_delta);
		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	return 0;
}