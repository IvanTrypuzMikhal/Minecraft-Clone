#version 330 core
out vec4 FragColor;

in vec2 TextCoord;
in float aoIntensity;

uniform sampler2D ourTexture;

void main()
{
    vec4 texColor = texture(ourTexture, TextCoord);
    if(texColor.a < 0.1)
        discard;

    FragColor = vec4(texColor.rgb * aoIntensity * 0.9, 1.0f);
} 