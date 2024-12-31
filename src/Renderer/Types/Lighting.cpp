#include "Lighting.h"



Light light2 = Light(0.0f, 1.0f, 4.0f);

Light* Light::light = &light2;

Light::Light( float posX, float posY, float posZ) {

    this->posX = posX;
    this->posY = posY;
    this->posZ = posZ;

}

void Light::SetPosition(glm::vec3& position) {
    this->posX = position.x;
    this->posY = position.y;
    this->posZ = position.z;
}

void Light::SetAmbientLight(glm::vec3 lightColor) {
	this->AmbientMaterial = lightColor;
}

glm::vec3 Light::GetAmbientLight() {
    return AmbientMaterial;
}

void Light::SetDifuseLight(glm::vec3 lightColor) {
	this->DifuseMaterial = lightColor;
}

glm::vec3 Light::GetDifuseLight() {
	return DifuseMaterial;
}

void Light::SetSpecularLight(glm::vec3 lightColor) {
	this->SpecularMaterial = lightColor;
}

glm::vec3 Light::GetSpecularLight() {
	return SpecularMaterial;
}

const float Light::GetWorldSpaceWidth() {
    return width;
}

const float Light::GetWorldSpaceHeight() {
    return height;
}

const float Light::GetWorldSpaceDepth() {
    return depth;
}

const int Light::GetProbeSpaceDepth() {
    return depth / 0.5f;
}

const glm::vec3 Light::GetPosition() {
    return glm::vec3(posX, posY, posZ);
}

const glm::vec4 Light::GetLightColor() {
	return lightColor;
}

const float Light::GetStrength() {
    return strength;
}

const float Light::GetLightConstant() {
    return Lightconstant;
}

const float Light::GetLightLinear() {
    return Lightlinear; 
}

const float Light::GetLightQuadratic() {
    return Lightquadratic;
}