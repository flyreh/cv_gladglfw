#include <iostream>﻿
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./Back/Back.h"
#include "./API/Gl_renderer.h"
#include "./API/GL_BackVertex.h"

// Frame
bool firstFrame = true;
double currentFrame = 0.0f;
double deltaTime = 0.0f; 
double lastFrame = 0.0f; 

// FPS
double prevTime = 0.0;
double crntTime = 0.0;
double timeDiff;

// frames counter
unsigned int counter = 0;

void DrawCubeMap();




Camera camera1 = Camera();

Camera camera2 = Camera();


struct Plane {
  glm::vec3 normal;
  float distance;
};

std::array<Plane, 6> extractFrustumPlanes(const glm::mat4& vpMatrix) {

  std::array<Plane, 6> planes;

  // Izquierdo
  planes[0].normal = glm::vec3(vpMatrix[0][3] + vpMatrix[0][0], vpMatrix[1][3] + vpMatrix[1][0],
                               vpMatrix[2][3] + vpMatrix[2][0]);
  planes[0].distance = vpMatrix[3][3] + vpMatrix[3][0];


  // Derecho
  planes[1].normal = glm::vec3(vpMatrix[0][3] - vpMatrix[0][0], vpMatrix[1][3] - vpMatrix[1][0],
                               vpMatrix[2][3] - vpMatrix[2][0]);
  planes[1].distance = vpMatrix[3][3] - vpMatrix[3][0];

  // Inferior
  planes[2].normal = glm::vec3(vpMatrix[0][3] + vpMatrix[0][1], vpMatrix[1][3] + vpMatrix[1][1],
                               vpMatrix[2][3] + vpMatrix[2][1]);
  planes[2].distance = vpMatrix[3][3] + vpMatrix[3][1];

  // Superior
  planes[3].normal = glm::vec3(vpMatrix[0][3] - vpMatrix[0][1], vpMatrix[1][3] - vpMatrix[1][1],
                               vpMatrix[2][3] - vpMatrix[2][1]);
  planes[3].distance = vpMatrix[3][3] - vpMatrix[3][1];

  // Cercano
  planes[4].normal = glm::vec3(vpMatrix[0][3] + vpMatrix[0][2], vpMatrix[1][3] + vpMatrix[1][2],
                               vpMatrix[2][3] + vpMatrix[2][2]);
  planes[4].distance = vpMatrix[3][3] + vpMatrix[3][2];

  // Lejano
  planes[5].normal = glm::vec3(vpMatrix[0][3] - vpMatrix[0][2], vpMatrix[1][3] - vpMatrix[1][2],
                               vpMatrix[2][3] - vpMatrix[2][2]);
  planes[5].distance = vpMatrix[3][3] - vpMatrix[3][2];

  // Normalizar los planos
  for (auto& plane : planes) {
    float length = glm::length(plane.normal);
    plane.normal /= length;
    plane.distance /= length;
  }

  return planes;
}

bool isCubeInFrustum(const std::array<Plane, 6>& planes, const glm::vec3& cubeCenter,
                     float halfSize) {
  for (const auto& plane : planes) {
    glm::vec3 positiveVertex = cubeCenter;

    positiveVertex.x += (plane.normal.x >= 0) ? halfSize : -halfSize;
    positiveVertex.y += (plane.normal.y >= 0) ? halfSize : -halfSize;
    positiveVertex.z += (plane.normal.z >= 0) ? halfSize : -halfSize;

    if (glm::dot(plane.normal, positiveVertex) + plane.distance < 0) {
      return false; 
    }
  }
  return true; 
}

std::array<Plane, 6> frustumPlanes;

std::vector<glm::vec3> visibleCubes;
float maxPos = 10000.0f;
float step = 3.0f;
float cubeHalfSize = 0.5f;  

// Actualiza la lista de cubos visibles según el frustum
void updateVisibleCubes(const std::array<Plane, 6>& frustumPlanes, float cubeHalfSize) {

  visibleCubes.clear();

  for (float i = 0; i < maxPos; i += step) {
    glm::vec3 cubeCenter(i, 2.0f, 0.0f);
    if (isCubeInFrustum(frustumPlanes, cubeCenter, cubeHalfSize)) {
      visibleCubes.push_back(cubeCenter);  // Almacena solo los cubos visibles
    }
  }
}

// Renderiza solo los cubos que están en el frustum
void renderVisibleCubes() {
  int cubeIndex = 0;
  for (const auto& cubeCenter : visibleCubes) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubeCenter);
    model = glm::rotate(model,(float)currentFrame*2, glm::vec3(1.0f, 1.0f, 1.0f));
   
    OpenGLRenderer::g_shaders.Cube.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 50);
  }
}

