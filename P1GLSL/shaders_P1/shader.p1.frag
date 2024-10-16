#version 330 core

uniform sampler2D colorTex; 
out vec4 outColor;

in vec3 vPos;  
in vec3 vNorm; 

void main()
{
   
    vec2 texCoord = vPos.xy;
    vec4 texColor = texture(colorTex, texCoord);

    // Descartar fragmentos cuyo color en la textura sea negro (r, g, b son todos 0)
    if (texColor.r == 0.0 && texColor.g == 0.0 && texColor.b == 0.0) {
        discard;
    }

    // Si el índice de la primitiva es impar, colorear con la normal, si es par, usar la textura
    if (gl_PrimitiveID % 2 != 0) {
        outColor = vec4(vNorm, 1.0);  // Color basado en la normal si es impar
    } else {
        outColor = texColor;  // Color de la textura si es par
    }
}
