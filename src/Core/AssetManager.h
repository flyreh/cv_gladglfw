#pragma once 

#include "../Enviroment/CV_Enviroment.h"
#include "../Renderer/Types/TextureObject.h"
#include "../Renderer/Types/TextureCubeMap.h"
namespace AssetManager {
	
	void LoadAssetPath();

	
	//Texturas
	void LoadTexture(TextureObject* texture);
    TextureObject* GetTextureByName(const std::string& name);
    TextureObject* GetTextureByIndex(const int index);
    int GetTextureCount();
    int GetTextureIndexByName(const std::string& filename, bool ignoreWarning = false);
    bool TextureExists(const std::string& name);
    std::vector<TextureObject>& GetTextures();

	// Cubemaps

	void LoadCubemapTexture(TextureCubeMap* texture);
   // TextureCubeMap* GetCubemapTextureByName(const std::string& name);
    TextureCubeMap* GetCubemapTextureByIndex(const int index);
   // int GetCubemapTextureCount();
    int GetCubemapTextureIndexByName(const std::string& name);
   // bool CubemapTextureExists(const std::string& name);
   // std::vector<TextureCubeMap>& GetCubemapTextures();
    TextureCubeMap* GetCubemapTextureByName(const std::string& name);

    void CreateIndicesCubeMapData();

	
	// Vertices Cube
    void UploadVertexData();
    void CreateVertexData();

    void UploadVertexDataCubeMap();
    void CreateVertexDataCubeMap();

	// Para modelos



}