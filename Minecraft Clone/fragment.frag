#version 330 core
out vec4 FragColor;

in vec2 TextCoord;

uniform sampler2D ourTexture;

void main()
{
    vec4 texColor = texture(ourTexture, TextCoord);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
} 