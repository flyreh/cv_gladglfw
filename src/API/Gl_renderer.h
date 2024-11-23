#pragma once
#include "../Enviroment/CV_Enviroment.h"
#include "Types/GL_shader.h"
#include "../Math/Frustum.h"

namespace OpenGLRenderer {

  struct Shaders {
        Shader geometry;
        Shader lighting;
        Shader UI;
        Shader shadowMap;
        Shader shadowMapCSG;
        Shader debugSolidColor;
        Shader debugSolidColor2D;
        Shader debugPointCloud;
        Shader flipBook;
        Shader glass;
        Shader horizontalBlur;
        Shader verticalBlur;
        Shader decalsBullet;
        Shader vatBlood;
        Shader debugProbes;
        Shader csg;
        Shader outline;
        Shader gbufferSkinned;
        Shader csgSubtractive;
        Shader triangles2D;
        Shader heightMap;
        Shader debugLightVolumeAabb;
        Shader winston;
        Shader DefectCube;
        Shader CubeMirror;
        Shader cubeMap;
    };

    extern Shaders g_shaders;


    void InitMinimum();
  
    void HotloadShaders();

    void ObjectsPass();

    void RenderFrame();

    void ActivateCubeMirrorShader();
   
	void ActivateCubeDefectShader();

    void ActivateCubeVAO();

    void ActivateCubeTexture(std::string textureName);

    void DeleteCubeVAO();

    void DeleteCubeVBO();

    void ActivateCubeMapShader();

    void ActivateCubeMapVAO();

    void ActivateTextureCubeMap(std::string textureName);

    void DeleteCubeMapVAO();

    void DeleteCubeMapVBO();



    /* para lo .obj
    

    */

}  // namespace OpenGLRenderer