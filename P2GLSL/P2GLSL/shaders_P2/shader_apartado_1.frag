#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 inNormal;
in vec3 inPos;

// Fuente de luz
vec3 Ia = vec3(0.1); // Luz ambiental

// Dos luces con sus intensidades y posiciones . Usamos arrays para declaralo
vec3 Il[2] = vec3[2](vec3(1), vec3(0.4)); // Intensidad de las dos luces ,  variamos un poco para percibir la diferencia
vec3 pl[2] = vec3[2](vec3(-5, 0, 0), vec3(5, 0, 0)); // Posiciones de las dos luces , izquierda derecha



// Coeficientes
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
float n;

in vec3 po;
in vec3 no;
in vec2 tc;

uniform sampler2D colorTex;
uniform sampler2D specularTex;
uniform sampler2D emiTex;

vec3 shade() 
{
    vec3 color = vec3(0);

    // Componente ambiental
    color += Ia * Ka;

    // Cálculo para cada luz


    for (int i = 0; i < 2; i++) {
        // Difusa
        vec3 N = normalize(no);
        vec3 L = normalize(pl[i] - po);
        color += Il[i] * Kd * (max(dot(N, L), 0.0));

        // Especular
        vec3 V = normalize(-po);
        vec3 R = reflect(-L, N);
        color += Il[i] * Ks * pow(max(dot(V, R), 0.0), n);

        color += Ke;
    }

    return color;
}

void main()
{
    // Asignar materiales
    Ka =  texture(colorTex, tc).rgb;
    Kd = Ka;
    Ke = texture(emiTex, tc).rgb;
    Ks = texture(specularTex , tc).rrr;
    n = 100; // Exponente especular

    // Calcular el color final
    outColor = vec4(shade(), 1.0);
}