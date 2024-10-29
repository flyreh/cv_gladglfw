#include "../Enviroment/CV_Enviroment.h"
#include "Gl_renderer.h"
#include "../Back/Back.h"
#include "Types/GL_shader.h"
#include "GL_BackVertex.h"
#include "../Core/AssetManager.h"

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
  g_shaders.cubeMap.Load("GL_CubeMap.vert", "GL_CubeMap.frag");

  /*Demás Shaders*/

  std::cout << "ready shaders... \n";
}

void OpenGLRenderer::InitMinimum() {
  HotloadShaders();

  // carga de buffers VAO y VBO para objetos 3D
}

void OpenGLRenderer::ObjectsPass() {
  // Carga vertices del .obj
  // std::vector<glm::vec2> CubeVertices = Util.GenerateVerticesCube();

  // Creación de VAO y VBO para el cubo
  AssetManager::UploadVertexData();

  //Creación de VAO y VBO para otro objeto

  AssetManager::UploadVertexDataCubeMap();

}

void OpenGLRenderer::ActivateCubeShader() {
  g_shaders.Cube.Use();
  g_shaders.Cube.SetInt("texture1", 0);
}

void OpenGLRenderer::ActivateCubeVAO() {
  glBindVertexArray(GLBackVertex::GetCubeVAO());
}

void OpenGLRenderer::ActivateCubeTexture(std::string textureName) {


  TextureObject* cubeTexture = AssetManager::GetTextureByName(textureName);
  
  if (cubeTexture) {
    glBindTexture(GL_TEXTURE_2D, cubeTexture->GetGLTexture().GetID());
  }
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




void OpenGLRenderer::ActivateCubeMapShader() {
  g_shaders.cubeMap.Use();
  g_shaders.cubeMap.SetInt("skybox", 0);
}

void OpenGLRenderer::ActivateCubeMapVAO() {
  glBindVertexArray(GLBackVertex::GetCubeMapVAO());
}

void OpenGLRenderer::ActivateTextureCubeMap(std::string textureName) {
  TextureCubeMap* cubeMapTexture = AssetManager::GetCubemapTextureByName(textureName);
  
  if (cubeMapTexture) {
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture->GetGLtextureCubeMap().GetID());

  }
}

void OpenGLRenderer::DeleteCubeMapVAO() {
  unsigned int cubeMapVAO = GLBackVertex::GetCubeMapVAO();
  glDeleteVertexArrays(1, &cubeMapVAO);

  std::cout << "Cube VAO delete" << std::endl;
}

void OpenGLRenderer::DeleteCubeMapVBO() {
  unsigned int cubeMapVBO = GLBackVertex::GetCubeMapVBO();
  glDeleteBuffers(1, &cubeMapVBO);
  std::cout << "Cube VBO delete" << std::endl;
}



