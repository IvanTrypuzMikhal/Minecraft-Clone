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
	Vertex vertices[6];
};

namespace CubeData {
	constexpr FaceData CUBE_FACES[6] = {
		{
			// Frontal face
			0, 0, -1,
			BlockFace::Front,
			{	//{x, y, z, u_isMin, z_isMax} | Neighbors: LEFT            TOP           DIAGONAL TOP LEFT 
				{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, {-1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}},		// Top left
				//							   |        LEFT                 BOTTOM         DIAGONAL BTOOM LEFT												
				{0.0f, -1.0f, 0.0f, 0.0f, 0.0f, {-1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f}},	// Bottom left
				//                            |		   RIGHT              TOP           DIAGONAL TOP RIGHT 
				{1.0f, 0.0f, 0.0f, 1.0f, 1.0f, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},			// Top right
				//							   |        LEFT                 BOTTOM         DIAGONAL BTOOM LEFT												
				{0.0f, -1.0f, 0.0f, 0.0f, 0.0f, {-1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f}},	// Bottom left
				//                            |		   RIGHT              TOP           DIAGONAL TOP RIGHT 
				{1.0f, 0.0f, 0.0f, 1.0f, 1.0f, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},			// Top right
				//							   |       RIGHT              BOTTOM         DIAGONAL BOTTOM RIGHT
 				{1.0f, -1.0f, 0.0f, 1.0f, 0.0f, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 1.0f}, {1.0f, -1.0f, 1.0f}}		// Bottom right
			}
		},
		{
			// Back face
			0, 0, +1,
			BlockFace::Back,
			{	//								| Neighbors: LEFT            TOP           DIAGONAL TOP LEFT 
				{0.0f,  0.0f, -1.0f, 0.0f, 1.0f, {-1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f}},		// Top left
				//								 |        LEFT                 BOTTOM         DIAGONAL BTOOM LEFT						
				{0.0f, -1.0f, -1.0f, 0.0f, 0.0f, {-1.0f, 0.0f, -1.0f}, {0.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}},	// Bottom left
				//								|		   RIGHT              TOP           DIAGONAL TOP RIGHT 
				{1.0f,  0.0f, -1.0f, 1.0f, 1.0f, {1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}},		// Top right
				//								 |        LEFT                 BOTTOM         DIAGONAL BTOOM LEFT						
				{0.0f, -1.0f, -1.0f, 0.0f, 0.0f, {-1.0f, 0.0f, -1.0f}, {0.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}},	// Bottom left
				//								|		   RIGHT              TOP           DIAGONAL TOP RIGHT 
				{1.0f,  0.0f, -1.0f, 1.0f, 1.0f, {1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}},		// Top right
				//								 |       RIGHT              BOTTOM         DIAGONAL BOTTOM RIGHT
				{1.0f, -1.0f, -1.0f, 1.0f, 0.0f, {1.0f, 0.0f, -1.0f}, {0.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}}		// Bottom right
			},
		},
		{
			// Right face
			1, 0, 0,
			BlockFace::Right,
			{	//								| Neighbors: LEFT          TOP          DIAGONAL TOP LEFT
				{1.0f,  0.0f, 0.0f, 0.0f, 1.0f,	{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},		// Top left
				//								 |      LEFT               BOTTOM         DIAGONAL BTOOM LEFT
				{1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	{1.0f, 0.0f, 1.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 1.0f}},		// Bottom left
				//								|       RIGHT                TOP            DIAGONAL TOP RIGHT
				{1.0f,  0.0f, -1.0f, 1.0f, 1.0f, {1.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, -1.0f}},		// Top right
				//								 |      LEFT               BOTTOM         DIAGONAL BTOOM LEFT
				{1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	{1.0f, 0.0f, 1.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 1.0f}},		// Bottom left
				//								|       RIGHT                TOP            DIAGONAL TOP RIGHT
				{1.0f,  0.0f, -1.0f, 1.0f, 1.0f, {1.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, -1.0f}},		// Top right
				//								 |      RIGHT              BOTTOM           DIAGONAL BOTTOM RIGHT
				{1.0f, -1.0f, -1.0f, 1.0f, 0.0f, {1.0f, 0.0f, -1.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, -1.0f}}	// Bottom right
			},
		},
		{
			// Left face
			-1, 0, 0,
			BlockFace::Left,
			{	//{x, y, z, u_isMin, z_isMax}   | Neighbors: LEFT          TOP          DIAGONAL TOP LEFT
				{0.0f,  0.0f, -1.0f, 0.0f, 1.0f, {-1.0f, 0.0f, -1.0f}, {-1.0f, 1.0f, 0.0f}, {-1.0f, 1.0f, -1.0f}},		// Top left
				//								|		LEFT               BOTTOM         DIAGONAL BTOOM LEFT
				{0.0f, -1.0f, -1.0f, 0.0f, 0.0f, {-1.0f, 0.0f, -1.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, -1.0f}},		// Bottom left
				//							   |		RIGHT                TOP            DIAGONAL TOP RIGHT
				{0.0f,  0.0f, 0.0f, 1.0f, 1.0f,	{-1.0f, 0.0f, 1.0f}, {-1.0f, 1.0f, 0.0f}, {-1.0f, 1.0f, 1.0f}},		// Top right
				//								|		LEFT               BOTTOM         DIAGONAL BTOOM LEFT
				{0.0f, -1.0f, -1.0f, 0.0f, 0.0f, {-1.0f, 0.0f, -1.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, -1.0f}},		// Bottom left
				//							   |	   RIGHT                TOP            DIAGONAL TOP RIGHT
				{0.0f,  0.0f, 0.0f, 1.0f, 1.0f, {-1.0f, 0.0f, 1.0f}, {-1.0f, 1.0f, 0.0f}, {-1.0f, 1.0f, 1.0f}},		// Top right
				//							   |		RIGHT              BOTTOM           DIAGONAL BOTTOM RIGHT
				{0.0f, -1.0f, 0.0f, 1.0f, 0.0f,	{-1.0f, 0.0f, 1.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 1.0f}}	// Bottom right
			}
		},
		{
			// Top face
			0, -1, 0,
			BlockFace::Top,
			{	//{x, y, z, u_isMin, z_isMax}  | Neighbors: LEFT          TOP          DIAGONAL TOP LEFT  
				{0.0f, 0.0f, -1.0f, 0.0f, 1.0f, {-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f}},	// Top left
				//							   |		  LEFT               BOTTOM         DIAGONAL BTOOM LEFT
				{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, {-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}},		// Bottom left
				//							   |	   RIGHT                TOP            DIAGONAL TOP RIGHT
				{1.0f, 0.0f, -1.0f, 1.0f, 1.0f,	{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}},		// Top right
				//							   |		  LEFT               BOTTOM         DIAGONAL BTOOM LEFT
				{0.0f, 0.0f, 0.0f, 0.0f, 0.0f,{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}},		// Bottom left
				//							   |	   RIGHT                TOP            DIAGONAL TOP RIGHT
				{1.0f, 0.0f, -1.0f, 1.0f, 1.0f,{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}},		// Top right
				//							   |		  RIGHT              BOTTOM           DIAGONAL BOTTOM RIGHT
				{1.0f, 0.0f, 0.0f, 1.0f, 0.0f, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}			// Bottom right
			},
		},
		{
			// Bottom face
			0, 1, 0,
			BlockFace::Bottom,	
			{	//{x, y, z, u_isMin, z_isMax}   | Neighbors: LEFT          TOP          DIAGONAL TOP LEFT  
				{0.0f, -1.0f, -1.0f, 0.0f, 1.0f, {-1.0f, -1.0f, 0.0f}, {0.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}},	// Top left
				//								|		LEFT               BOTTOM         DIAGONAL BTOOM LEFT
				{0.0f, -1.0f, 0.0f, 0.0f, 0.0f,	{-1.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f}},		// Bottom left
				//								|		RIGHT                TOP            DIAGONAL TOP RIGHT
				{1.0f, -1.0f, -1.0f, 1.0f, 1.0f, {1.0f, -1.0f, 0.0f}, {0.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}},		// Top right
				//								|		LEFT               BOTTOM         DIAGONAL BTOOM LEFT
				{0.0f, -1.0f, 0.0f, 0.0f, 0.0f,	{-1.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f}},		// Bottom left
				//								|		RIGHT                TOP            DIAGONAL TOP RIGHT
				{1.0f, -1.0f, -1.0f, 1.0f, 1.0f, {1.0f, -1.0f, 0.0f}, {0.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}},		// Top right
				//								|		RIGHT              BOTTOM           DIAGONAL BOTTOM RIGHT
				{1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	{1.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 1.0f}, {1.0f, -1.0f, 1.0f}}			// Bottom right
			},
		} 
	};
}

