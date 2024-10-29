#pragma once

#include "../Enviroment/CV_Enviroment.h"
#include "../Math/Types.h"


class Camera {

private :

	static glm::vec3 cameraPos;
	static glm::vec3 cameraFront;
	static glm::vec3 cameraUp;

	static float yaw;
	static float pitch;

	static float lastX, lastY;

	static bool firstMouse;

public:

	static void Init();

	static void Update(float deltatime);

	static void SetCameraPos(glm::vec3 pos);

	static void SetCameraFront(glm::vec3 front);

	static void SetCameraUp(glm::vec3 up);


	static glm::vec3 GetCameraPos();

	static glm::vec3 GetCameraFront();

	static glm::vec3 GetCameraUp();

	static glm::mat4 GetViewMatrix();


};


