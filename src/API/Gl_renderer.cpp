#include "../Enviroment/CV_Enviroment.h"
#include "Gl_renderer.h"
#include "../Back/Back.h"
#include "Types/GL_shader.h"
#include "GL_BackVertex.h"
#include "../Core/AssetManager.h"
#include "../Math/Frustum.h"
#include "../Renderer/Types/Lighting.h"

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
  Frustum frustumA;

}  // namespace OpenGLRenderer

void CubePass();
void CubeMapPass();
void LightPass();

float cubeHalfSize = 0.5f;

void OpenGLRenderer::HotloadShaders() {

  std::cout << "Hotloading shaders...\n";

  g_shaders.DefectCube.Load("GL_CubeDefect.vert", "GL_CubeDefect.frag");
  g_shaders.CubeMirror.Load("GL_CubeMirror.vert", "GL_CubeMirror.frag");
  g_shaders.cubeMap.Load("GL_CubeMap.vert", "GL_CubeMap.frag");
  g_shaders.lighting.Load("GL_Light.vert", "GL_Light.frag");

  /*Dem�s Shaders*/

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

  // Creaci�n de VAO y VBO para el cubo
  AssetManager::UploadVertexData();
  AssetManager::UploadVertexLight();

  //Creaci�n de VAO y VBO para CubeMap

  AssetManager::UploadVertexDataCubeMap();


}
float zPosition;

void OpenGLRenderer::RenderFrame() {

    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    LightPass();
    CubePass();
    CubeMapPass();

}

void renderCube(glm::vec3 CubeCenter, glm::mat4 modelObj) {

  glm::mat4 model = glm::mat4(1.0f);

  if (OpenGLRenderer::frustumA.IntersectsCube(CubeCenter, cubeHalfSize)) {

    model = glm::translate(model, CubeCenter);

    glm::mat4 modelObject = model * modelObj;
    
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(modelObject));

    OpenGLRenderer::g_shaders.DefectCube.SetMat3("normalMatrix", normalMatrix);

    OpenGLRenderer::g_shaders.DefectCube.SetMat4("model", modelObject);


    glDrawArrays(GL_TRIANGLES, 0, 50);
  }
}

void LightPass() {

    //SetMat4 pasa las matrices al shader /solo shaders del cubo/ y este pueda realizar las transformaciones de los v�rtices
    
    glm::mat4 view = Camera::activeCamera->GetViewMatrix();
    glm::mat4 projection = Camera::activeCamera->GetProjectionMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    // Activaci�n del shader del cubo
    OpenGLRenderer::ActivateCubeLightShader();
    //
    float time = static_cast<float>(Back::GetCurrentFrame()) / 4.0f;

    // Define el radio de la circunferencia
    float radius = 3.0f;

    // Calcula la posición de la luz en la circunferencia
    float xPosition = radius * std::cos(time);
    float zPosition = radius * std::sin(time);

    // Actualiza la posición de la luz
    glm::vec3 lightPosition = glm::vec3(xPosition, Light::light->GetPosition().y, zPosition);
    Light::light->SetPosition(lightPosition);


    glm::mat4 modelFirstCube = glm::translate(glm::mat4(1.0f), Light::light->GetPosition());
    modelFirstCube = glm::scale(modelFirstCube, glm::vec3(0.2f, 0.2f, 0.2f));


    OpenGLRenderer::g_shaders.lighting.SetMat4("view", view);
    OpenGLRenderer::g_shaders.lighting.SetMat4("projection", projection);
	OpenGLRenderer::g_shaders.lighting.SetMat4("model", modelFirstCube);

	OpenGLRenderer::g_shaders.lighting.SetVec4("lightColor", Light::light->GetLightColor());
    // Activaci�n del VAO del cubo
    OpenGLRenderer::ActivateCubeLightVAO();

    glDrawArrays(GL_TRIANGLES, 0, 50);
    
}
    
