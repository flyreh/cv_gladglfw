#pragma once 

#include "../Enviroment/CV_Enviroment.h"
#include "../Renderer/Types/TextureObject.h"
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

	
	// Vertices
    void UploadVertexData();
    void CreateVertexData();

	// Para modelos



}