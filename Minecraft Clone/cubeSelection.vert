#version 330 core
layout (location = 0) in vec3 vertex;


uniform vec3 cubePos;
uniform mat4 projection;
uniform mat4 view;

void main()
{	
	gl_Position = projection * view * vec4((vertex * 1.005) + cubePos, 1.0);
}  