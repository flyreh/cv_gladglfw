#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./Back/Back.h"
#include "./API/Gl_renderer.h"
#include "./API/GL_BackVertex.h"

#include <tiny_obj_loader.h>

int main() {

  // inicio de las configuraciones de la ventana, de glfw y glad. Además carga y compila todos los shaders que definamos (por ahora solo hay del cubo).
  Back::Init();

  //// Enables the Depth Buffer
  glEnable(GL_DEPTH_TEST);

  // glEnable(GL_CULL_FACE);

  // glCullFace(GL_BACK);

  while (Back::WindowIsOpen()) {

    Back::BeginFrame();

    // Actualización del Input del mouse o teclado y las cámaras
    Back::UpdateSubSystems();

    // glEnable(GL_CULL_FACE);

    OpenGLRenderer::RenderFrame();

    std::cout << "Frame: " << Back::GetCurrentFrame() << std::endl;
    // glDisable(GL_CULL_FACE);

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
