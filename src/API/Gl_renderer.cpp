#include "../Enviroment/CV_Enviroment.h"
#include "Gl_renderer.h"
#include "../Back/Back.h"
#include "Types/GL_shader.h"
#include "GL_BackVertex.h"

struct LightVolumeData {
  glm::vec4 aabbMin;
  glm::vec4 aabbMax;
};

struct TileData {
  int lightCount;
  int lightIndices[127];
};

namespace OpenGLRenderer {

Shaders g_shaders;

}  // namespace OpenGLRenderer

void OpenGLRenderer::HotloadShaders() {

  std::cout << "Hotloading shaders...\n";

  g_shaders.Cube.Load("GL_Cube.vert", "GL_Cube.frag");

  /*Demás Shaders*/

  std::cout << "ready shaders... \n";
}

void OpenGLRenderer::InitMinimum() {
  HotloadShaders();

  // carga de buffers VAO y VBO para objetos 3D
}

void OpenGLRenderer::CubePass(std::vector<float> Cubevertices /*por ahora se pasan los vertices del main*/) {
  // Carga vertices del .obj
  // std::vector<glm::vec2> CubeVertices = Util.GenerateVerticesCube();

  // Creación de VAO y VBO para el cubo
  GLBackVertex::UploadCubeVertexData(Cubevertices);

  //activa shader de cubo
  g_shaders.Cube.Use();

  //activa VAO del Cubo
  glBindVertexArray(GLBackVertex::GetCubeVAO());
}

void OpenGLRenderer::DeleteCubeVAO() {
  unsigned int cubeVAO = GLBackVertex::GetCubeVAO();
  glDeleteVertexArrays(1, &cubeVAO);

  std::cout << "Cube VAO delete" << std::endl;
}

void OpenGLRenderer::DeleteCubeVBO() {
  unsigned int cubeVBO = GLBackVertex::GetCubeVBO();
  glDeleteBuffers(1, &cubeVBO);
  std::cout << "Cube VBO delete"<<std::endl;
}