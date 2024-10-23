#include "Gl_renderer.h"
#include "../Enviroment/CV_Enviroment.h"
#include "Types/GL_shader.h"
#include "../Back/Back.h"

struct LightVolumeData
{
    glm::vec4 aabbMin;
    glm::vec4 aabbMax;
};

struct TileData
{
    int lightCount;
    int lightIndices[127];
};

namespace OpenGLRenderer
{

    struct Shaders
    {
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
        Shader decalsBlood;
        Shader decalsBullet;
        Shader vatBlood;
        Shader skyBox;
        Shader debugProbes;
        Shader csg;
        Shader outline;
        Shader gbufferSkinned;
        Shader csgSubtractive;
        Shader triangles2D;
        Shader heightMap;
        Shader debugLightVolumeAabb;
        Shader winston;
        Shader megaTextureBloodDecals;

    } g_shaders;

}

void OpenGLRenderer::HotloadShaders()
{

    std::cout << "Hotloading shaders...\n";

    g_shaders.geometry.Load("GL_Cube.vert", "GL_Cube.frag");
}

void OpenGLRenderer::InitMinimum()
{
    HotloadShaders();

    // carga de buffers VAO y VBO para objetos 3D
}

void geometryPass()
{
    Shader &shader = OpenGLRenderer::g_shaders.geometry;
    shader.Use();
}
