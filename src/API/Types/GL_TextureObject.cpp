#include <iostream>
#include "../../Util.hpp"
#include "GL_TextureObject.h"
#include <stb_image.h>



//uint32_t CmpToOpenGlFormat(CMP_FORMAT format) {
//  //std::cout << "FORMAT IS: " << format << "\n";
//  switch (format) {
//    case CMP_FORMAT_DXT1:
//      return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
//    case CMP_FORMAT_DXT3:
//      return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
//    case CMP_FORMAT_DXT5:
//      return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
//    case CMP_FORMAT_BC4:
//      return GL_COMPRESSED_RED_RGTC1;  // Single-channel compressed format
//    case CMP_FORMAT_BC5:
//      return GL_COMPRESSED_RG_RGTC2;  // Two-channel compressed format
//    case CMP_FORMAT_ATI2N_XY:
//      return GL_COMPRESSED_RG_RGTC2;  // Two-channel compressed format
//    case CMP_FORMAT_BC6H:
//      return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;  // HDR format
//    case CMP_FORMAT_BC7:
//      return GL_COMPRESSED_RGBA_BPTC_UNORM;  // High-quality RGBA
//    case CMP_FORMAT_ETC2_RGB:
//      return GL_COMPRESSED_RGB8_ETC2;
//    case CMP_FORMAT_ETC2_SRGB:
//      return GL_COMPRESSED_SRGB8_ETC2;
//    case CMP_FORMAT_ETC2_RGBA:
//      return GL_COMPRESSED_RGBA8_ETC2_EAC;
//    case CMP_FORMAT_ETC2_SRGBA:
//      return GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC;
//    case CMP_FORMAT_ASTC:
//      return GL_COMPRESSED_RGBA_ASTC_4x4_KHR;  // Assuming ASTC 4x4 block size
//    case CMP_FORMAT_BC1:
//      return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
//    case CMP_FORMAT_BC2:
//      return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
//    default:
//      return 0xFFFFFFFF;  // Invalid format
//  }
//}


//TextureData LoadEXRData(std::string filepath);
TextureData LoadTextureData(std::string filepath);
TextureData LoadDDSTextureData(std::string filepath);

CompressedTextureData LoadCompressedDDSFromDisk(const char* filepath);

CompressedTextureData LoadCompressedDDSFromDisk(const char* filepath) {

  CompressedTextureData textureData;
  gli::texture texture = gli::load(filepath);
  if (texture.empty()) {
    std::cerr << "Failed to load compressed DDS texture: " << filepath << std::endl;
    return textureData;
  }
  gli::gl GL(gli::gl::PROFILE_GL33);
  textureData.data = (void*)texture.data(0, 0, 0);
  textureData.format = GL.translate(texture.format(), texture.swizzles()).Internal;
  textureData.width = static_cast<GLsizei>(texture.extent().x);
  textureData.height = static_cast<GLsizei>(texture.extent().y);
  textureData.size = static_cast<GLsizei>(texture.size(0));
  textureData.target = texture.target();

  return textureData;
}

//TextureData LoadEXRData(std::string filepath) {
//  TextureData textureData;
//  const char* err = nullptr;
//  const char** layer_names = nullptr;
//  int num_layers = 0;
//  bool status = EXRLayers(filepath.c_str(), &layer_names, &num_layers, &err);
//  if (err) {
//    fprintf(stderr, "EXR error = %s\n", err);
//  }
//  if (status != 0) {
//    fprintf(stderr, "Load EXR err: %s\n", err);
//    std::cout << " GetEXRLayers() FAILED \n";
//  }
//  if (num_layers > 0) {
//    fprintf(stdout, "EXR Contains %i Layers\n", num_layers);
//    for (int i = 0; i < (int)num_layers; ++i) {
//      fprintf(stdout, "Layer %i : %s\n", i + 1, layer_names[i]);
//    }
//  }
//  free(layer_names);
//  const char* layername = NULL;
//  float* floatPtr = nullptr;
//  status = LoadEXRWithLayer(&floatPtr, &textureData.m_width, &textureData.m_height,
//                            filepath.c_str(), layername, &err);
//  if (err) {
//    fprintf(stderr, "EXR error = %s\n", err);
//  }
//  if (status != 0) {
//    fprintf(stderr, "Load EXR err: %s\n", err);
//    std::cout << " LoadEXRRGBA() FAILED \n";
//  }
//  textureData.m_data = floatPtr;
//  return textureData;
//}


bool OpenGLTexture::Load(const std::string filepath, bool compressed) {

  if (!Util::FileExists(filepath)) {
    std::cout << filepath << " does not exist.\n";
    return false;
  }
  m_filename = Util::GetFilename(filepath);
  m_filetype = Util::GetFileInfo(filepath).filetype;

  TextureData textureData = LoadTextureData(filepath);
  this->m_data = textureData.m_data;
  this->_width = textureData.m_width;
  this->_height = textureData.m_height;
  this->_NumOfChannels = textureData.m_numChannels;

  std::cout << "Load Texture : " << m_filename << " width : " << textureData.m_width
            << ", height : " << textureData.m_height << "Numero de canales : "<<textureData.m_numChannels<<std::endl;

  return true;
}

