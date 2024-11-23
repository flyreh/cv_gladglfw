#version 330 core
layout(location = 0) in vec3 aPos;        // Posici�n de los v�rtices
layout(location = 1) in vec2 aTexCoord;   // Coordenadas de textura
layout(location = 2) in vec3 aNormal;     // Normal de los v�rtices

out vec3 Normal;
out vec3 Position;
out vec2 TexCoord;                        // Salida de coordenadas de textura

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    Normal = mat3(transpose(inverse(model))) * aNormal;

    Position = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);

    TexCoord = aTexCoord;                 // Pasar las coordenadas de textura al fragment shader
}
