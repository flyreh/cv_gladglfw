#include "TextureCubeMap.h"

void TextureCubeMap::SetName(std::string _name) {
  this->name = _name;
}

void TextureCubeMap::SetFileType(std::string filetype) {
  this->fileType = filetype;
}


void TextureCubeMap::Load() {

  glTextureCubeMap.Load(name, fileType);

}

OpenGLCubemapTexture& TextureCubeMap::GetGLtextureCubeMap() {
  return glTextureCubeMap;
}

std::string TextureCubeMap::GetFullpath() {
  return m_fullPath;
}

std::string TextureCubeMap::GetFileType() {
  return fileType;
}

std::string TextureCubeMap::GetName() {
  return name;
}