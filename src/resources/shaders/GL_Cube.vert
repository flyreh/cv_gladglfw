#version 330 core
layout(location = 0) in vec3 aPos;        // Posici�n
layout(location = 1) in vec2 aTexCoord;   // Coordenadas de textura
out vec2 TexCoord;                        // Salida de coordenadas de textura
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;                 // Pasar las coordenadas de textura al fragment shader
}