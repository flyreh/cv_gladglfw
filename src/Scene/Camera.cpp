#include "Camera.h"
#include "../Input/Input.h"
#include "../Back/Back.h"

Camera* Camera::activeCamera = nullptr;

Camera camera1;
Camera camera2;

void Camera::UpdateCameraInput(float deltaTime /*Camera& camera1, Camera& camera2*/ ) {

  if (Input::KeyDown(GLFW_KEY_1)) {

	  std::cout << "Cambio a camara 1"  << std::endl;
    activeCamera = &camera1;  // Cambiar a la primera cámara
  }
  if (Input::KeyDown(GLFW_KEY_2)) {


    std::cout << "Cambio a camara 2" << std::endl;
    activeCamera = &camera2;  // Cambiar a la segunda cámara
  }
 
  // Actualiza solo la cámara activa
  activeCamera->Update(deltaTime);
}

void Camera::Init() {

	camera1 = Camera();

	camera2 = Camera();

	activeCamera = &camera1;
	
	camera2.SetCameraPos(glm::vec3(3.0f, 0.0f, 5.0f));
}

void Camera::Update(float deltatime) {

	float cameraSpeed = 5.0f * deltatime;

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

glm::mat4 Camera::GetProjectionMatrix() {
	return glm::perspective(glm::radians(45.0f), Back::GetWindowedWidth() / Back::GetWindowedHeight(), 0.1f, 1000.0f);
}









