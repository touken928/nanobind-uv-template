# Shared CMake defaults for the nbuv project (C++17, PIC, default build type).
#
# Entry points: libs/nbuv/CMakeLists.txt (standalone C++) or
# packages/nbuv/CMakeLists.txt (scikit-build-core). Add
# ``cmake/nbuv`` to CMAKE_MODULE_PATH, then ``include(NBUVOptions)``.

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