//const char* GetGLFormatString(GLenum format) {
//  switch (format) {
//    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
//      return "GL_COMPRESSED_RGB_S3TC_DXT1_EXT";
//    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
//      return "GL_COMPRESSED_RGBA_S3TC_DXT1_EXT";
//    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
//      return "GL_COMPRESSED_RGBA_S3TC_DXT3_EXT";
//    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
//      return "GL_COMPRESSED_RGBA_S3TC_DXT5_EXT";
//    case GL_COMPRESSED_RED_RGTC1:
//      return "GL_COMPRESSED_RED_RGTC1";
//    case GL_COMPRESSED_SIGNED_RED_RGTC1:
//      return "GL_COMPRESSED_SIGNED_RED_RGTC1";
//    case GL_COMPRESSED_RG_RGTC2:
//      return "GL_COMPRESSED_RG_RGTC2";
//    case GL_COMPRESSED_SIGNED_RG_RGTC2:
//      return "GL_COMPRESSED_SIGNED_RG_RGTC2";
//    case GL_COMPRESSED_RGB8_ETC2:
//      return "GL_COMPRESSED_RGB8_ETC2";
//    case GL_COMPRESSED_SRGB8_ETC2:
//      return "GL_COMPRESSED_SRGB8_ETC2";
//    case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
//      return "GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2";
//    case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
//      return "GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2";
//    case GL_COMPRESSED_RGBA8_ETC2_EAC:
//      return "GL_COMPRESSED_RGBA8_ETC2_EAC";
//    case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
//      return "GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC";
//      // Add more formats as needed
//    default:
//      return "Unknown Format";
//  }
//}

TextureData LoadTextureData(std::string filepath) {

  std::cout << "filepathTexture : " << filepath << std::endl;
  stbi_set_flip_vertically_on_load(false);
  TextureData textureData;
  std::cout << "filepath data : " << filepath.data() << std::endl;
  textureData.m_data = stbi_load(filepath.data(), &textureData.m_width, &textureData.m_height,
                                 &textureData.m_numChannels, STBI_rgb_alpha);

  if (textureData.m_data == nullptr) {
    std::cerr << "Error al cargar la textura: " << filepath << std::endl;
    return {};
  }

  return textureData;
}

TextureData LoadDDSTextureData(std::string filepath) {
  TextureData textureData;
  return textureData;
}


bool OpenGLTexture::Bake() {

    int maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    if (_width > maxTextureSize || _height > maxTextureSize) {
        std::cout << "Error: Tamaño de la textura excede el máximo soportado (" << maxTextureSize << ")" << std::endl;
        return false;
    }
    
    if (m_data == nullptr) {
      std::cout << "Error: m_data es nullptr en Bake()." << std::endl;
      return false;
    }
   
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);

    glGenerateMipmap(GL_TEXTURE_2D);

     if (m_data == nullptr) {
      std::cout << "ATTENTION! OpenGLTexture::Bake() called but m_data was nullptr:" << m_filename
                << "\n";
      return false;
    }
   
    //uso de la libreria stb para liberar la memoria de la textura
     stbi_image_free(m_data);
  

  std::cout << "Baked texture: " << m_filename << ", ID: " << ID << std::endl;

  m_data = nullptr;
  return true;
}


//void OpenGLTexture::BakeCMPData(CMP_Texture* cmpTexture) {
//  uint32_t glFormat = CmpToOpenGlFormat(cmpTexture->format);
//  if (glFormat == 0xFFFFFFFF) {
//    std::cout << "Invalid format! Failed to load compressed texture: " << m_filename << "\n";
//    return;
//  }
//  glGenTextures(1, &ID);
//  glBindTexture(GL_TEXTURE_2D, ID);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  uint32_t width = cmpTexture->dwWidth;
//  uint32_t height = cmpTexture->dwHeight;
//  uint32_t size = cmpTexture->dwDataSize;
//  glCompressedTexImage2D(GL_TEXTURE_2D, 0, glFormat, cmpTexture->dwWidth, cmpTexture->dwHeight, 0,
//                         size, cmpTexture->pData);
//  glGenerateMipmap(GL_TEXTURE_2D);
//  freeCMPTexture(cmpTexture);
//  if (!dontDoBindless) {
//    bindlessID = glGetTextureHandleARB(ID);
//    glMakeTextureHandleResidentARB(bindlessID);
//  }
//}

unsigned int OpenGLTexture::GetID() {
  return ID;
}

void OpenGLTexture::Bind(unsigned int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, ID);
}

int OpenGLTexture::GetWidth() {
  return _width;
}

int OpenGLTexture::GetHeight() {
  return _height;
}

std::string& OpenGLTexture::GetFilename() {
  return m_filename;
}

std::string& OpenGLTexture::GetFiletype() {
  return m_filetype;
}

/*
bool OpenGLTexture::IsBaked() {
    return _baked;
}
*/