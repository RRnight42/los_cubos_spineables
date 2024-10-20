#version 330 core

uniform sampler2D colorTex; 
out vec4 outColor;

in vec3 vPos;  
in vec3 vNorm; 
in vec2 vTexCoord;

void main()
{
   
    
    vec4 texColor = texture(colorTex, vTexCoord);

    // Descartar fragmentos cuyo color en la textura sea negro (r, g, b son todos 0)
    if (texColor.r == 0.0 && texColor.g == 0.0 && texColor.b == 0.0) {
        discard;
    }

    
        outColor = texColor;  
    
}
