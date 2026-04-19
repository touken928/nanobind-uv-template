# Shared defaults for every CMake entry point in this repo.
#
# Projects may enter through libs/<pkg>/CMakeLists.txt (standalone C++ build) or
# through packages/nbuv/CMakeLists.txt (scikit-build-core Python build). Both
# load this module so they agree on the basics.

if(NBUV_OPTIONS_INCLUDED)
  return()
endif()
set(NBUV_OPTIONS_INCLUDED TRUE)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/NBUVWarnings.cmake")
