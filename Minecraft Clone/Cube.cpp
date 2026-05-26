#include "Cube.h"
#include "Vao.h"
#include "Vbo.h"
#include "Globals.h"

// Not finished yet, too tired
void Cube::createCube() const {


}


void Cube::createVAO() {
	Vbo vbo(Globals::vertices, sizeof(Globals::vertices));
	Vao vao(5, m_vertexAttributes);
}