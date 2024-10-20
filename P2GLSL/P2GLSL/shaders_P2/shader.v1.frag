#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 inNormal;
in vec3 inPos;

// Fuente de luz
//uniform vec3 Ia; - Propiedad escena.
vec3 Ia = vec3(0.1);
//uniform vec3 Il; - Propiedad luz.
vec3 Il = vec3(1);
//uniform vec3 pl;
vec3 pl = vec3(0);

// Objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
float n;

in vec3 po;
in vec3 no;

vec3 shade() 
{
	vec3 color = vec3(0);

	//amb
	color += Ia * Ka;

	//diff
	vec3 N = normalize(no);
	vec3 L = normalize(pl-po);

	color += Il * Kd * (max(dot(N, L), 0));

	//spec
	vec3 V = normalize(-po);
	vec3 R = reflect(-L, N);
	
	color += Il * Ks * pow(max(dot(V,R),0), n);

	return color;
}

void main()
{
    Ka = color;
	Kd = color;
	//Ka = vec3(1,0,0);
	//Kd = vec3(1,0,0);
	Ks = vec3(1);
	n = 100;
	
	outColor = vec4(shade(), 1.0);   
}