int main() {


  Camera::activeCamera = &camera1;

	//inicio de las configuraciones de la ventana, de glfw y glad. Además carga y compila todos los shaders que definamos (por ahora solo hay del cubo).
    Back::Init();

     //// Enables the Depth Buffer
     glEnable(GL_DEPTH_TEST);

     //glEnable(GL_CULL_FACE);

      //glCullFace(GL_BACK);

     // Creación de VAO y VBO para cada objeto a Renderizar
     OpenGLRenderer::ObjectsPass();
          
     camera2.SetCameraPos(glm::vec3(3.0f, 0.0f, 5.0f));

    while (Back::WindowIsOpen()){
        
        Back::BeginFrame();

        // Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string newTitle = "Frustum Culling/ " + FPS + "FPS / " + ms + "ms" + " Objetos Renderizandose :" + std::to_string(visibleCubes.size());
            glfwSetWindowTitle(Back::GetWindowPointer(), newTitle.c_str());
			prevTime = crntTime;
			counter = 0;
		}

		//Actualización del Input del mouse o teclado en vez de un callback
        Back::UpdateSubSystems();

        if(firstFrame){
            currentFrame = glfwGetTime();
            firstFrame = false;       
        }
        lastFrame = currentFrame;
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;

        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       // glEnable(GL_CULL_FACE);

        Camera::activeCamera->UpdateCameraInput(deltaTime, camera1, camera2);

         glm::mat4 view = Camera::activeCamera->GetViewMatrix();  // Matriz de vista

        ////    * Al renderizar múltiples objetos en la misma escena, se comparte la misma matriz view y projection) para todos,
        ////    pero cada objeto tendrá su propia matriz model en función de su posición y orientación.
        //*/

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), Back::GetWindowedWidth() / Back::GetWindowedHeight(), 1.0f,
            10000.0f);

        glm::mat4 viewProjectionMatrix = projection * view;

        frustumPlanes = extractFrustumPlanes(viewProjectionMatrix);
                
        // Activación del shader del cubo
        OpenGLRenderer::ActivateCubeShader();
        //SetMat4 pasa las matrices al shader /solo shaders del cubo/ y este pueda realizar las transformaciones de los vértices
        OpenGLRenderer::g_shaders.Cube.SetMat4("view", view);
        OpenGLRenderer::g_shaders.Cube.SetMat4("projection", projection);

         // Activación del VAO del cubo
        OpenGLRenderer::ActivateCubeVAO();
        glActiveTexture(GL_TEXTURE0);
        // Activación de la textura del cubo por su nombre.

        OpenGLRenderer::ActivateCubeTexture("Cube");

        updateVisibleCubes(frustumPlanes, cubeHalfSize);  // Actualiza arreglo de cubos visibles actualmente

        
        glm::vec3 secondCubeCenter(3.0f, 5.0f, -3.0f);

        if (isCubeInFrustum(frustumPlanes, secondCubeCenter, cubeHalfSize)) {
            visibleCubes.push_back(secondCubeCenter); 
        }


        renderVisibleCubes();  // Renderiza solo los cubos visibles

       // glDisable(GL_CULL_FACE);

        DrawCubeMap();

	    Back::EndFrame();
    }

    Back::CleanUp();

    // Limpiar recursos por cada VAO y VBO creado (bucle)
    OpenGLRenderer::DeleteCubeVAO();
    OpenGLRenderer::DeleteCubeVBO();

    OpenGLRenderer::DeleteCubeMapVAO();
    OpenGLRenderer::DeleteCubeMapVBO();

    return 0;
}


void DrawCubeMap(){

  // Activación del cubemap

  glDepthFunc(GL_LEQUAL);  // Cambiar test de profundidad para el cubemap

  OpenGLRenderer::ActivateCubeMapShader();

  glm::mat4 view = glm::mat4(glm::mat3(Camera::activeCamera->GetViewMatrix()));  // Sin traslación para el cubemap
  glm::mat4 projection = glm::perspective(
      glm::radians(45.0f), Back::GetWindowedWidth() / Back::GetWindowedHeight(), 0.1f, 100.0f);
  OpenGLRenderer::g_shaders.cubeMap.SetMat4("view", view);
  OpenGLRenderer::g_shaders.cubeMap.SetMat4("projection", projection);

  // Activamos el VAO del cubemap
  OpenGLRenderer::ActivateCubeMapVAO();

  glActiveTexture(GL_TEXTURE0);

  // Activamos la Textura del cubemap por su nombre
  OpenGLRenderer::ActivateTextureCubeMap("Sky2");

  glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);

  glDepthFunc(GL_LESS);  // Restaura el test de profundidad
    
}