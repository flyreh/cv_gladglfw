#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

struct Shader {
 public:
  void Load(std::string vertexPath, std::string fragmentPath);
  void Load(std::string vertexPath, std::string fragmentPath, std::string geomPath);
  void Use();
  void SetBool(const std::string& name, bool value);
  void SetInt(const std::string& name, int value);
  void SetFloat(const std::string& name, float value);
  void SetMat4(const std::string& name, glm::mat4 &value);
  void SetMat3(const std::string& name, glm::mat3& value);
  void SetVec3(const std::string& name, const glm::vec3& value);
  void SetVec2(const std::string& name, const glm::vec2& value);
  void SetVec4(const std::string& name, const glm::vec4& value);
  int GetId();

 private:
  int m_ID = -1;
  std::unordered_map<std::string, int> m_uniformsLocations;
};
