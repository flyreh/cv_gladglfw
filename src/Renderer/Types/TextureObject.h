#pragma once

#include <memory>
#include <string>
#include "../../API/Types/GL_TextureObject.h"


class TextureObject {

 public:
  TextureObject() = default;
  TextureObject(std::string fullpath, bool compressed);
  void Load();
  void Bake();

  int GetWidth();
  int GetHeight();
  std::string& GetFilename();
  std::string& GetFiletype();
  OpenGLTexture& GetGLTexture();



  /*const LoadingState GetLoadingState();
  const BakingState GetBakingState();*/

  std::string m_fullPath = "";
  bool m_compressed = false;

 private:
  OpenGLTexture glTexture;

  std::string m_fileName;
  std::string m_fileType;
  int width = 0;
  int height = 0;
  int channelCount = 0;
  /*LoadingState m_loadingState = LoadingState::AWAITING_LOADING_FROM_DISK;
  BakingState m_bakingState = BakingState::AWAITING_BAKE;*/
};
