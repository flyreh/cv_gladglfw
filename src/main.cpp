#include <iostream>
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

int main() {

	//inicio de las configuraciones de la ventana, de glfw y glad. Además carga y compila todos los shaders que definamos (por ahora solo hay del cubo).
    Back::Init();

	// Creación de VAO y VBO para cada objeto a Renderizar
    OpenGLRenderer::ObjectsPass();

    // Activación del shader del cubo
    OpenGLRenderer::ActivateCubeShader();

    // Activación del VAO del cubo
    OpenGLRenderer::ActivateCubeVAO();

    // Activación de la textura del cubo
    OpenGLRenderer::ActivateCubeTexture("Cube");

    while (Back::WindowIsOpen()){

        Back::BeginFrame();

		//Actualización del Input del mouse o teclado en vez de un callback
        Back::UpdateSubSystems();

        if(firstFrame){
            currentFrame = glfwGetTime();
            firstFrame = false;       
        }
        lastFrame = currentFrame;
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;

		//Actualización de la "cámara"
        Camera::Update(deltaTime);

        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST); // Habilitar el test de profundidad

        // Matrices de transformación
        // Son alternativas de la librería glm a las funciones de la matriz de transformación de glut.h (como gluLlookAt o glperspective)
        /*
            * Al renderizar múltiples objetos en la misma escena, se comparte la misma matriz view y projection) para todos,
            pero cada objeto tendrá su propia matriz model en función de su posición y orientación.
        */
        glm::mat4 model = glm::mat4(1.0f); // Matriz modelo 

        glm::mat4 view = glm::lookAt(Camera::GetCameraPos(), Camera::GetCameraPos() + Camera::GetCameraFront(), Camera::GetCameraUp()); // Matriz vista

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), Back::GetWindowedWidth() / Back::GetCurrentWindowHeight(), 0.1f, 100.0f);

        //SetMat4 pasa las matrices al shader /solo shaders del cubo/ y este pueda realizar las transformaciones de los vértices
		OpenGLRenderer::g_shaders.Cube.SetMat4("model", model);
		OpenGLRenderer::g_shaders.Cube.SetMat4("view", view);
		OpenGLRenderer::g_shaders.Cube.SetMat4("projection", projection);

        // Primer Cubo con rotación
        glm::mat4 modelFirstCube = glm::rotate(glm::mat4(1.0f), (float)currentFrame, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación del cubo

        OpenGLRenderer::g_shaders.Cube.SetMat4("model", modelFirstCube);

        glDrawArrays(GL_TRIANGLES, 0, 50); // Dibuja

		// Esto Dibuja un segundo cubo, con escala, rotación y traslación
        glm::mat4 modelOtherCube = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, -10.0f));
		modelOtherCube = glm::scale(modelOtherCube, glm::vec3(2.0f, 2.0f, 2.0f)); 
		modelOtherCube = glm::rotate(modelOtherCube, (float)currentFrame, glm::vec3(1.0f, 0.0f, 0.0f));

        OpenGLRenderer::g_shaders.Cube.SetMat4("model", modelOtherCube);

        glDrawArrays(GL_TRIANGLES, 0, 50); // Dibuja

	    Back::EndFrame();
    }

    Back::CleanUp();

    // Limpiar recursos por cada VAO y VBO creado (bucle)
    OpenGLRenderer::DeleteCubeVAO();
    OpenGLRenderer::DeleteCubeVBO();

    return 0;
}