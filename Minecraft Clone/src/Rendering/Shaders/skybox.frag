#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform vec3 skyboxColor;
uniform vec3 fogColor;

void main()
{    
    vec3 direction = normalize(TexCoords);

    float horizonFactor = clamp(direction.y, 0.0, 1.0);
    horizonFactor = pow(horizonFactor, 0.6); 

    vec3 finalColor = mix(fogColor, skyboxColor, horizonFactor);

    FragColor = vec4(finalColor, 1.0f);
}