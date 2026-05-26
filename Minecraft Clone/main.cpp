#include "Window.h"
#include "ShaderProgram.h"
#include "Vao.h"
#include "Vbo.h"
#include "Camera.h"
#include "Globals.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int height = 800;
unsigned int width = 800;

float last_delta = 0.0;
float new_delta = 0.0;

VertexAttribute position{ 0, 3, 0 };
VertexAttribute texture{ 1, 2, 3 };
std::vector<VertexAttribute> grassBlock{position, texture};

int main() {
	Window w(height, width, "Minecraft");
	Camera cam  = Camera();
	ShaderProgram shaderProgram("vertex.vert", "fragment.frag");
	Texture texture("grass-block.png", GL_RGBA);
	Vbo vbo(vertices, sizeof(vertices));
	Vao vao(5, grassBlock);

	
	
	while (!w.shouldClose()) {
		w.processInput();
		glm::mat4 view = cam.view();

		glClearColor(0.416f, 0.329f, 0.459f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
		cam.setView(shaderProgram);
		shaderProgram.setMat4("model", glm::mat4(1));
		shaderProgram.setMat4("projection", projection);

		texture.setTexture();
		vao.use();
		shaderProgram.use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		last_delta = new_delta;
		new_delta = (float) glfwGetTime();
		
		cam.processInput(w.getWindow(), new_delta - last_delta);
		glfwSwapBuffers(w.getWindow());
		glfwPollEvents();
	}

	return 0;
}