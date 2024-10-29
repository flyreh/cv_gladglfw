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


int main() {

	//inicio de las configuraciones de la ventana, de glfw y glad. Además carga y compila todos los shaders que definamos (por ahora solo hay del cubo).
    Back::Init();


     //// Enables the Depth Buffer
     glEnable(GL_DEPTH_TEST);

     // Creación de VAO y VBO para cada objeto a Renderizar
     OpenGLRenderer::ObjectsPass();


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
			std::string newTitle = "CV-Gx / " + FPS + "FPS / " + ms + "ms";
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

		//Actualización de la "cámara"
        Camera::Update(deltaTime);

        // Activación del shader del cubo
        OpenGLRenderer::ActivateCubeShader();

        ////// Matrices de transformación
        ////// Son alternativas de la librería glm a las funciones de la matriz de transformación de glut.h (como gluLlookAt o glperspective)
        /////*
        ////    * Al renderizar múltiples objetos en la misma escena, se comparte la misma matriz view y projection) para todos,
        ////    pero cada objeto tendrá su propia matriz model en función de su posición y orientación.
        //*/
        glm::mat4 model = glm::mat4(1.0f);  // Matriz modelo

        glm::mat4 view = Camera::GetViewMatrix();  // Matriz de vista

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), Back::GetWindowedWidth() / Back::GetCurrentWindowHeight(), 0.1f,
            100.0f);

        //SetMat4 pasa las matrices al shader /solo shaders del cubo/ y este pueda realizar las transformaciones de los vértices
        OpenGLRenderer::g_shaders.Cube.SetMat4("model", model);
        OpenGLRenderer::g_shaders.Cube.SetMat4("view", view);
        OpenGLRenderer::g_shaders.Cube.SetMat4("projection", projection);

         //// Activación del VAO del cubo
        OpenGLRenderer::ActivateCubeVAO();
        glActiveTexture(GL_TEXTURE0);
        // Activación de la textura del cubo
        OpenGLRenderer::ActivateCubeTexture("Purple2");

        // Primer Cubo con rotación
        glm::mat4 modelFirstCube = glm::rotate(glm::mat4(1.0f), (float)currentFrame/2,
                                               glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación del cubo

        OpenGLRenderer::g_shaders.Cube.SetMat4("model", modelFirstCube);

        glDrawArrays(GL_TRIANGLES, 0, 50);  // Dibuja

        //// Esto Dibuja un segundo cubo, con escala, rotación y traslación
        glm::mat4 modelOtherCube = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 4.0f, -5.0f));
        modelOtherCube = glm::scale(modelOtherCube, glm::vec3(2.0f, 2.0f, 2.0f));
        modelOtherCube = glm::rotate(modelOtherCube, (float)currentFrame/2, glm::vec3(1.0f, 0.0f, 1.0f));

        OpenGLRenderer::g_shaders.Cube.SetMat4("model", modelOtherCube);

        glDrawArrays(GL_TRIANGLES, 0, 50);  // Dibuja

        // Activación del cubemap

        glDepthFunc(GL_LEQUAL);  // Cambiar test de profundidad para el cubemap

        OpenGLRenderer::ActivateCubeMapShader();

        view = glm::mat4(glm::mat3(Camera::GetViewMatrix())); // Sin traslación para el cubemap
        projection = glm::perspective(glm::radians(45.0f), Back::GetWindowedWidth() / Back::GetCurrentWindowHeight(), 0.1f, 100.0f);
        OpenGLRenderer::g_shaders.cubeMap.SetMat4("view", view);
        OpenGLRenderer::g_shaders.cubeMap.SetMat4("projection", projection);

        // Activamos el VAO del cubemap
        OpenGLRenderer::ActivateCubeMapVAO();

        glActiveTexture(GL_TEXTURE0);

        // Activamos la Textura del cubemap por su nombre
        OpenGLRenderer::ActivateTextureCubeMap("Sky");

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glDepthFunc(GL_LESS);  // Restaura el test de profundidad

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