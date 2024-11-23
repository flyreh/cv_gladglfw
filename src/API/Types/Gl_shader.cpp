#include "GL_shader.h"
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>

std::string readTextFromFile(std::string path) {
  std::ifstream file(path);
  std::string str;
  std::string line;
  while (std::getline(file, line)) {
    str += line + "\n";
  }
  return str;
}

int checkCompileErrors(unsigned int shader, std::string type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- \n";
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- \n";
    }
  }
  return success;
}

void Shader::Use() {
  glUseProgram(m_ID);
}

void Shader::Load(std::string vertexPath, std::string fragmentPath) {
 
  std::string vertexSource = readTextFromFile(std::string(SHADERS_DIR) + "shaders/" + vertexPath);
  std::string fragmentSource = readTextFromFile(std::string(SHADERS_DIR) + "shaders/" + fragmentPath);

  const char* vShaderCode = vertexSource.c_str();
  const char* fShaderCode = fragmentSource.c_str();


  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");

  int tempID = glCreateProgram();
  glAttachShader(tempID, vertex);
  glAttachShader(tempID, fragment);
  glLinkProgram(tempID);

 m_ID = tempID;
    
  checkCompileErrors(m_ID, "PROGRAM");

    m_uniformsLocations.clear();
   
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  std::cout << "Shader ID LOADED: " << m_ID << std::endl;
}

int Shader::GetId() {
  return m_ID;
}

void Shader::SetMat4(const std::string& name, glm::mat4 &value) {
  glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
  if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
    m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());

  glUniform3fv(m_uniformsLocations[name], 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) {
  if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
    m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
  glUniform2fv(m_uniformsLocations[name], 1, &value[0]);
}

void Shader::SetBool(const std::string& name, bool value) {
  if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
    m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
  glUniform1i(m_uniformsLocations[name], (int)value);
}

void Shader::SetInt(const std::string& name, int value) {
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) {
  if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
    m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
  glUniform1f(m_uniformsLocations[name], value);
}

//incluye GeomPath para cargar shaders de geometría

void Shader::Load(std::string vertexPath, std::string fragmentPath, std::string geomPath) {

  std::string vertexSource = readTextFromFile("resources/shaders/OpenGL/" + vertexPath);
  std::string fragmentSource = readTextFromFile("resources/shaders/OpenGL/" + fragmentPath);
  std::string geometrySource = readTextFromFile("resources/shaders/OpenGL/" + geomPath);

  const char* vShaderCode = vertexSource.c_str();
  const char* fShaderCode = fragmentSource.c_str();
  const char* gShaderCode = geometrySource.c_str();

  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");

  unsigned int geometry = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometry, 1, &gShaderCode, NULL);
  glCompileShader(geometry);
  checkCompileErrors(geometry, "GEOMETRY");

  int tempID = glCreateProgram();
  glAttachShader(tempID, vertex);
  glAttachShader(tempID, fragment);
  glAttachShader(tempID, geometry);
  glLinkProgram(tempID);

  if (checkCompileErrors(tempID, "PROGRAM")) {
    if (m_ID != -1) {
      glDeleteProgram(m_ID);
    }
    m_uniformsLocations.clear();
    m_ID = tempID;
  } else {
    std::cout << "shader failed to compile " << vertexPath << " " << fragmentPath << "\n";
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  glDeleteShader(geometry);
}
