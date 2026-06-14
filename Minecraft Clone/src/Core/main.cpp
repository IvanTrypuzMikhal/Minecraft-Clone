#include "Engine.h"

int main() {

	Engine engine;
	engine.run();

	ResourceManager::clearResources();
	return 0;
}