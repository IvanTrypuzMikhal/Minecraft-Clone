#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;

out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool isGlow;
uniform bool isMoon;
uniform int moonPhase;

void main()
{
    if(isMoon) {
        float atlasX = float(moonPhase % 4);
        float atlasY = float(1 - (moonPhase / 4)); 

        float finalU = (atlasX * 0.25) + (aTexture.x * 0.25);
        float finalV = (atlasY * 0.5) + (aTexture.y * 0.5);
 
        TexCoord = vec2(finalU, finalV);
    } 
    else {
        TexCoord = aTexture;
    }
    
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = vec4(pos.xy, isGlow ? pos.w * 0.99999 : pos.w * 0.9999, pos.w);
}