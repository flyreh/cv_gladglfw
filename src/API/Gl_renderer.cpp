#include "../Enviroment/CV_Enviroment.h"
#include "Gl_renderer.h"
#include "../Back/Back.h"
#include "Types/GL_shader.h"
#include "GL_BackVertex.h"
#include "../Core/AssetManager.h"
#include "../Math/Frustum.h"

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

void CubePass();
void CubeMapPass();

float cubeHalfSize = 0.5f;


void OpenGLRenderer::HotloadShaders() {

  std::cout << "Hotloading shaders...\n";

  g_shaders.DefectCube.Load("GL_CubeDefect.vert", "GL_CubeDefect.frag");
  g_shaders.CubeMirror.Load("GL_CubeMirror.vert", "GL_CubeMirror.frag");
  g_shaders.cubeMap.Load("GL_CubeMap.vert", "GL_CubeMap.frag");

  /*Demás Shaders*/

  std::cout << "ready shaders... \n";
}

void OpenGLRenderer::InitMinimum() {

  HotloadShaders();

  // carga de buffers VAO y VBO para objetos 3D

  ObjectsPass();
}

void OpenGLRenderer::ObjectsPass() {

  // Carga vertices del .obj
  // std::vector<glm::vec2> CubeVertices = Util.GenerateVerticesCube();

  // Creación de VAO y VBO para el cubo
  AssetManager::UploadVertexData();

  //Creación de VAO y VBO para CubeMap

  AssetManager::UploadVertexDataCubeMap();


}

void OpenGLRenderer::RenderFrame() {

    CubePass();

	CubeMapPass();

}

Frustum frustumA;

void renderCube(glm::vec3 CubeCenter, glm::mat4 modelObj) {

    glm::mat4 model = glm::mat4(1.0f);

    if (frustumA.IntersectsCube(CubeCenter, cubeHalfSize)) {

        model = glm::translate(model, CubeCenter);

        glm::mat4 modelObject = model * modelObj;

        OpenGLRenderer::g_shaders.CubeMirror.SetMat4("model", modelObject);

        glDrawArrays(GL_TRIANGLES, 0, 50);
    }
}

void CubePass() {

    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    //SetMat4 pasa las matrices al shader /solo shaders del cubo/ y este pueda realizar las transformaciones de los vértices
    
    glm::mat4 view = Camera::activeCamera->GetViewMatrix();
	glm::mat4 projection = Camera::activeCamera->GetProjectionMatrix();

    glm::mat4 viewProjectionMatrix = projection * view;

	frustumA.Update(viewProjectionMatrix);


    // Activación del shader del cubo
    OpenGLRenderer::ActivateCubeMirrorShader();

    OpenGLRenderer::g_shaders.CubeMirror.SetMat4("view", view);
    OpenGLRenderer::g_shaders.CubeMirror.SetMat4("projection", projection);
    OpenGLRenderer::g_shaders.CubeMirror.SetVec3("cameraPos", Camera::activeCamera->GetCameraPos());
    // Activación del VAO del cubo
    OpenGLRenderer::ActivateCubeVAO();

    // Activación de la textura del cubo por su nombre.
    glActiveTexture(GL_TEXTURE1);
    OpenGLRenderer::ActivateCubeTexture("Floor");
    OpenGLRenderer::g_shaders.CubeMirror.SetInt("texture1", 1);

    OpenGLRenderer::g_shaders.CubeMirror.SetInt("skybox", 0);

    //    * Al renderizar múltiples objetos en la misma escena, se comparte la misma matriz view y projection) para todos,
    //    pero cada objeto tendrá su propia matriz model en función de su posición y orientación.
    //
    glm::vec3 firstCubeCenter(0.0f, 0.0f, 0.0f);
    glm::mat4 modelFirstCube = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    renderCube(firstCubeCenter, modelFirstCube);

    glm::vec3 secondCubeCenter(3, 3, 3);
    glm::mat4 modelSecondCube = glm::rotate(glm::mat4(1.0f), (float)Back::GetCurrentFrame() / 4, glm::vec3(0.0f, 1.0f, 0.0f));
    renderCube(secondCubeCenter, modelSecondCube);

}

void CubeMapPass() {

    // Activación del cubemap

    glDepthFunc(GL_LEQUAL);  // Cambiar test de profundidad para el cubemap

    glActiveTexture(GL_TEXTURE0);
    // Activamos la Textura del cubemap por su nombre
    OpenGLRenderer::ActivateTextureCubeMap("Desert");

    OpenGLRenderer::ActivateCubeMapShader();

    glm::mat4 viewNotraslation = glm::mat4(glm::mat3(Camera::activeCamera->GetViewMatrix()));  // Sin traslación para el cubemap
	glm::mat4 projection = Camera::activeCamera->GetProjectionMatrix();
    OpenGLRenderer::g_shaders.cubeMap.SetMat4("view", viewNotraslation);
    OpenGLRenderer::g_shaders.cubeMap.SetMat4("projection", projection);

    // Activamos el VAO del cubemap
    OpenGLRenderer::ActivateCubeMapVAO();


    glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glDepthFunc(GL_LESS);  // Restaura el test de profundidad
    
}


void OpenGLRenderer::ActivateCubeMirrorShader() {
  g_shaders.CubeMirror.Use();
}

void OpenGLRenderer::ActivateCubeDefectShader() {
	g_shaders.DefectCube.Use();
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

  std::cout << "CubeMap VAO delete" << std::endl;
}

void OpenGLRenderer::DeleteCubeMapVBO() {
  unsigned int cubeMapVBO = GLBackVertex::GetCubeMapVBO();
  glDeleteBuffers(1, &cubeMapVBO);
  std::cout << "CubeMap VBO delete" << std::endl;
}



