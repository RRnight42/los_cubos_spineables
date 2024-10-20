#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 inNormal;
in vec3 inPos;

// Luz direccional
vec3 Ia = vec3(0.3); // Luz ambiental
vec3 Id = vec3(1.0, 0.9, 0.7); // Intensidad de la luz difusa (luz blanca)
vec3 Is = vec3(0.2, 0.15, 0.1); // Intensidad de la luz especular (luz blanca)

// Dirección de la luz direccional (normalizamos el vector)
vec3 lightDir = normalize(vec3(-1.0, -5.0, 2.0)); // Luz proveniente de una dirección fija

// Coeficientes de material
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
float n;

in vec3 po;  // Posición del fragmento
in vec3 no;  // Normal del fragmento
in vec2 tc;  // Coordenadas de textura

uniform sampler2D colorTex;
uniform sampler2D specularTex;
uniform sampler2D emiTex;

vec3 shade() 
{
    vec3 color = vec3(0);

    // Componente ambiental
    color += Ia * Ka;

    // Difusa
    vec3 N = normalize(no);
    vec3 L = -lightDir;  // Luz direccional es constante, ya está normalizada en el paso anterior
    float diff = max(dot(N, L), 0.0);  // Máximo entre el producto escalar y 0 (para evitar valores negativos)
    color += Id * Kd * diff;  // Componente difusa

    // Especular
    vec3 V = normalize(-po); // Vector hacia la cámara (usualmente se toma el opuesto de la posición del fragmento)
    vec3 R = reflect(-L, N);  // Vector de reflexión de la luz direccional
    float spec = pow(max(dot(V, R), 0.0), n);  // Componente especular (con el exponente n para la dureza del brillo)
    color += Is * Ks * spec;

    // Componente emisiva
    color += Ke;

    return color;
}

void main()
{
    // Asignar materiales usando texturas
    Ka = texture(colorTex, tc).rgb;  // Componente ambiental del material
    Kd = Ka;  // Componente difusa
    Ke = texture(emiTex, tc).rgb;  // Componente emisiva
    Ks = texture(specularTex, tc).rrr;  // Componente especular
    n = 100.0;  // Exponente especular (dureza del brillo)

    // Calcular el color final del fragmento
    outColor = vec4(shade(), 1.0);
}
