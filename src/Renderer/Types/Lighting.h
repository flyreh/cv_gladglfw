#pragma once
#include <glm/glm.hpp>

class Light {

public:
    Light() = default;
    Light(/*float witdh, float height, float depth,*/ float posX, float posY, float posZ);
    //void CreateTexure3D();
    const float GetWorldSpaceWidth();
    const float GetWorldSpaceHeight();
    const float GetWorldSpaceDepth();
   // const int GetProbeSpaceWidth();
   // const int GetProbeSpaceHeight();
    const int GetProbeSpaceDepth();
   // const int GetProbeCount();
    const glm::vec3 GetPosition();
	const glm::vec4 GetLightColor();
    //Texture3D texutre3D;
    static Light* light;

    void SetPosition(glm::vec3 & position);
	void SetAmbientLight(glm::vec3 lightColor);
	void SetDifuseLight(glm::vec3 lightColor);
	void SetSpecularLight(glm::vec3 lightColor);

	glm::vec3 GetAmbientLight();
	glm::vec3 GetDifuseLight();
	glm::vec3 GetSpecularLight();

    const float GetStrength();
    const float GetLightConstant();
    const float GetLightLinear();
    const float GetLightQuadratic();
private:
    // glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
    //  glm::vec4(1.0f, 0.831f, 0.50f, 1.0f);
	glm::vec4 lightColor = glm::vec4(1.0f, 0.85f, 0.60f, 1.0f);
    glm::vec3 AmbientMaterial = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 DifuseMaterial = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 SpecularMaterial = glm::vec3(1.0f, 1.0f, 1.0f);
    float strength = 10.0f;
    float Lightconstant = 1.0f;
    float Lightlinear = 0.22;
    float Lightquadratic = 0.20;
    float width = 0;
    float height = 0;
    float depth = 0;
    float posX = 0;
    float posY = 0;
    float posZ = 0;
};

