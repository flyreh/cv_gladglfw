#pragma once
#include "./Enviroment/CV_Enviroment.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "glm/gtx/intersect.hpp"
#include <random>
#include <format>
#include <filesystem>
//#include <assimp/matrix3x3.h>
//#include <assimp/matrix4x4.h>
//#include "rapidjson/document.h"
//#include <rapidjson/filereadstream.h>
//#include <rapidjson/istreamwrapper.h>
//#include <rapidjson/writer.h>
//#include <rapidjson/stringbuffer.h>
//#include <rapidjson/ostreamwrapper.h>
//#include <rapidjson/prettywriter.h>



namespace Util {

    inline std::string Lowercase(std::string& str) {
      std::string result = "";
      for (auto& c : str) {
        result += std::tolower(c);
      }
      return result;
    }

    inline std::string Uppercase(std::string& str) {
      std::string result = "";
      for (auto& c : str) {
        result += std::toupper(c);
      }
      return result;
    }

    inline bool FileExists(const std::string_view name) {
      struct stat buffer;
      return (stat(name.data(), &buffer) == 0);
    }

    inline std::string NormalizePath(const std::string& path) {
      std::string normalizedPath = path;
      std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
      return normalizedPath;
    }

	inline FileInfo GetFileInfo(const std::filesystem::directory_entry &filepath)
    {
     
    const auto &path{ filepath.path() };

    static const auto get_material_type{ [] (std::string_view filename) {
        if (filename.size() > 5) {
            filename.remove_prefix(filename.size() - 3);
            if (filename == "ALB" || filename == "RMA" || filename == "NRM") {
                return std::string{ filename };
            }
        }
        return std::string{ "NONE" };
    }};

    const auto stem{ path.has_stem() ? path.stem().string() : "" };

    std::string filetype = path.has_extension() ? path.extension().string().substr(1) : "";  // remove dot

    return FileInfo{
        NormalizePath(path.string()),
        NormalizePath(path.parent_path().string()),
        stem,
        Lowercase(filetype),
        get_material_type(stem)
    };
    }
    inline FileInfo GetFileInfo(std::string filepath) {
        // isolate name
        std::string filename = filepath.substr(filepath.rfind("/") + 1);
        filename = filename.substr(0, filename.length() - 4);
        // isolate filetype
        std::string filetype = filepath.substr(filepath.length() - 3);
        // isolate directory
        std::string directory = filepath.substr(0, filepath.rfind("/") + 1);
        // material name
        std::string materialType = "NONE";
        if (filename.length() > 5) {
            std::string query = filename.substr(filename.length() - 3);
            if (query == "ALB" || query == "RMA" || query == "NRM")
                materialType = query;
        }
        // RETURN IT
        FileInfo info;
        info.fullpath = filepath;
        info.filename = filename;
        info.filetype = filetype;
        info.directory = directory;
        info.materialType = materialType;
        return info;
    }


    inline const std::string GetFilename(const std::string& filepath) {
      std::string result = filepath.substr(filepath.rfind("/") + 1);
      result = result.substr(0, result.length() - 4);
      return result;
    }

  
}