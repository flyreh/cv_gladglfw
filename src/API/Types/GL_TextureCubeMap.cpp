#include "../../Util.hpp"
#include "GL_TextureCubeMap.h"
#include <string>
#include <stb_image.h>

void OpenGLCubemapTexture::Load (std::string name, std::string filetype) {

	 std::vector<std::string> filepaths;
    filepaths.push_back(std::string(SHADERS_DIR) + "textures/cubemap/" + name + "_Right." + filetype);
    filepaths.push_back(std::string(SHADERS_DIR) + "textures/cubemap/" + name + "_Left." + filetype);
    filepaths.push_back(std::string(SHADERS_DIR) + "textures/cubemap/" + name + "_Top." + filetype);
    filepaths.push_back(std::string(SHADERS_DIR) + "textures/cubemap/" + name + "_Bottom." + filetype);
    filepaths.push_back(std::string(SHADERS_DIR) + "textures/cubemap/" + name + "_Front." + filetype);
    filepaths.push_back(std::string(SHADERS_DIR) + "textures/cubemap/" + name + "_Back." + filetype);
    

     glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

     glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    for (unsigned int i = 0; i < 6; i++) {
        m_textureData[i].m_data = stbi_load(filepaths[i].c_str(), &m_textureData[i].m_width, &m_textureData[i].m_height, &m_textureData[i].m_numChannels, 0 );
    }

    for (int i = 0; i < 6; i++) {
      if (m_textureData[i].m_data != nullptr) {
        std::cout << "Loaded Texture: " << filepaths[i] << std::endl;
      } else {
        std::cout << "Failed to load Texture: " << filepaths[i] << std::endl;
      }
    }

    std::cout << "ID : CubeMap loaded and baked " << ID << std::endl;

    width = m_textureData[0].m_width;
    height = m_textureData[0].m_height;

    for (unsigned int i = 0; i < 6; i++) {
      if (m_textureData[i].m_data != nullptr) {
        stbi_set_flip_vertically_on_load(true);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, m_textureData[i].m_data);

        stbi_image_free(m_textureData[i].m_data);
      } else {

        std::cout << "Failed to load texture: " << m_textureData[i].m_filepath << std::endl;
        stbi_image_free(m_textureData[i].m_data);

      }
    }
    


}

void OpenGLCubemapTexture::Bake() {

    /*width = m_textureData[0].m_width;
    height = m_textureData[0].m_height;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID); 

    std::cout << "ID : CubeMap "<< ID << std::endl;

    for (unsigned int i = 0; i < 6; i++) {
        if (m_textureData[i].m_data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_textureData[i].m_data);
            stbi_image_free(m_textureData[i].m_data);
        }
    }
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/
}

unsigned int OpenGLCubemapTexture::GetID() {
  return ID;
}

unsigned int OpenGLCubemapTexture::GetWidth() {
  return width;
}

unsigned int OpenGLCubemapTexture::GetHeight() {
  return height;
}