#pragma once

#include "../Enviroment/CV_Enviroment.h"
#include "../Math/Types.h"


class Camera {

private :

	 glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
 glm::vec3 cameraFront = (glm::vec3(0.0f, 0.0f, -1.0f));
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch  = 0.0f;

	float lastX = 400;
	float lastY = 300;

	bool firstMouse = true;

	float sensitivity = 0.1f;


public:
    static Camera* activeCamera; 

	Camera()
        : cameraPos(glm::vec3(0.0f, 0.0f, 5.0f)),
          cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
          cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
          yaw(-90.0f),
          pitch(0.0f),
          lastX(400),
          lastY(300),
          firstMouse(true),
          sensitivity(0.1f)
	{}


	 void Init();

	void Update(float deltatime);

	void SetCameraPos(glm::vec3 pos);

	void SetCameraFront(glm::vec3 front);

	void SetCameraUp(glm::vec3 up);

	void UpdateCameraInput(float deltaTime, Camera& camera1, Camera& camera2);


	glm::vec3 GetCameraPos();

	glm::vec3 GetCameraFront();

	glm::vec3 GetCameraUp();

	glm::mat4 GetViewMatrix();





};


