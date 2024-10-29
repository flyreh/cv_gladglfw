#pragma once
#include <memory>
#include <string>
#include "../../Enviroment/CV_Enviroment.h"


struct OpenGLCubemapTexture {

 public:
  OpenGLCubemapTexture() = default;
  void Load(std::string filepath, std::string filetype);
  void Bake();
 // void Activate();
  unsigned int GetID();
  unsigned int GetWidth();
  unsigned int GetHeight();

  TextureData m_textureData[6];

 private:
  unsigned int ID = 0;
  int width = 0;
  int height = 0;
};