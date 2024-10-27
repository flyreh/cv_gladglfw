#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#include <string>
#include "../../Enviroment/CV_Enviroment.h"

struct OpenGLTexture {

  OpenGLTexture() = default;
  //explicit OpenGLTexture(const std::string filepath);
  GLuint GetID();
 
  void Bind(unsigned int slot);
  bool Load(const std::string filepath, bool compressed);
  bool Bake();
 // void BakeCMPData(CMP_Texture* cmpTexture);
  //void UploadToGPU(void* data, CMP_Texture* cmpTexture, int width, int height, int channelCount);
  //bool IsBaked();
  int GetWidth();
  int GetHeight();
  std::string& GetFilename();
  std::string& GetFiletype();

 private:
  GLuint ID;
  GLuint64 bindlessID;
  std::string m_fullPath;
  std::string m_filename;
  std::string m_filetype;
  bool m_compressed = false;

  //unsigned char* _data = nullptr;
  //float* _floatData = nullptr;

  //CompressedTextureData m_compressedTextureData;

  unsigned char* m_data = nullptr;

  int _NumOfChannels = 0;
  int _width = 0;
  int _height = 0;
  //bool _baked = false;
};
