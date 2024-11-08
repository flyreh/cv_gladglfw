#version 330 core
out vec4 fragColor;
in vec2 TexCoord;                         // Entrada de coordenadas de textura
uniform sampler2D texture1;               // Textura a usar
void main() {
    fragColor = texture(texture1, TexCoord);  // Aplicar la textura
}


