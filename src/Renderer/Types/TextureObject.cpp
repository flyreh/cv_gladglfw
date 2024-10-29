#include "../../Back/Back.h"
#include "../../Util.hpp"
#include "TextureObject.h"


TextureObject::TextureObject(std::string fullpath, bool compressed) {
  m_compressed = compressed;
  m_fullPath = fullpath;
  m_fileName = Util::GetFilename(m_fullPath);
  m_fileType = Util::GetFileInfo(m_fullPath).filetype;
}

void TextureObject::Load() {


    glTexture.Load(m_fullPath, m_compressed);
  
  return;
}

void TextureObject::Bake() {

      glTexture.Bake();  
  
}

int TextureObject::GetWidth() {

    return glTexture.GetWidth();
  
}

int TextureObject::GetHeight() {
 
    return glTexture.GetHeight();
 
}

std::string& TextureObject::GetFilename() {
  return m_fileName;

}

std::string& TextureObject::GetFiletype() {
  return m_fileType;
 
}

OpenGLTexture& TextureObject::GetGLTexture() {
  return glTexture;
}



//void TextureObject::SetLoadingState(LoadingState loadingState) {
//  m_loadingState = loadingState;
//}
//
//const LoadingState TextureObject::GetLoadingState() {
//  return m_loadingState;
//}
//
//const BakingState TextureObject::GetBakingState() {
//  return m_bakingState;
//}