#pragma once
#include "Chunk.h"

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


struct FaceData
{
	int dx, dy, dz;
	BlockFace faceDirection;
	float vertices[6][5];
};

namespace CubeData {
	constexpr FaceData CUBE_FACES[6] = {
		{
			// Frontal face
			0, 0, -1,
			BlockFace::Front,
			{	//{x, y, z, u_isMin, z_isMax}
				{0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
				{0.0f, -1.0f, 0.0f, 0.0f, 0.0f},
				{1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
				{0.0f, -1.0f, 0.0f, 0.0f, 0.0f},
				{1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
				{1.0f, -1.0f, 0.0f, 1.0f, 0.0f}
			}
		},
		{
			// Back face
			0, 0, +1,
			BlockFace::Back,
			{	
				{0.0f,  0.0f, -1.0f, 0.0f, 1.0f},
				{0.0f, -1.0f, -1.0f, 0.0f, 0.0f},
				{1.0f,  0.0f, -1.0f, 1.0f, 1.0f},
				{0.0f, -1.0f, -1.0f, 0.0f, 0.0f},
				{1.0f,  0.0f, -1.0f, 1.0f, 1.0f},
				{1.0f, -1.0f, -1.0f, 1.0f, 0.0f}
			},
		},
		{
			// Right face
			1, 0, 0,
			BlockFace::Right,
			{	
				{1.0f,  0.0f, 0.0f, 0.0f, 1.0f},
				{1.0f, -1.0f, 0.0f, 0.0f, 0.0f},
				{1.0f,  0.0f, -1.0f, 1.0f, 1.0f},
				{1.0f, -1.0f, 0.0f, 0.0f, 0.0f},
				{1.0f,  0.0f, -1.0f, 1.0f, 1.0f},
				{1.0f, -1.0f, -1.0f, 1.0f, 0.0f}
			},
		},
		{
			// Left face
			-1, 0, 0,
			BlockFace::Left,
			{	//{x, y, z, u_isMin, z_isMax}
				{0.0f,  0.0f, -1.0f, 0.0f, 1.0f},
				{0.0f, -1.0f, -1.0f, 0.0f, 0.0f},
				{0.0f,  0.0f, 0.0f, 1.0f, 1.0f},
				{0.0f, -1.0f, -1.0f, 0.0f, 0.0f},
				{0.0f,  0.0f, 0.0f, 1.0f, 1.0f},
				{0.0f, -1.0f, 0.0f, 1.0f, 0.0f}
			}
		},
		{
			// Top face
			0, -1, 0,
			BlockFace::Top,
			{	//{x, y, z, u_isMin, z_isMax}
				{0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
				{0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
				{1.0f, 0.0f, -1.0f, 1.0f, 1.0f},
				{0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
				{1.0f, 0.0f, -1.0f, 1.0f, 1.0f},
				{1.0f, 0.0f, 0.0f, 1.0f, 0.0f}
			},
		},
		{
			// Bottom face
			0, 1, 0,
			BlockFace::Bottom,
			{	//{x, y, z, u_isMin, z_isMax}
				{0.0f, -1.0f, -1.0f, 0.0f, 1.0f},
				{0.0f, -1.0f, 0.0f, 0.0f, 0.0f},
				{1.0f, -1.0f, -1.0f, 1.0f, 1.0f},
				{0.0f, -1.0f, 0.0f, 0.0f, 0.0f},
				{1.0f, -1.0f, -1.0f, 1.0f, 1.0f},
				{1.0f, -1.0f, 0.0f, 1.0f, 0.0f}
			},
		} 
	};
}

