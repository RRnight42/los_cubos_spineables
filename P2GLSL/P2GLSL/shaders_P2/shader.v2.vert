#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec3 inNormal;

in vec2 inTexCoord;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

out vec3 color;
out vec3 no;
out vec3 po;
out vec2 tc;

void main()
{
	color = inColor;

	no = normalize((normal * vec4(inNormal, 0)).xyz); //normalizar para valor central
	po = (modelView * vec4(inPos, 1)).xyz;
	tc = inTexCoord;

	// color = shade();
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
