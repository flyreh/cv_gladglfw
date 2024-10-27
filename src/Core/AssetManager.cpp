#define TINYOBJLOADER_IMPLEMENTATION
#include "AssetManager.h"
#include "../API/GL_BackVertex.h"
#include "../API/GL_renderer.h"
#include "../Back/Back.h"
#include "../Util.hpp"
#include <future>
#include <thread>
#include <numeric>
#include <glm/glm.hpp>
#include <algorithm>
#include <numeric>
#include <vector>
#include <thread>
#include <chrono>

#include <stb_image.h>
//#include "tiny_obj_loader.h"

namespace AssetManager {

    struct CompletedLoadingTasks {
      bool g_hardcodedModels = false;
      bool g_materials = false;
      bool g_texturesBaked = false;
      bool g_cubemapTexturesBaked = false;
      bool g_cmpTexturesBaked = false;
      bool g_all = false;
    } g_completedLoadingTasks;

    std::vector<std::string> g_loadLog;

    std::vector<float> g_vertices;

    std::vector<std::future<void>> _futures;

    std::vector<WeightedVertex> g_weightedVertices;
    std::vector<uint32_t> g_indices;
    std::vector<uint32_t> g_weightedIndices;

    //std::vector<Mesh> g_meshes;
    //std::vector<Model> g_models;

    std::vector<TextureObject> g_textures;
    //std::vector<Material> g_materials;
    //std::vector<CubemapTexture> g_cubemapTextures;
    //std::vector<GPUMaterial> g_gpuMaterials;

    std::unordered_map<std::string, int> g_materialIndexMap;
    std::unordered_map<std::string, int> g_modelIndexMap;
    std::unordered_map<std::string, int> g_meshIndexMap;

    std::unordered_map<std::string, int> g_textureIndexMap;


    // Used to new data insert into the vectors above
    int _nextVertexInsert = 0;
    int _nextIndexInsert = 0;
    int _nextWeightedVertexInsert = 0;
    int _nextWeightedIndexInsert = 0;

    int _upFacingPlaneMeshIndex = 0;
    int _quadMeshIndex = 0;
    int _quadMeshIndexSplitscreenTop = 0;
    int _quadMeshIndexSplitscreenBottom = 0;
    int _quadMeshIndexQuadscreenTopLeft = 0;
    int _quadMeshIndexQuadscreenTopRight = 0;
    int _quadMeshIndexQuadscreenBottomLeft = 0;
    int _quadMeshIndexQuadscreenBottomRight = 0;
    int _halfSizeQuadMeshIndex = 0;

    
}  // namespace AssetManager


void AssetManager::LoadAssetPath() {
    

      // Textures

      auto texturePaths = std::filesystem::directory_iterator(std::string(SHADERS_DIR) + "textures/");
      for (const auto& entry : texturePaths) {
        FileInfo info = Util::GetFileInfo(entry);

        if (info.filetype == "png" || info.filetype == "jpg" || info.filetype == "tga") {
          g_textures.emplace_back(TextureObject(info.fullpath, false));
        }
      }

      /*auto uiTexturePaths = std::filesystem::directory_iterator("res/textures/ui/");
      for (const auto& entry : uiTexturePaths) {
        FileInfo info = Util::GetFileInfo(entry);
        if (info.filetype == "png" || info.filetype == "jpg" || info.filetype == "tga") {
          g_textures.emplace_back(Texture(info.fullpath, false));
        }
      }*/

      // Cargar Arreglo de texturas
      for (TextureObject& texture : g_textures) {
         
          LoadTexture(&texture);
          //_futures.push_back(std::async(std::launch::async, LoadTexture, &texture));
        
      }

      ////Bake Texturas
      for (TextureObject& texture : g_textures) {
        
        texture.Bake();
      }

      std::cout<<"Uploading textures and models to GPU"<<std::endl;
     
 }

void AssetManager::CreateVertexData() {
    g_vertices = {
    // Positions (3)   // Texture Coords (2)
    // Trasera (z = -0.5)
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

    // frontal (z = 0.5)
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

    // Izquierda (x = -0.5)
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,

    // Derecha (x = 0.5)
    0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

    // Inferior (y = -0.5)
    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,

    // Superior (y = 0.5)
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

};

}

void AssetManager::UploadVertexData() {
    //Carga de vertices del objeto.
   CreateVertexData(); 

   // Creaci�n de VAO and VBO para el cubo
   GLBackVertex::UploadCubeVertexData(g_vertices);
    
}

bool AssetManager::TextureExists(const std::string& filename) {
  for (TextureObject& texture : g_textures)
    if (texture.GetFilename() == filename)
      return true;
  return false;
}


int AssetManager::GetTextureCount() {
  return g_textures.size();
}

int AssetManager::GetTextureIndexByName(const std::string& name, bool ignoreWarning) {
  auto it = g_textureIndexMap.find(name);
  if (it != g_textureIndexMap.end()) {
    return it->second;
  } else {
    /*std::cout << "g_textureIndexMap.size(): " << g_textureIndexMap.size() << "\n";
        for (const auto& pair : g_textureIndexMap) {
            std::cout << pair.first << ": " << pair.second << '\n';
        }*/
    if (!ignoreWarning) {
      std::cout << "AssetManager::GetTextureIndexByName() failed because '" << name
                << "' does not exist\n";
    }
    return -1;
  }
}

TextureObject* AssetManager::GetTextureByIndex(const int index) {
  if (index >= 0 && index < g_textures.size()) {
    return &g_textures[index];
  }
  std::cout << "AssetManager::GetTextureByIndex() failed because index '" << index
            << "' is out of range. Size is " << g_textures.size() << "!\n";
  return nullptr;
}

TextureObject* AssetManager::GetTextureByName(const std::string& name) {
  for (TextureObject& texture : g_textures) {
    if (texture.GetFilename() == name)
      return &texture;
  }
  std::cout << "AssetManager::GetTextureByName() failed because '" << name << "' does not exist\n";
  return nullptr;
}

void AssetManager::LoadTexture(TextureObject* texture) {
  texture->Load();
}

std::vector<TextureObject>& AssetManager::GetTextures() {
  return g_textures;
}