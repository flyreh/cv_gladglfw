#version 330 core
out vec4 fragColor;


in vec2 TexCoord;                         // Entrada de coordenadas de textura
in vec3 Normal;
in vec3 Position;

uniform sampler2D texture1;               // Textura a usar
uniform vec3 cameraPos;
uniform samplerCube skybox;
              

void main() {

    vec3 I = normalize(Position - cameraPos);
    vec3 X = reflect(I, normalize(Normal));
    vec3 R = refract(I, normalize(Normal), 1.0/1.33);

    

// Inicializar color acumulado
    vec4 blurredReflection = vec4(0.0);

// Número de muestras para el desenfoque
    int numSamples = 20;

// Desplazamientos aleatorios en la reflexión
    float roughness = 0.008; // Controla la intensidad del empañado

for (int i = 0; i < numSamples; ++i) {
    vec3 randomOffset = roughness * vec3(
        fract(sin(dot(vec3(i, i * 2.1, i * 3.7), vec3(12.9898, 78.233, 45.164))) * 43758.5453),
        fract(sin(dot(vec3(i * 1.2, i * 2.4, i * 3.8), vec3(32.123, 24.234, 64.128))) * 13758.5453),
        fract(sin(dot(vec3(i * 1.5, i * 3.1, i * 2.7), vec3(72.123, 92.234, 34.128))) * 23758.5453)
    );

    vec3 sampleDir = normalize(X + randomOffset); // Perturbar dirección de reflexión
    blurredReflection += texture(skybox, sampleDir);
}

// Promediar los reflejos muestreados
blurredReflection /= float(numSamples);

vec4 textureColor = texture(texture1, TexCoord);



fragColor = mix(textureColor, blurredReflection , 0.5);
}


