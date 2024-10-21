#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 inNormal;
in vec3 inPos;

// Luz focal
vec3 Ia = vec3(0.3); // Luz ambiental
vec3 Id = vec3(1.0, 0.9, 0.7); // Intensidad de la luz difusa (luz blanca)
vec3 Is = vec3(0.2, 0.15, 0.1); // Intensidad de la luz especular (luz blanca)

// Parámetros de la luz focal
uniform vec3 lightPos = vec3(0.0, 0.0, 10.0);   // Posición de la luz
uniform vec3 lightDir = vec3(0.0, 0.0, 10.0);   // Dirección de la luz
uniform float cutoff = cos(radians(0.1));    // Ángulo de corte del cono (en grados, convertido a coseno)
uniform float outerCutoff = cos(radians(0.5));  // Ángulo de corte externo para el suavizado (coseno)

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

    // Dirección desde el fragmento a la luz
    vec3 lightDirToFrag = normalize(lightPos - po);

    // Calcular si el fragmento está dentro del cono de luz focal
    float theta = dot(lightDirToFrag, normalize(lightDir));  // Ángulo entre la dirección de la luz y el fragmento
    
    // Suavizar el borde del foco usando outerCutoff
    float epsilon = cutoff - outerCutoff;
    float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

    if (intensity > 0.0)  // Solo iluminar si está dentro del ángulo suavizado
    {
        // Difusa
        vec3 N = normalize(no);
        vec3 L = lightDirToFrag;
        float diff = max(dot(N, L), 0.0);
        color += Id * Kd * diff * intensity;  // Aplicar intensidad suavizada

        // Especular
        vec3 V = normalize(-po);  // Vector hacia la cámara
        vec3 R = reflect(-L, N);  // Vector de reflexión de la luz
        float spec = pow(max(dot(V, R), 0.0), n);
        color += Is * Ks * spec * intensity;  // Aplicar intensidad suavizada
    }

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
