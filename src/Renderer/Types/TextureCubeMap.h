#pragma once 

#include <string>
#include "../../API/Types/GL_TextureCubeMap.h"

class TextureCubeMap {
 public:
  TextureCubeMap(std::string _fullpath){ m_fullPath = _fullpath;}

  void Load();
  void SetName(std::string name);
  void SetFileType(std::string filetype);
  std::string GetFullpath();
  std::string GetFileType();
  std::string GetName();

  OpenGLCubemapTexture& GetGLtextureCubeMap();

  std::string m_fullPath = "";



 private:

	 OpenGLCubemapTexture glTextureCubeMap;
	 std::string name;
     std::string fileType;

};

