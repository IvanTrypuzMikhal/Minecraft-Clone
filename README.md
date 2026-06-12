# Minecraft Clone

A Minecraft clone built from scratch in C++ and OpenGL.  
Started as a learning project.

<img width="426" height="240" alt="MinecraftCloneGIFDemo" src="https://github.com/user-attachments/assets/7cd812fc-5626-48bc-9e3e-c29bc5b1aa64" />

## Features

- Chunk-based world generation with procedural terrain (FastNoiseLite)
- Threaded terrain and mesh loading — terrain thread + mesh thread, lock-free queues
- Vertex packing for minimal GPU memory usage
- Ambient occlusion per vertex
- Block placement and breaking with border-chunk rebuilding
- Raycasting block selection with outline rendering
- HUD rendering via sprite batching
- Physics with per-axis AABB collision resolution
- Crouch with edge detection (shift mechanic)

## Architecture
The project is structured around a producer-consumer pipeline:

1. **TerrainThread** generates chunk block data off the main thread
2. **MeshThread** builds vertex buffers once terrain + neighbors are ready  
3. **Main thread** uploads finished meshes to the GPU and renders

Chunk states: `TERRAIN_READY → DECORATED → MESH_BUILDING → MESH_READY`


## Build
### Visual Studio (Windows)
Open `Minecraft Clone.sln` and build. Dependencies are linked via project properties.

### CMake (Linux / other)
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./MinecraftClone
```
**Dependencies:** GLFW3, FreeType, GLM, OpenGL

On Fedora:
```bash
sudo dnf install glfw-devel freetype-devel glm-devel mesa-libGL-devel
```
On Ubuntu/Debian:
```bash
sudo apt install libglfw3-dev libfreetype-dev libglm-dev
```

## TODO
- Chunk unloading
- Skybox
- Lighting (torches / light propagation)
- Hotbar block selection
- Health and hunger mechanics
- Inventory
- Items
- Mobs
- Improve terrain generation with bioms, caves, rivers, water, etc.
