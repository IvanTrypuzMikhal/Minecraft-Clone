#pragma once
#include "Chunk.h"
#include <array>

enum BlockType : unsigned char
{
	Air,
	Grass,
	Dirt,
	Stone,
	Bedrock,
	OakLeaf,
	OakLog,
};

enum BlockFace : unsigned char {
	Front,
	Back,
	Left,
	Right,
	Top,
	Bottom
};

struct AOBlock {
	float x, y, z;
};

struct Vertex {
	float x, y, z;
	float u, v;

	AOBlock side1;
	AOBlock side2;
	AOBlock diagonal;
};

struct FaceData
{
	int dx, dy, dz;
	BlockFace faceDirection;
	std::array<Vertex, 6> vertices;
};

namespace CubeData {
	constexpr std::array<FaceData, 6> CUBE_FACES = {
		{
			// =================================================================
			// FRONT FACE
			// =================================================================
			FaceData{
				.dx = 0, .dy = 0, .dz = -1,
				.faceDirection = BlockFace::Front,
				.vertices = {
					Vertex{	// Top left
						.x = 0.0f, .y = 0.0f, .z = 0.0f, .u = 0.0f, .v = 1.0f,
						.side1 = {.x = -1.0f, .y = 0.0f, .z = 1.0f},
						.side2 = {.x = 0.0f,  .y = 1.0f, .z = 1.0f},
						.diagonal = {.x = -1.0f, .y = 1.0f, .z = 1.0f}
					},
					Vertex{	// Bottom left
						.x = 0.0f, .y = 1.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f,
						.side1 = {.x = -1.0f, .y = 0.0f,  .z = 1.0f},
						.side2 = {.x = 0.0f,  .y = -1.0f, .z = 1.0f},
						.diagonal = {.x = -1.0f, .y = -1.0f, .z = 1.0f}
					},
					Vertex{	// Top right
						.x = 1.0f, .y = 0.0f, .z = 0.0f, .u = 1.0f, .v = 1.0f,
						.side1 = {.x = 1.0f, .y = 0.0f, .z = 1.0f},
						.side2 = {.x = 0.0f, .y = 1.0f, .z = 1.0f},
						.diagonal = {.x = 1.0f, .y = 1.0f, .z = 1.0f}
					},
					Vertex{	// Bottom left (Duplicate for triangle 2)
						.x = 0.0f, .y = 1.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f,
						.side1 = {.x = -1.0f, .y = 0.0f,  .z = 1.0f},
						.side2 = {.x = 0.0f,  .y = -1.0f, .z = 1.0f},
						.diagonal = {.x = -1.0f, .y = -1.0f, .z = 1.0f}
					},
					Vertex{	// Top right (Duplicate for triangle 2)
						.x = 1.0f, .y = 0.0f, .z = 0.0f, .u = 1.0f, .v = 1.0f,
						.side1 = {.x = 1.0f, .y = 0.0f, .z = 1.0f},
						.side2 = {.x = 0.0f, .y = 1.0f, .z = 1.0f},
						.diagonal = {.x = 1.0f, .y = 1.0f, .z = 1.0f}
					},
					Vertex{	// Bottom right
						.x = 1.0f, .y = 1.0f, .z = 0.0f, .u = 1.0f, .v = 0.0f,
						.side1 = {.x = 1.0f, .y = 0.0f,  .z = 1.0f},
						.side2 = {.x = 0.0f, .y = -1.0f, .z = 1.0f},
						.diagonal = {.x = 1.0f, .y = -1.0f, .z = 1.0f}
					}
				}
			},
		// =================================================================
		// BACK FACE
		// =================================================================
		FaceData{
			.dx = 0, .dy = 0, .dz = 1,
			.faceDirection = BlockFace::Back,
			.vertices = {
				Vertex{	// Top left
					.x = 0.0f, .y = 0.0f, .z = 1.0f, .u = 0.0f, .v = 1.0f,
					.side1 = {.x = -1.0f, .y = 0.0f, .z = -1.0f},
					.side2 = {.x = 0.0f,  .y = 1.0f, .z = -1.0f},
					.diagonal = {.x = -1.0f, .y = 1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom left
					.x = 0.0f, .y = 1.0f, .z = 1.0f, .u = 0.0f, .v = 0.0f,
					.side1 = {.x = -1.0f, .y = 0.0f,  .z = -1.0f},
					.side2 = {.x = 0.0f,  .y = -1.0f, .z = -1.0f},
					.diagonal = {.x = -1.0f, .y = -1.0f, .z = -1.0f}
				},
				Vertex{	// Top right
					.x = 1.0f, .y = 0.0f, .z = 1.0f, .u = 1.0f, .v = 1.0f,
					.side1 = {.x = 1.0f, .y = 0.0f, .z = -1.0f},
					.side2 = {.x = 0.0f, .y = 1.0f, .z = -1.0f},
					.diagonal = {.x = 1.0f, .y = 1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom left
					.x = 0.0f, .y = 1.0f, .z = 1.0f, .u = 0.0f, .v = 0.0f,
					.side1 = {.x = -1.0f, .y = 0.0f,  .z = -1.0f},
					.side2 = {.x = 0.0f,  .y = -1.0f, .z = -1.0f},
					.diagonal = {.x = -1.0f, .y = -1.0f, .z = -1.0f}
				},
				Vertex{	// Top right
					.x = 1.0f, .y = 0.0f, .z = 1.0f, .u = 1.0f, .v = 1.0f,
					.side1 = {.x = 1.0f, .y = 0.0f, .z = -1.0f},
					.side2 = {.x = 0.0f, .y = 1.0f, .z = -1.0f},
					.diagonal = {.x = 1.0f, .y = 1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom right
					.x = 1.0f, .y = 1.0f, .z = 1.0f, .u = 1.0f, .v = 0.0f,
					.side1 = {.x = 1.0f, .y = 0.0f,  .z = -1.0f},
					.side2 = {.x = 0.0f, .y = -1.0f, .z = -1.0f},
					.diagonal = {.x = 1.0f, .y = -1.0f, .z = -1.0f}
				}
			}
		},
		// =================================================================
		// RIGHT FACE
		// =================================================================
		FaceData{
			.dx = 1, .dy = 0, .dz = 0,
			.faceDirection = BlockFace::Right,
			.vertices = {
				Vertex{	// Top left
					.x = 1.0f, .y = 0.0f, .z = 0.0f, .u = 0.0f, .v = 1.0f,
					.side1 = {.x = 1.0f, .y = 0.0f, .z = 1.0f},
					.side2 = {.x = 1.0f, .y = 1.0f, .z = 0.0f},
					.diagonal = {.x = 1.0f, .y = 1.0f, .z = 1.0f}
				},
				Vertex{	// Bottom left
					.x = 1.0f, .y = 1.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f,
					.side1 = {.x = 1.0f, .y = 0.0f,  .z = 1.0f},
					.side2 = {.x = 1.0f, .y = -1.0f, .z = 0.0f},
					.diagonal = {.x = 1.0f, .y = -1.0f, .z = 1.0f}
				},
				Vertex{	// Top right
					.x = 1.0f, .y = 0.0f, .z = 1.0f, .u = 1.0f, .v = 1.0f,
					.side1 = {.x = 1.0f, .y = 0.0f, .z = -1.0f},
					.side2 = {.x = 1.0f, .y = 1.0f, .z = 0.0f},
					.diagonal = {.x = 1.0f, .y = 1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom left
					.x = 1.0f, .y = 1.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f,
					.side1 = {.x = 1.0f, .y = 0.0f,  .z = 1.0f},
					.side2 = {.x = 1.0f, .y = -1.0f, .z = 0.0f},
					.diagonal = {.x = 1.0f, .y = -1.0f, .z = 1.0f}
				},
				Vertex{	// Top right
					.x = 1.0f, .y = 0.0f, .z = 1.0f, .u = 1.0f, .v = 1.0f,
					.side1 = {.x = 1.0f, .y = 0.0f, .z = -1.0f},
					.side2 = {.x = 1.0f, .y = 1.0f, .z = 0.0f},
					.diagonal = {.x = 1.0f, .y = 1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom right
					.x = 1.0f, .y = 1.0f, .z = 1.0f, .u = 1.0f, .v = 0.0f,
					.side1 = {.x = 1.0f, .y = 0.0f,  .z = -1.0f},
					.side2 = {.x = 1.0f, .y = -1.0f, .z = 0.0f},
					.diagonal = {.x = 1.0f, .y = -1.0f, .z = -1.0f}
				}
			}
		},
		// =================================================================
		// LEFT FACE
		// =================================================================
		FaceData{
			.dx = -1, .dy = 0, .dz = 0,
			.faceDirection = BlockFace::Left,
			.vertices = {
				Vertex{	// Top left
					.x = 0.0f, .y = 0.0f, .z = 1.0f, .u = 0.0f, .v = 1.0f,
					.side1 = {.x = -1.0f, .y = 0.0f, .z = -1.0f},
					.side2 = {.x = -1.0f, .y = 1.0f, .z = 0.0f},
					.diagonal = {.x = -1.0f, .y = 1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom left
					.x = 0.0f, .y = 1.0f, .z = 1.0f, .u = 0.0f, .v = 0.0f,
					.side1 = {.x = -1.0f, .y = 0.0f,  .z = -1.0f},
					.side2 = {.x = -1.0f, .y = -1.0f, .z = 0.0f},
					.diagonal = {.x = -1.0f, .y = -1.0f, .z = -1.0f}
				},
				Vertex{	// Top right
					.x = 0.0f, .y = 0.0f, .z = 0.0f, .u = 1.0f, .v = 1.0f,
					.side1 = {.x = -1.0f, .y = 0.0f, .z = 1.0f},
					.side2 = {.x = -1.0f, .y = 1.0f, .z = 0.0f},
					.diagonal = {.x = -1.0f, .y = 1.0f, .z = 1.0f}
				},
				Vertex{	// Bottom left
					.x = 0.0f, .y = 1.0f, .z = 1.0f, .u = 0.0f, .v = 0.0f,
					.side1 = {.x = -1.0f, .y = 0.0f,  .z = -1.0f},
					.side2 = {.x = -1.0f, .y = -1.0f, .z = 0.0f},
					.diagonal = {.x = -1.0f, .y = -1.0f, .z = -1.0f}
				},
				Vertex{	// Top right
					.x = 0.0f, .y = 0.0f, .z = 0.0f, .u = 1.0f, .v = 1.0f,
					.side1 = {.x = -1.0f, .y = 0.0f, .z = 1.0f},
					.side2 = {.x = -1.0f, .y = 1.0f, .z = 0.0f},
					.diagonal = {.x = -1.0f, .y = 1.0f, .z = 1.0f}
				},
				Vertex{	// Bottom right
					.x = 0.0f, .y = 1.0f, .z = 0.0f, .u = 1.0f, .v = 0.0f,
					.side1 = {.x = -1.0f, .y = 0.0f,  .z = 1.0f},
					.side2 = {.x = -1.0f, .y = -1.0f, .z = 0.0f},
					.diagonal = {.x = -1.0f, .y = -1.0f, .z = 1.0f}
				}
			}
		},
		// =================================================================
		// TOP FACE
		// =================================================================
		FaceData{
			.dx = 0, .dy = -1, .dz = 0,
			.faceDirection = BlockFace::Top,
			.vertices = {
				Vertex{	// Top left
					.x = 0.0f, .y = 0.0f, .z = 1.0f, .u = 0.0f, .v = 1.0f,
					.side1 = {.x = -1.0f, .y = 1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f,  .y = 1.0f, .z = -1.0f},
					.diagonal = {.x = -1.0f, .y = 1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom left
					.x = 0.0f, .y = 0.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f,
					.side1 = {.x = -1.0f, .y = 1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f,  .y = 1.0f, .z = 1.0f},
					.diagonal = {.x = -1.0f, .y = 1.0f, .z = 1.0f}
				},
				Vertex{	// Top right
					.x = 1.0f, .y = 0.0f, .z = 1.0f, .u = 1.0f, .v = 1.0f,
					.side1 = {.x = 1.0f, .y = 1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f, .y = 1.0f, .z = -1.0f},
					.diagonal = {.x = 1.0f, .y = 1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom left
					.x = 0.0f, .y = 0.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f,
					.side1 = {.x = -1.0f, .y = 1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f,  .y = 1.0f, .z = 1.0f},
					.diagonal = {.x = -1.0f, .y = 1.0f, .z = 1.0f}
				},
				Vertex{	// Top right
					.x = 1.0f, .y = 0.0f, .z = 1.0f, .u = 1.0f, .v = 1.0f,
					.side1 = {.x = 1.0f, .y = 1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f, .y = 1.0f, .z = -1.0f},
					.diagonal = {.x = 1.0f, .y = 1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom right
					.x = 1.0f, .y = 0.0f, .z = 0.0f, .u = 1.0f, .v = 0.0f,
					.side1 = {.x = 1.0f, .y = 1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f, .y = 1.0f, .z = 1.0f},
					.diagonal = {.x = 1.0f, .y = 1.0f, .z = 1.0f}
				}
			}
		},
		// =================================================================
		// BOTTOM FACE
		// =================================================================
		FaceData{
			.dx = 0, .dy = 1, .dz = 0,
			.faceDirection = BlockFace::Bottom,
			.vertices = {
				Vertex{	// Top left
					.x = 0.0f, .y = 1.0f, .z = 1.0f, .u = 0.0f, .v = 1.0f,
					.side1 = {.x = -1.0f, .y = -1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f,  .y = -1.0f, .z = -1.0f},
					.diagonal = {.x = -1.0f, .y = -1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom left
					.x = 0.0f, .y = 1.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f,
					.side1 = {.x = -1.0f, .y = -1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f,  .y = -1.0f, .z = 1.0f},
					.diagonal = {.x = -1.0f, .y = -1.0f, .z = 1.0f}
				},
				Vertex{	// Top right
					.x = 1.0f, .y = 1.0f, .z = 1.0f, .u = 1.0f, .v = 1.0f,
					.side1 = {.x = 1.0f, .y = -1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f, .y = -1.0f, .z = -1.0f},
					.diagonal = {.x = 1.0f, .y = -1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom left
					.x = 0.0f, .y = 1.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f,
					.side1 = {.x = -1.0f, .y = -1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f,  .y = -1.0f, .z = 1.0f},
					.diagonal = {.x = -1.0f, .y = -1.0f, .z = 1.0f}
				},
				Vertex{	// Top right
					.x = 1.0f, .y = 1.0f, .z = 1.0f, .u = 1.0f, .v = 1.0f,
					.side1 = {.x = 1.0f, .y = -1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f, .y = -1.0f, .z = -1.0f},
					.diagonal = {.x = 1.0f, .y = -1.0f, .z = -1.0f}
				},
				Vertex{	// Bottom right
					.x = 1.0f, .y = 1.0f, .z = 0.0f, .u = 1.0f, .v = 0.0f,
					.side1 = {.x = 1.0f, .y = -1.0f, .z = 0.0f},
					.side2 = {.x = 0.0f, .y = -1.0f, .z = 1.0f},
					.diagonal = {.x = 1.0f, .y = -1.0f, .z = 1.0f}
				}
			}
		}
	}
	};


	constexpr std::array wireframeVertices = {
		// Bottom
		0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Line 1
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, // Line 2
		1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, // Line 3
		0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f, // Line 4
												
		// Top									
		0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f, // Line 5
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f, // Line 6
		1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f, // Line 7
		0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Line 8
												
		// Columns								
		0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Line 9
		1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f, // Line 10
		1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f, // Line 11
		0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f  // Line 12
	};
}

