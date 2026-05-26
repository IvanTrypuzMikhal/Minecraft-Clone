#include "Window.h"
#include "ShaderProgram.h"
#include "Vao.h"
#include "Vbo.h"
#include "Camera.h"
#include "Globals.h"
#include "SpaceTransformation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


unsigned int height = 800;
unsigned int width = 800;

int main() {
	Window w(height, width, "Minecraft");
	Camera cam  = Camera();
	SpaceTransformation st(height, width);
	ShaderProgram shaderProgram("vertex.vert", "fragment.frag");
	Vao vao = Vao();
	Vbo vbo(vertices, sizeof(vertices),  3, 0);
	
	while (!w.shouldClose()) {
		w.processInput();
		glm::mat4 view = cam.view();

		glClearColor(0.416f, 0.329f, 0.459f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		st.setModel(shaderProgram);
		st.setProjection(shaderProgram);
		st.setView(shaderProgram, cam);

		shaderProgram.use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cam.processInput(w.getWindow());
		glfwSwapBuffers(w.getWindow());
		glfwPollEvents();
	}

	return 0;
}