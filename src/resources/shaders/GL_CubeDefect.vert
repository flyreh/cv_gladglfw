#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;


out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;


void main(){

	//Normal = normalMatrix * VertexNormal;

	//FragPos = vec3(model * vec4(VertexPosition, 1.0));

	vs_out.FragPos = vec3(model * vec4(VertexPosition, 1.0));  
	vs_out.TexCoords = aTexCoord;

	vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * VertexNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = aBitangent;

	vs_out.TBN = transpose(mat3(T, B, N)); 
	
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0);

	//TexCoord = aTexCoord;   

}	
