#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec3 inNormal;

uniform mat4 modelView;
uniform mat4 modelViewProj;
uniform mat4 normal;

out vec3 color;


//luz ambiental
vec3 Ia = vec3(0.15);

//fuente de luz
vec3 Il = vec3(1);
//uniform vec3 pl;
vec3 pl = vec3(0);


//objeto
vec3 Ka,Kd,Ks,po,no;
float n;

vec3 shade(){
color = vec3(0);

// ambiental
color += Ia * Ka;

// difuso
vec3 N = no;
vec3 L = normalize(pl - po);
color += Il * Kd * max(dot(N ,L),0);

//specular
vec3 V = normalize(-po);
vec3 R = reflect(-L,N);

color += Il * Ks * pow(max((dot(V,R)),0),n);


return color;

}


void main()
{
    po = (modelView * vec4(inPos , 1)).xyz;
	
   Ka = inColor;
   Kd = inColor;
   Ks = vec3(1);

   n = 10;
	
	//Ka = vec3(0);
	//Kd = vec3(0);
	no = normalize((normal * vec4(inNormal, 0.0f)).xyz);
	color = shade();
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
