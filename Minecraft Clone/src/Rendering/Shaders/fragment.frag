#version 330 core
out vec4 FragColor;

in vec2 TextCoord;
in float aoIntensity;
in float ambientLightFactorIntensity;

uniform sampler2D ourTexture;
uniform float timeLightFactor;

void main()
{
    // Lighting should only affect surface blocks.
    // One solution i have in mind is passing a bit to indicate if the block is a surface block or not.
    // For now, we will just use the ambient light factor to determine if the block is a surface block or not.
    // The problem with passing a bit of is surface or not is that it correlates with the lighting factor of the block so idk it should be fine. Dont know yet.

    float ambientLightIntensity = ambientLightFactorIntensity > 4 ? ambientLightFactorIntensity * timeLightFactor : ambientLightFactorIntensity;
    float finalLight = ambientLightIntensity / 15.0;

    vec4 texColor = texture(ourTexture, TextCoord);
    if(texColor.a < 0.5)
        discard;

    FragColor = vec4(texColor.rgb * aoIntensity * finalLight, 1.0f);
} 