void CubePass() {

    //SetMat4 pasa las matrices al shader /solo shaders del cubo/ y este pueda realizar las transformaciones de los v�rtices

    glm::mat4 view = Camera::activeCamera->GetViewMatrix();
    glm::mat4 projection = Camera::activeCamera->GetProjectionMatrix();     

    glm::mat4 viewProjectionMatrix = projection * view;

    OpenGLRenderer::frustumA.Update(viewProjectionMatrix);

    // Activaci�n del shader del cubo
    OpenGLRenderer::ActivateCubeDefectShader();

    OpenGLRenderer::g_shaders.DefectCube.SetMat4("view", view);
    OpenGLRenderer::g_shaders.DefectCube.SetMat4("projection", projection);
 
    // Oscilación de la posición de la luz en el eje z

    //Light Point
    OpenGLRenderer::g_shaders.DefectCube.SetVec3("light.Color", Light::light->GetLightColor());
    OpenGLRenderer::g_shaders.DefectCube.SetVec3("light.position", Light::light->GetPosition());
    OpenGLRenderer::g_shaders.DefectCube.SetFloat("light.strength", Light::light->GetStrength());
    OpenGLRenderer::g_shaders.DefectCube.SetVec3("viewPos", Camera::activeCamera->GetCameraPos());
    OpenGLRenderer::g_shaders.DefectCube.SetFloat("light.constant", Light::light->GetLightConstant());
    OpenGLRenderer::g_shaders.DefectCube.SetFloat("light.linear", Light::light->GetLightLinear());
    OpenGLRenderer::g_shaders.DefectCube.SetFloat("light.quadratic", Light::light->GetLightQuadratic());

    //light Directional
    OpenGLRenderer::g_shaders.DefectCube.SetVec3("dirLight.direction", glm::vec3(0.0, -1.0, -0.8f));
	OpenGLRenderer::g_shaders.DefectCube.SetVec3("dirLight.Color", glm::vec3(1.00f, 1.00f, 1.00f));
	OpenGLRenderer::g_shaders.DefectCube.SetFloat("dirLight.strength", 5.0f);

    // Activaci�n del VAO del cubo
    OpenGLRenderer::ActivateCubeVAO();

    // Activaci�n de la textura del cubo por su nombre.
    glActiveTexture(GL_TEXTURE1);
    OpenGLRenderer::ActivateCubeTexture("Ladrillo_ALB");
    glActiveTexture(GL_TEXTURE2);
    OpenGLRenderer::ActivateCubeTexture("Ladrillo_NRM");
    glActiveTexture(GL_TEXTURE3);
    OpenGLRenderer::ActivateCubeTexture("Ladrillo_RMA");


    glm::vec3 firstCubeCenter(0.0f, 0.0f, 0.0f);
    glm::mat4 modelFirstCube = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    modelFirstCube = glm::rotate(modelFirstCube, 0.0f/*(float)Back::GetCurrentFrame() / 6*/, glm::vec3(1.0f, 0.0f, 1.0f));

    renderCube(firstCubeCenter, modelFirstCube);

    glm::vec3 secondCubeCenter(1.0f, 0.0f, 0.0f);
    glm::mat4 modelSecondCube = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    renderCube(secondCubeCenter, modelSecondCube);

    glm::vec3 secondCubeCenter3(-1.0f, 0.0f, 0.0f);
    glm::mat4 modelSecondCube3 = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    renderCube(secondCubeCenter3, modelSecondCube3);

    glm::vec3 secondCubeCenter4(0.0f, 1.0f, 0.0f);
    glm::mat4 modelSecondCube4 = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    renderCube(secondCubeCenter4, modelSecondCube4);

    glm::vec3 secondCubeCenter5(0.0f, -1.0f, 0.0f);
    glm::mat4 modelSecondCube5 = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    renderCube(secondCubeCenter5, modelSecondCube5);

    glm::vec3 secondCubeCenter6(1.0f, 1.0f, 0.0f);
    glm::mat4 modelSecondCube6 = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    renderCube(secondCubeCenter6, modelSecondCube6);

    glm::vec3 a(-1.0, -1.0, 0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    renderCube(a, model);

    glm::vec3 b(1.0, -1.0, 0.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
    renderCube(b, model2);

    glm::vec3 c(-1.0, 1.0, 0.0f);
    glm::mat4 model3 = glm::mat4(1.0f);
    renderCube(c, model3);
}

void CubeMirrorPass() {
    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //SetMat4 pasa las matrices al shader /solo shaders del cubo/ y este pueda realizar las transformaciones de los v�rtices

    glm::mat4 view = Camera::activeCamera->GetViewMatrix();
    glm::mat4 projection = Camera::activeCamera->GetProjectionMatrix();

    glm::mat4 viewProjectionMatrix = projection * view;

    OpenGLRenderer::frustumA.Update(viewProjectionMatrix);
    // Activaci�n del shader del cubo
    OpenGLRenderer::ActivateCubeMirrorShader();

    OpenGLRenderer::g_shaders.CubeMirror.SetMat4("view", view);
    OpenGLRenderer::g_shaders.CubeMirror.SetMat4("projection", projection);
    OpenGLRenderer::g_shaders.CubeMirror.SetVec3("cameraPos", Camera::activeCamera->GetCameraPos());
    // Activaci�n del VAO del cubo
    OpenGLRenderer::ActivateCubeVAO();

    // Activaci�n de la textura del cubo por su nombre.
    glActiveTexture(GL_TEXTURE1);
    OpenGLRenderer::ActivateCubeTexture("Wood");
    OpenGLRenderer::g_shaders.CubeMirror.SetInt("texture1", 1);

    OpenGLRenderer::g_shaders.CubeMirror.SetInt("skybox", 0);

    glm::vec3 firstCubeCenter(0.0f, 0.0f, 0.0f);
    glm::mat4 modelFirstCube = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    renderCube(firstCubeCenter, modelFirstCube);

    /* glm::vec3 secondCubeCenter(3, 3, 3);
     glm::mat4 modelSecondCube = glm::rotate(glm::mat4(1.0f), (float)Back::GetCurrentFrame() / 4, glm::vec3(0.0f, 1.0f, 0.0f));
     renderCube(secondCubeCenter, modelSecondCube);*/
}


void CubeMapPass() {

  // Activaci�n del cubemap

  glDepthFunc(GL_LEQUAL);  // Cambiar test de profundidad para el cubemap

  glActiveTexture(GL_TEXTURE0);
  // Activamos la Textura del cubemap por su nombre
  OpenGLRenderer::ActivateTextureCubeMap("Moonlight");

  OpenGLRenderer::ActivateCubeMapShader();

  glm::mat4 viewNotraslation = glm::mat4(glm::mat3(Camera::activeCamera->GetViewMatrix()));  // Sin traslaci�n para el cubemap
  glm::mat4 projection = Camera::activeCamera->GetProjectionMatrix();
  OpenGLRenderer::g_shaders.cubeMap.SetMat4("view", viewNotraslation);
  OpenGLRenderer::g_shaders.cubeMap.SetMat4("projection", projection);

  // Activamos el VAO del cubemap
  OpenGLRenderer::ActivateCubeMapVAO();

  glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);

  glDepthFunc(GL_LESS);  // Restaura el test de profundidad

}

void OpenGLRenderer::ActivateCubeLightShader() {
	g_shaders.lighting.Use();
}

void OpenGLRenderer::ActivateCubeLightVAO() {
	glBindVertexArray(GLBackVertex::GetCubeLightVAO());
}

void OpenGLRenderer::DeleteCubeLightVAO() {
	unsigned int cubeLightVAO = GLBackVertex::GetCubeLightVAO();
	glDeleteVertexArrays(1, &cubeLightVAO);

	std::cout << "CubeLight VAO delete" << std::endl;
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
  std::cout << "Cube VBO delete" << std::endl;
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



