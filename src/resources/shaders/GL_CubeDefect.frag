#version 460 core

out vec4 fragColor;

//in vec2 TexCoord;
//in vec3 FragPos;
//in vec3 Normal;

layout (binding = 1) uniform sampler2D albedoTexture;  // Base color texture
layout (binding = 2) uniform sampler2D normalTexture;  // Normal map
layout (binding = 3) uniform sampler2D rmaTexture;     // Roughness, Metalness, AO texture

uniform vec3 viewPos;    // Camera/view position

struct Light {
    vec3 Color;
    vec3 position;
    float strength;
    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 Color;
    vec3 direction;
    float strength;
};

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;


uniform DirLight dirLight;
uniform Light light;

// Functions for PBR calculation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (3.14159265359 * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float denom = NdotV * (1.0 - k) + k;
    return NdotV / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 CalculatePointLight(vec3 albedo, vec3 norm, float roughness, float metalness, float ao,
 vec3 TangentFragPos, vec3 TangentLightPos, vec3 TangentViewPos){
    
    // Light calculations
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 halfwayDir = normalize(viewDir + lightDir);
    vec3 radiance = light.Color  * light.strength;

    // Cook-Torrance BRDF components
    vec3 F0 = vec3(0.04); // Default reflectance for dielectrics
    F0 = mix(F0, albedo, metalness); // Adjust reflectance for metals

    // Fresnel-Schlick approximation
    vec3 F = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);

    // Normal Distribution Function (NDF)
    float D = DistributionGGX(norm, halfwayDir, roughness);

    // Geometry Function
    float G = GeometrySmith(norm, viewDir, lightDir, roughness);

    // Cook-Torrance specular BRDF
    vec3 numerator = D * G * F;
    float denominator = 4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, lightDir), 0.0) + 0.001;
    vec3 specular = numerator / denominator;

    // Diffuse component (Lambertian)
    vec3 kD = vec3(1.0) - F; // Energy conservation: kD + kS = 1
    kD *= 1.0 - metalness;   // Metals have no diffuse component

    float NdotL = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = kD * albedo / 3.14159265359 * NdotL;

    // Combine components
    vec3 ambient = ao * albedo * 0.3; // Ambient light with AO

    //atenuación
    float dist = length(TangentLightPos - TangentFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
    		    light.quadratic * (dist * dist)); 
                
    vec3 color = (ambient + (diffuse + specular) * radiance * NdotL ) * attenuation;

    return color;

}

vec3 CalculateDirLight(vec3 albedo, vec3 norm, float roughness, float metalness, float ao,
                    vec3 TangentFragPos, vec3 TangentLightPos, vec3 TangentViewPos, vec3 TangentlightDirection){

  // Light calculations
    vec3 lightDir = normalize(-TangentlightDirection);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 halfwayDir = normalize(viewDir + lightDir);
    vec3 radiance = dirLight.Color * dirLight.strength;

    // Cook-Torrance BRDF components
    vec3 F0 = vec3(0.04); // Default reflectance for dielectrics
    F0 = mix(F0, albedo, metalness); // Adjust reflectance for metals

    // Fresnel-Schlick approximation
    vec3 F = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);

    // Normal Distribution Function (NDF)
    float D = DistributionGGX(norm, halfwayDir, roughness);

    // Geometry Function
    float G = GeometrySmith(norm, viewDir, lightDir, roughness);

    // Cook-Torrance specular BRDF
    vec3 numerator = D * G * F;
    float denominator = 4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, lightDir), 0.0) + 0.001;
    vec3 specular = numerator / denominator;

    // Diffuse component (Lambertian)
    vec3 kD = vec3(1.0) - F; // Energy conservation: kD + kS = 1
    kD *= 1.0 - metalness;   // Metals have no diffuse component

    float NdotL = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = kD * albedo / 3.14159265359 * NdotL;

    // Combine components
    vec3 ambient = ao * albedo * 0.3; // Ambient light with AO
                
    vec3 color = (ambient + (diffuse + specular) * radiance * NdotL );

    return color;

    
}

void main() {
    // Retrieve albedo and normal
    vec3 albedo = texture(albedoTexture, fs_in.TexCoords).rgb;
    vec3 normal = texture(normalTexture, fs_in.TexCoords).rgb * 2.0 - 1.0; // Convert normal map from [0, 1] to [-1, 1]
    vec3 norm = normalize(normal);  

    //vec3 norm = normalize(Normal);


    // Retrieve RMA texture values
    vec3 rma = texture(rmaTexture, fs_in.TexCoords).rgb;
    float roughness = rma.r;  // Roughness (red channel)
    float metalness = rma.g;  // Metalness (green channel)
    float ao = rma.b;         // Ambient Occlusion (blue channel)

   
   vec3 TangentLightPos = fs_in.TBN * light.position;
    vec3 TangentViewPos  = fs_in.TBN * viewPos;

    vec3 TangentFragPos  = fs_in.TBN * fs_in.FragPos;

    vec3 TangentlightDirection = fs_in.TBN * dirLight.direction;
    // Calculate Light Directional

    vec3 ResultPointLight = CalculatePointLight(albedo, norm, roughness, metalness, ao,
                                TangentFragPos, TangentLightPos, TangentViewPos);

    vec3 ResultDirLight = CalculateDirLight(albedo, norm, roughness, metalness, ao,
                                TangentFragPos, TangentLightPos, TangentViewPos, TangentlightDirection);

    vec3 result = ResultPointLight + ResultDirLight;

    // Final color output
    fragColor = vec4(result, 1.0);
}
