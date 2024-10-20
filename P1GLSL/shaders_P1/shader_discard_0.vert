#version 330 core

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;
in vec3 inPos;	
in vec3 inNormal;

out vec3 vPos;
out vec3 vNorm;

in vec2 inTexCoord; 
out vec2 vTexCoord; 

void main() {
    vPos = (modelView * vec4(inPos, 1)).xyz;
    vNorm = (normal * vec4(inNormal, 0)).xyz;
    vTexCoord = inTexCoord; // Pasa las coordenadas de textura

    gl_Position = modelViewProj * vec4(inPos, 1.);
}

