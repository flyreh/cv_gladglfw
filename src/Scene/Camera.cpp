#include "Camera.h"
#include "../Input/Input.h"



glm::vec3 Camera::cameraPos = glm::vec3(0);
glm::vec3 Camera::cameraFront = glm::vec3(0);
glm::vec3 Camera::cameraUp = glm::vec3(0);

float Camera::yaw = 0.0f;
float Camera::pitch = 0.0f;

float Camera::lastX = 0;
float Camera::lastY = 0;

bool Camera::firstMouse = false;


void Camera::Init() {

	cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	yaw = -90.0f;
	pitch = 0.0f;

	lastX = 400;
	lastY = 300;

	firstMouse = true;

}
void  Camera::Update(float deltatime) {

	float cameraSpeed = 2.5f * deltatime;
	float sensitivity = 0.1f;

	if (Input::KeyDown(GLFW_KEY_W))
		cameraPos += cameraSpeed * cameraFront;
	if (Input::KeyDown(GLFW_KEY_S))
		cameraPos -= cameraSpeed * cameraFront;
	if (Input::KeyDown(GLFW_KEY_A))
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (Input::KeyDown(GLFW_KEY_D))
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (firstMouse){

		lastX = Input::GetMouseX();
		lastY = Input::GetMouseY();
		firstMouse = false;
	
	}

	float xoffset = Input::GetMouseOffsetX();
	float yoffset = Input::GetMouseOffsetY();

	lastX = Input::GetMouseX();
	lastY = Input::GetMouseY();

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);


}

glm::vec3 Camera::GetCameraPos() {
	return cameraPos;
}

glm::vec3 Camera::GetCameraFront() {
	return cameraFront;
}

glm::vec3 Camera::GetCameraUp() {
	return cameraUp;
}

void Camera::SetCameraPos(glm::vec3 pos) {
	cameraPos = pos;
}

void Camera::SetCameraFront(glm::vec3 front) {
	cameraFront = front;
}

void Camera::SetCameraUp(glm::vec3 up) {
	cameraUp = up;
}

glm::mat4 Camera::GetViewMatrix(){
  return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}






