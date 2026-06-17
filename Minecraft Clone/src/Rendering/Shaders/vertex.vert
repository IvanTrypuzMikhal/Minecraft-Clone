#version 330 core
layout (location = 0) in uint packedData;
layout (location = 1) in uint packedAttributes;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 chunkPos;


out vec2 TextCoord;
out float aoIntensity;
out float ambientLightFactorIntensity;

const float TEXTURE_SIZE = 1.0f / 4.0f;

void main(){

	uint x = packedData & 31u;
	uint z = (packedData >> 5u) & 31u;
	uint y = (packedData >> 10u) & 511u;
	uint u = (packedData >> 19u) & 1u;
	uint v = (packedData >> 20u) & 1u;
	uint textureID = (packedData >> 21u) & 255u;
	uint ao = (packedData >> 29u) & 3u;
	uint ambientLightValue = packedAttributes & 15u;

	float aoValues[4] = float[](0.4f, 0.6f, 0.8f, 1.0f);

	vec3 localPosition = vec3(float(x), -float(y), float(z));
    
    vec3 worldPosition = localPosition + chunkPos;
    
    gl_Position = projection * view * vec4(worldPosition, 1.0);

	uint atlasX = textureID % 4u;
	uint atlasY = textureID / 4u;

	float uMin = float(atlasX) * TEXTURE_SIZE;
    float vMin = float(atlasY) * TEXTURE_SIZE;

    float finalU = uMin + (float(u) * TEXTURE_SIZE);
    float finalV = vMin + (float(v) * TEXTURE_SIZE);

    TextCoord = vec2(finalU, finalV); 
	aoIntensity = aoValues[ao];
	ambientLightFactorIntensity = float(ambientLightValue);
}