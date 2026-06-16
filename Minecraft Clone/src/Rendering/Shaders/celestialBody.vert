#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;

out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool isGlow;

void main()
{
    TexCoord = aTexture;
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    if(isGlow) {
        pos.w *= 0.99999;
    }
    else {
        pos.w *= 0.9;
    }
    gl_Position = vec4(pos.xy, pos.w , pos.w);
}