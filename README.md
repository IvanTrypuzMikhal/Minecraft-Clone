# Minecraft Clone

A Minecraft clone built from scratch in C++ and OpenGL.  
Started as a learning project.

![gameplay](https://imgur.com/a/2SThF3X)

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
```<img width="426" height="240" alt="MinecraftCloneGIFDemo" src="https://github.com/user-attachments/assets/8932cc09-9091-426a-8d32-0d1d09015f30" />

**Dependencies:** GLFW3, FreeType, GLM, OpenGL

On Fedora:
```bash
sudo dnf install glfw-devel freetype-devel glm-devel mesa-libGL-devel<img width="426" height="240" alt="MinecraftCloneGIFDemo" src="https://github.com/user-attachments/assets/b6a5487b-d6c4-4d11-ba67-6f6ca3230081" />
<img width="426" height="240" alt="MinecraftCloneGIFDemo" src="https://github.com/user-attachments/assets/9f949d39-0372-4c32-8dfd-800fee28bbf2" />


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
