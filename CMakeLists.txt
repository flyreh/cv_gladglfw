cmake_minimum_required(VERSION 3.8)

if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("cv_gladglfw")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(gli CONFIG REQUIRED)
find_package(nlohmann_json REQUIRED )
find_package(stb REQUIRED)
find_package (tinyobjloader REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")

# Ruta relativa a resources
set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/src/resources/")

# Agregue un origen al ejecutable de este proyecto.
add_executable(cv_gladglfw ${SOURCES})

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cv_gladglfw PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(cv_gladglfw PRIVATE "${CMAKE_SOURCE_DIR}\\out\\build\\x64-debug\\vcpkg_installed\\x64-windows\\include")
target_link_libraries(cv_gladglfw PRIVATE OpenGL::GL glfw glad::glad glm::glm gli nlohmann_json::nlohmann_json tinyobjloader::tinyobjloader ${stb_INCLUDE_DIR})

target_compile_definitions(cv_gladglfw PRIVATE SHADERS_DIR="${SHADERS_DIR}")