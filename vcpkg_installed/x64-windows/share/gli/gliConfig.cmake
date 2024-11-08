set(GLI_VERSION "0.8.3")


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

####################################################################################

set_and_check(GLI_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/include")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/gliTargets.cmake")
endif()
